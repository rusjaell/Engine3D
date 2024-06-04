#include "pch.h"
#include "Shader.h"
#include <cassert>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertex_file_path, const char* fragment_file_path)
	: compiled_(false), vertexPath_(vertex_file_path), fragmentPath_(fragment_file_path), handle_(0)
{
	// Create the shaders
	unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);
	if (!vertexShaderStream.is_open()) {
		return;
	}

	std::stringstream sstr;
	sstr << vertexShaderStream.rdbuf();
	vertexShaderCode = sstr.str();
	vertexShaderStream.close();

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderId, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShaderId, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderId);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderId, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	handle_ = glCreateProgram();
	glAttachShader(handle_, vertexShaderId);
	glAttachShader(handle_, fragmentShaderId);
	glLinkProgram(handle_);

	glGetProgramiv(handle_, GL_LINK_STATUS, &Result);
	glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(handle_, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(handle_, vertexShaderId);
	glDetachShader(handle_, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	compiled_ = true;
}

std::string Shader::vertexPath() const
{
	return vertexPath_;
}

std::string Shader::fragmentPath() const
{
	return fragmentPath_;
}

bool Shader::IsCompiled() const {
	return compiled_;
}

void Shader::Use() const
{
	glUseProgram(handle_);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

bool Shader::HasUniform(const std::string& name)
{
	int location = glGetUniformLocation(handle_, name.c_str());
	return location != -1;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLuint location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::SetUniform1iv(const std::string& name, int* values, int count)
{
	GLuint location = GetUniformLocation(name);
	glUniform1iv(location, count, values);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLuint location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
	GLuint location = GetUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
	GLuint location = GetUniformLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
	GLuint location = GetUniformLocation(name);
	glUniform4f(location, x, y, z, w);
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	GLuint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SwapProgram(const Shared<Shader>& otherShader)
{
	glDeleteProgram(handle_);
	handle_ = otherShader->handle_;
	uniformCache_.clear();
}

int Shader::GetUniformLocation(const std::string& name) {
	if (uniformCache_.find(name) != uniformCache_.end()) {
		return uniformCache_[name];
	}

	int location = glGetUniformLocation(handle_, name.c_str());
	uniformCache_[name] = location;
	return location;
}
