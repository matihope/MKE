#pragma once

#include "MKE/Ints.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/ResPath.hpp"

namespace mk {
	class Shader {
	public:
		Shader() {};
		Shader(const ResPath& vertex_shader, const ResPath& fragment_shader);
		~Shader();

		void use() const;

		void load(const ResPath& vertex_shader, const ResPath& fragment_shader);
		bool tryLoad(const ResPath& vertex_shader, const ResPath& fragment_shader);
		void destroy();

		void setBool(const std::string& name, bool value) const;
		void setI32(const std::string& name, i32 value) const;
		void setFloat(const std::string& name, float value) const;

		void setMatrix4f(const std::string& name, const math::Matrix4f& value) const;

	private:
		bool is_compiled = false;
		u32  program_id  = 0;
	};
}
