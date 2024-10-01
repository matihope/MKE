#pragma once

#include "MKE/Drawable.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Panic.hpp"
#include "MKE/Vertex.hpp"
#include "glad/glad.h"

namespace mk {
	class RenderTarget;

	template<class Vert>
	class VertexArray {
		friend class RenderTarget;

	public:
		VertexArray(bool enable_element_buffer): enable_element_buffer(enable_element_buffer) {
			glGenVertexArrays(1, &vertex_array);
			glBindVertexArray(vertex_array);

			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

			if (enable_element_buffer) {
				glGenBuffers(1, &element_buffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
			}

			Vert::configureVertexAttribute();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		~VertexArray() {
			glDeleteVertexArrays(1, &vertex_array);
			glDeleteBuffers(1, &vertex_buffer);
			if (enable_element_buffer) glDeleteBuffers(1, &element_buffer);
		}

		VertexArray(usize size) { setSize(size); }

		void setSize(usize size) {
			this->size = size;
			vertices.reset(new Vert[size]);
		}

		void setElementBuffer(const u32* buffer, usize size) {
			MK_ASSERT(element_buffer, "Setting element buffer, but enable_element_buffer == false");
			element_buffer_size = size / sizeof(u32);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void setVertexBuffer(const Vert* buffer, usize size) {
			MK_ASSERT(this->size == size, "invalid vertex count");
			// memcpy(vertices.get(), buffer, size * sizeof(Vert));
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vert), buffer, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		constexpr const Vert& operator()(usize index) const& {
			MK_ASSERT(index < size, "VertexArrays\'s index >= size");
			return vertices[index];
		}

	private:
		usize                 size{};
		usize                 element_buffer_size{};
		u32                   vertex_buffer{};
		u32                   vertex_array{};
		u32                   element_buffer{};
		bool                  enable_element_buffer = false;
		std::unique_ptr<Vert> vertices{};

		void start_draw() const {
			glBindVertexArray(vertex_array);
			if (element_buffer)
				glDrawElements(GL_TRIANGLES, element_buffer_size, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, size);
		}
	};

	class VertexArray2D: public VertexArray<Vertex2D>, public Drawable2D {
	public:
		using VertexArray<Vertex2D>::VertexArray;
		~VertexArray2D() = default;
		void draw(const RenderTarget& target, DrawContext2D context) const override;
	};
}
