#include "pch.h"
#include "ShaderLibrary.h"

std::map<std::string, Shared<Shader>> ShaderLibrary::shaders_;

Shared<Shader> ShaderLibrary::GetShader(const std::string& name)
{
	if (shaders_.find(name) != shaders_.end()) {
		return shaders_[name];
	}
	return NULL;
}

Shared<Shader> ShaderLibrary::LoadShader(const std::string& name, const char* vertex_file_path, const char* fragment_file_path)
{
	Shared<Shader> shader = MakeShared<Shader>(vertex_file_path, fragment_file_path);
	if (shader->IsCompiled()) {
		shaders_.insert(std::make_pair(name, shader));
		return shader;
	}
	return NULL;
}

void ShaderLibrary::Reload()
{
	for (const auto& kvp : shaders_)
	{
		Shared<Shader> current = kvp.second;

		Shared<Shader> newShader = MakeShared<Shader>(kvp.second->vertexPath().c_str(), kvp.second->fragmentPath().c_str());
		if (newShader->IsCompiled()) {

			current->SwapProgram(newShader);
			//std::cout << "Reloaded Shader: " << kvp.first << '\n';
		}
		else {
			std::cout << "Failed to load shader, not swapping\n";
		}
	}
}
