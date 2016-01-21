/*
The MIT License (MIT)

Copyright (c) 2014 Jeremy C Brooks
http://vertexarray.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "Shader.h"
#include "platform.h"

#include <vector>

void PrintProgramInfoLog(int obj)
{
    int infologLength = 1024;
    int charsWritten  = 0;
    char infoLog[1024];
	glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
	fprintf(stderr, "%s", infoLog);
}

void PrintShaderInfoLog(int obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0)
	{
        std::vector<char> infoLog(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, &infoLog[0]);
		fprintf(stderr, "%s", (char*)(&infoLog[0]));
    }
}

Shader::Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source) :program(0),
				vertex_shader(0),
				fragment_shader(0)
{
	// Create Shader And Program Objects
	program = glCreateProgram();
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load Shader Sources
	char* temp = (char*)vertex_shader_source.c_str();
	glShaderSource(vertex_shader, 1, (const char**)&temp, NULL);
	temp = (char*)fragment_shader_source.c_str();
	glShaderSource(fragment_shader, 1, (const char**)&temp, NULL);

	// Compile The Shaders
	fprintf(stderr, "Compiling...");
	glCompileShader(vertex_shader);
	PrintShaderInfoLog(vertex_shader);
	glCompileShader(fragment_shader);
	PrintShaderInfoLog(fragment_shader);
	fprintf(stderr, "done.\n");

	// Attach The Shader Objects To The Program Object
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// Link The Program Object
	fprintf(stderr, "Linking...");
	glLinkProgram(program);
	fprintf(stderr, "done.\n");

	PrintProgramInfoLog(program);
	fprintf(stderr, "===================\n");

	checkGLError();
}

Shader::~Shader()
{
	if(vertex_shader>0){
		glDeleteShader(vertex_shader);
	}
	checkGLError();

	if(fragment_shader>0){
		glDeleteShader(fragment_shader);
	}
	checkGLError();

	if(program>0){
		glDeleteProgram(program);
	}
	checkGLError();

	vertex_shader = 0;
	fragment_shader = 0;
	program = 0;
}

int Shader::GetAttribLoc(const std::string& name)const
{
	return glGetAttribLocation(program, name.c_str());
}

void Shader::Use()const
{
	glUseProgram(program);
}

void Shader::Disable()
{
	glUseProgram(0);
}

int getLoc(int program, const std::string& name)
{
	return glGetUniformLocation(program, name.c_str());
}

void Shader::SetUniform1f(const std::string& name, float value)const
{
	glUniform1f(getLoc(program, name), value);
}

void Shader::SetUniform1i(const std::string& name, int value)const
{
	glUniform1i(getLoc(program, name), value);
}

void Shader::SetUniform4f(const std::string& name, const float* value)const
{
	glUniform4fv(getLoc(program, name), 1, value);
}

void Shader::SetUniformMatrix(const std::string& name, const float* value)const
{
	glUniformMatrix4fv(getLoc(program, name), 1, false, value);
}

void Shader::SetUniform2fv(const std::string& name, const float value[2])const
{
	glUniform2fv(getLoc(program, name), 1, value);
}