#include "Shader.hpp"
#include "MKE/Exceptions.hpp"
#include "MKE/Ints.hpp"
#include "MKE/Panic.hpp"
#include "glad/glad.h"

using mk::exceptions::MkException;

void mk::Shader::load(const ResPath& vertex_shader, const ResPath& fragment_shader) {
	std::string vertex_shader_content   = vertex_shader.readContent();
	std::string fragment_shader_content = fragment_shader.readContent();
	load(vertex_shader_content.c_str(), fragment_shader_content.c_str());
}

bool mk::Shader::tryLoad(const ResPath& vertex_shader, const ResPath& fragment_shader) {
	try {
		load(vertex_shader, fragment_shader);
		return true;
	} catch (exceptions::MkException& e) {
		MK_LOG_ERROR("in Shader::tryLoad(), continuing after", e.what());
		return false;
	}
}

void mk::Shader::deleteProgram() {
	if (is_compiled) glDeleteProgram(program_id);
	is_compiled = false;
}

mk::Shader::~Shader() { this->deleteProgram(); }

void mk::Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int) value);
}

void mk::Shader::setI32(const std::string& name, i32 value) const {
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void mk::Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void mk::Shader::setMatrix4f(const std::string& name, const math::Matrix4f& value) const {
	glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_TRUE, value.data());
}

void mk::Shader::use(const Shader* shader) {
	// do not rebind if bound ;O
	if (shader) {
		MK_ASSERT(shader->is_compiled, "Using uncompiled shader...");
		glUseProgram(shader->program_id);
	} else {
		glUseProgram(0);
	}
}

void mk::Shader::load(const char* vertex_shader, const char* fragment_shader) {
	deleteProgram();

	u32 vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
	glCompileShader(vertex_shader_id);

	int  success;
	char info_log[512];
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
		throw MkException("Vertex shader compilation has failed:\n", info_log);
	}

	u32 fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
	glCompileShader(fragment_shader_id);

	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
		throw MkException("Fragment shader compilation has failed:\n", info_log);
	}

	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program_id, 512, NULL, info_log);
		throw MkException("Shader program compilation has failed:\n", info_log);
	}

	is_compiled = true;
}

void mk::Shader::setColor(const std::string& name, mk::Color color) const {
	glUniform4f(glGetUniformLocation(program_id, name.c_str()), color.r, color.g, color.b, color.a);
}
