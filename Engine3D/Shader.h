#ifndef SHADER_H
#define SHADER_H

#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <vector>
#include <unordered_map>

class Shader
{
public:
	Shader(const char* vertex_file_path, const char* fragment_file_path);

	std::string vertexPath() const;
	std::string fragmentPath() const;

	bool IsCompiled() const;

	void Use() const;
	void Unbind() const;

	bool HasUniform(const std::string& name);

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, int* values, int count);

	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

	void SwapProgram(const Shared<Shader>& otherShader);

private:
	int GetUniformLocation(const std::string& name);

private:
	std::string vertexPath_;
	std::string fragmentPath_;

	unsigned int handle_;
	bool compiled_;

	std::unordered_map<std::string, int> uniformCache_;
};

#endif // !SHADER_H