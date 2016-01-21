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
#include "ResourceManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Font.h"
#include "SpriteSheet.h"
#include "Entity.h"
#include "IPlatform.h"

ResourceManager::ResourceManager(IPlatform& platform, float ratio)
{
	shaders["flat_colored"] = platform.LoadShader("flat_color.vert", "flat_color.frag");

	shaders["flat_colored"]->Use();
	shaders["flat_colored"]->SetUniform1i("tex0", 0);

	{
		float color[] = { 0.f, 0.f, 0.1f, 1.f,
			0.f, 0.f, 0.6f, 1.f,
			0.f, 0.f, 0.6f, 1.f,
			0.f, 0.f, 0.1f, 1.f };
		meshes["background"].reset(Mesh::createColoredQuad(-1.f, -1.f, 1.f, 1.f, color));
		meshes["background"]->setAttributeLocations(shaders["flat_colored"]->GetAttribLoc("Position"), -1, shaders["flat_colored"]->GetAttribLoc("InTexCoord0"), shaders["flat_colored"]->GetAttribLoc("ColorIn"));
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("starfield", -2.f / ratio, -1.f, 2.f / ratio, 1.f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("bird", -0.1f, -0.1f, 0.1f, 0.1f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("baby_bird", -0.05f, -0.05f, 0.05f, 0.05f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("egg", -0.03f, -0.05f, 0.03f, 0.05f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("star", -0.1f, -0.1f, 0.1f, 0.1f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("blackhole", -0.3f, -0.3f, 0.3f, 0.3f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("zap", -1.f / ratio, -0.1f, 1.f / ratio, 0.1f, color);
	}

	{
		float color[16];
		std::fill_n(color, 16, 1.f);
		this->CreateQuad("startbtn", -1.f, -1.f, 1.f, 1.f, color);
	}

	Shader::Disable();

	textures["default.png"] = platform.LoadTexture("default.png");
	textures["startbtn.png"] = platform.LoadTexture("startbtn.png");
	textures["bird.png"] = platform.LoadTexture("bird.png");
	textures["egg.png"] = platform.LoadTexture("egg.png");
	textures["star.png"] = platform.LoadTexture("star.png");
	textures["blackhole.png"] = platform.LoadTexture("blackhole.png");
	textures["font.png"] = platform.LoadTexture("font.png");
	textures["zap.png"] = platform.LoadTexture("zap.png");
	textures["starfield.png"] = platform.LoadTexture("starfield.png", true);

	Texture::Disable();

	font = platform.LoadFont("font.fnt", *textures["font.png"].get(), ratio);

	buttons["start_button"].reset(new Entity(*meshes["startbtn"].get(), *textures["startbtn.png"].get()));

	entities["bird"].reset(new Entity(*meshes["bird"], *textures["bird.png"], new SpriteSheet(0.f, 2.f, 3.f, 1.f, 3.f, (float)textures["bird.png"]->image.width, (float)textures["bird.png"]->image.height)));
	entities["egg"].reset(new Entity(*meshes["egg"], *textures["egg.png"]));
	entities["0_starfield"].reset(new Entity(*meshes["starfield"].get(), *textures["starfield.png"].get()));
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::CreateQuad(const std::string& name, float left, float top, float right, float bottom, float* color)
{
	if(meshes.find(name) != meshes.end()){
		fprintf(stderr, "in ResourceManager::CreateQuad, Mesh already exists.");
        return;
	}
	const Shader& flat_color_shader = *shaders["flat_colored"].get();
	meshes[name].reset(Mesh::createColoredQuad(left, top, right, bottom, color));
	meshes[name]->setAttributeLocations(flat_color_shader.GetAttribLoc("Position"), -1, flat_color_shader.GetAttribLoc("InTexCoord0"), flat_color_shader.GetAttribLoc("ColorIn"));
}

void ResourceManager::CreatePolygon(const std::string& name, const std::vector<unsigned short>& indices, const std::vector<float>& vertices)
{
	if(meshes.find(name) != meshes.end()){
		fprintf(stderr, "in ResourceManager::CreatePolygon, Mesh already exists.");
        return;
	}
	const Shader& flat_color_shader = *shaders["flat_colored"].get();
	meshes[name].reset(new Mesh(indices, vertices));
	meshes[name]->setAttributeLocations(flat_color_shader.GetAttribLoc("Position"), -1, flat_color_shader.GetAttribLoc("InTexCoord0"), flat_color_shader.GetAttribLoc("ColorIn"));
}