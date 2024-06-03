#ifndef SHADER_LIBRARY_H
#define SHADER_LIBRARY_H

#include "Shader.h"

class ShaderLibrary
{
public:
	static Shared<Shader> GetShader(const std::string& name);
	static Shared<Shader> LoadShader(const std::string& name, const char* vertex_file_path, const char* fragment_file_path);

	static void Reload();

private:
	static std::map<std::string, Shared<Shader>> shaders_;
};

#endif // !SHADER_LIBRARY_H
