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
#include "PlatformWin.h"
#include "platform.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"

#include <sstream>
#include <fstream>

PlatformWin::PlatformWin(const std::string& dataPath) : dataPath(dataPath)
{
}

PlatformWin::~PlatformWin()
{
}

std::string PlatformWin::GetUserData(const std::string& key)
{
	return userData["key"];
}

void PlatformWin::GameEnded()
{

}
void PlatformWin::GameStarted()
{

}

void PlatformWin::SetUserData(const std::string& key, const std::string& value)
{
	userData["key"] = value;
}

std::unique_ptr<Texture> PlatformWin::LoadTexture(const std::string& name, bool wrap)
{
	std::string filename = dataPath + name;
	SDL_Surface* surface = IMG_Load(filename.c_str());

	Image image;
	image.channels = surface->format->BytesPerPixel;
	image.width = surface->w;
	image.height = surface->h;
	image.pixels = surface->pixels;
	std::unique_ptr<Texture> texture(new Texture(image, wrap));

	SDL_FreeSurface(surface);

	return texture;
}

//helper functions
std::string getFileContents(const std::string& filename)
{
	std::ifstream infile(filename.c_str(), std::ios::binary);
	infile.seekg(0, std::ios::end);
	std::streamoff length = infile.tellg();
	infile.seekg(0, std::ios::beg);
	std::string buffer((int)length, ' ');
	infile.read(&buffer[0], length);
	return buffer;
}

std::unique_ptr<Shader> PlatformWin::LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name)
{
	std::string vert_shader_fn = dataPath + vert_shader_name;
	std::string frag_shader_fn = dataPath + frag_shader_name;

	fprintf(stderr, "===================\n");
	fprintf(stderr, "Building Shader\n");
	fprintf(stderr, "vertex shader: %s\n", vert_shader_fn.c_str());
	fprintf(stderr, "fragment shader: %s\n", frag_shader_fn.c_str());
	fprintf(stderr, "Loading source...");
	std::string vertex_shader_source = getFileContents(vert_shader_fn);
	std::string fragment_shader_source = getFileContents(frag_shader_fn);
	fprintf(stderr, "done.\n");

	return std::unique_ptr<Shader>(new Shader(vertex_shader_source, fragment_shader_source));
}

std::unique_ptr<Font> PlatformWin::LoadFont(const std::string& name, Texture& texture, float ratio)
{
	return std::unique_ptr<Font>(new Font(getFileContents(dataPath + name), texture, ratio));
}