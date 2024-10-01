#pragma once

#include "MKE/Drawable.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Vertex.hpp"
#include "glad/glad.h"
#include <type_traits>

namespace mk {

    template<class Vert = Vertex>
    requires std::is_base_of_v<Vertex, Vert>
	class VertexArray: public Drawable {
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

	private:
		usize                   size{};
		u32                     vertex_buffer{};
		u32                     vertex_array{};
		u32                     element_buffer{};
		bool                    enable_element_buffer = false;
		std::unique_ptr<Vertex> vertices{};
	};

	class VertexArray2D: public VertexArray<Vertex2D>, public Drawable2D {
		public:

	};
}
