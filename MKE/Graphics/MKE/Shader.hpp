#pragma once

#include "MKE/Color.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/NonCopyable.hpp"
#include "MKE/ResPath.hpp"

namespace mk {
	class Shader: public NonCopyable {
	public:
		Shader() {}

		template<class T>
		Shader(const T& vertex_shader, const T& fragment_shader) {
			load(vertex_shader, fragment_shader);
		}

		~Shader();

		static void use(const Shader* shader = nullptr);

		void load(const ResPath& vertex_shader, const ResPath& fragment_shader);
		void load(const char* vertex_shader, const char* fragment_shader);
		bool tryLoad(const ResPath& vertex_shader, const ResPath& fragment_shader);
		void deleteProgram();

		void setBool(const std::string& name, bool value) const;
		void setI32(const std::string& name, i32 value) const;
		void setFloat(const std::string& name, float value) const;
		void setColor(const std::string& name, mk::Color color) const;
		void setMatrix4f(const std::string& name, const math::Matrix4f& value) const;
		void setVector3f(const std::string& name, const math::Vector3f& vec) const;
		void setVector3i(const std::string& name, const math::Vector3i& vec) const;

	private:
		bool is_compiled = false;
		u32  program_id  = 0;
	};
}
