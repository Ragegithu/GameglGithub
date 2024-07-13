#pragma once

#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string ,int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
public:
	Shader(const std::string& filepath);
	~Shader();

	ShaderProgramSource parseShader(const std::string& filepath);

	void Bind() const;
	void Unbind() const;

	//uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};

