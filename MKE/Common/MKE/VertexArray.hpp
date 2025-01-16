#pragma once

#include "MKE/Drawable.hpp"
#include "MKE/Ints.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/Panic.hpp"
#include "MKE/Vertex.hpp"
#include "MKE/RenderTarget.hpp"
#include "glad/glad.h"
#include <cstring>

namespace mk {
	enum class BUFFER_USAGE {
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		STATIC_DRAW  = GL_STATIC_DRAW,
	};

	template<class Vert, BUFFER_USAGE BufferUsage = BUFFER_USAGE::DYNAMIC_DRAW>
	class VertexArray: public NonCopyable, public Drawable {
	public:
		VertexArray(bool enable_index_buffer = false):
			  enable_index_buffer(enable_index_buffer) {
			glGenVertexArrays(1, &vertex_array);
			glBindVertexArray(vertex_array);

			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

			if (enable_index_buffer) {
				glGenBuffers(1, &index_buffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
			}

			Vert::configureVertexAttribute();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		~VertexArray() {
			glDeleteVertexArrays(1, &vertex_array);
			glDeleteBuffers(1, &vertex_buffer);
			if (enable_index_buffer) glDeleteBuffers(1, &index_buffer);
		}

		VertexArray(usize size) { setSize(size); }

		void setSize(const usize size) {
			vertex_buffer_size = size;
			vertices.reset(new Vert[size]);
		}

		void addVertices(const usize count) {
			auto old = std::move(vertices);
			vertices.reset(new Vert[vertex_buffer_size + count]);
			std::memcpy(vertices.get(), old.get(), sizeof(Vert) * vertex_buffer_size);
			vertex_buffer_size += count;
		}

		[[nodiscard]]
		usize getSize() const {
			return vertex_buffer_size;
		}

		void setIndexBuffer(const u32* buffer, usize length) {
			MK_ASSERT(
				enable_index_buffer, "Setting index buffer, but enable_index_buffer == false"
			);
			index_buffer_size = length;
			indices.reset(new u32[length]);
			std::memcpy(indices.get(), buffer, length * sizeof(u32));
			index_buffer_modified = true;
		}

		void setVertexBuffer(const Vert* buffer, usize length) {
			MK_ASSERT_EQUAL(vertex_buffer_size, length, "invalid vertex count");
			std::memcpy(vertices.get(), buffer, length * sizeof(Vert));
			vertex_buffer_modified = true;
		}

		constexpr const Vert& operator()(usize index) const& {
			MK_ASSERT(index < vertex_buffer_size, "VertexArrays\'s index >= size");
			return vertices.get()[index];
		}

		constexpr Vert& operator()(usize index) & {
			MK_ASSERT(index < vertex_buffer_size, "VertexArrays\'s index >= size");
			vertex_buffer_modified = true;
			return vertices.get()[index];
		}

		void save() {
			if (vertex_buffer_modified) {
				vertex_buffer_modified = false;
				glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
				glBufferData(
					GL_ARRAY_BUFFER,
					vertex_buffer_size * sizeof(Vert),
					vertices.get(),
					static_cast<GLenum>(BufferUsage)
				);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			if (index_buffer_modified) {
				index_buffer_modified = false;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
				glBufferData(
					GL_ELEMENT_ARRAY_BUFFER,
					index_buffer_size * sizeof(u32),
					indices.get(),
					static_cast<GLenum>(BufferUsage)
				);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}

	protected:
		u32 vertex_array{};

		u32                   vertex_buffer{};
		usize                 vertex_buffer_size{};
		bool                  vertex_buffer_modified{};
		std::unique_ptr<Vert> vertices{};

		u32                  index_buffer{};
		usize                index_buffer_size{};
		bool                 index_buffer_modified{};
		bool                 enable_index_buffer = false;
		std::unique_ptr<u32> indices{};

		void startDraw() const {
			glBindVertexArray(vertex_array);
			if (enable_index_buffer)
				glDrawElements(GL_TRIANGLES, index_buffer_size, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_size);
		}

		void draw(RenderTarget&, DrawContext context) const override {
			if (!vertex_buffer_size) return;
			context.bind();
			startDraw();
		}
	};

	class VertexArray2D: public VertexArray<Vertex2D> {
	public:
		using VertexArray::VertexArray;
		~VertexArray2D() = default;
		void draw(RenderTarget& target, DrawContext context) const override;
	};

	class VertexArray3D: public VertexArray<Vertex3D> {
	public:
		using VertexArray::VertexArray;
		~VertexArray3D() = default;
		void draw(RenderTarget& target, DrawContext context) const override;
	};
}
