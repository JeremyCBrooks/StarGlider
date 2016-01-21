#ifndef SHADER_H_
#define SHADER_H_

#include <string>

class Shader
{
public:
	Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
	~Shader();

	int GetAttribLoc(const std::string& name)const;
	void SetUniform1f(const std::string& name, float value)const;
	void SetUniform1i(const std::string& name, int value)const;
	void SetUniform2fv(const std::string& name, const float value[2])const;
	void SetUniform4f(const std::string& name, const float* value)const;
	void SetUniformMatrix(const std::string& name, const float* value)const;
	void Use()const;
	static void Disable();

private:
	int program;
	int vertex_shader;
	int fragment_shader;
};

#endif