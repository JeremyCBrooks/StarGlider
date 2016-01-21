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
#include "Font.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Shader.h"

#include "platform.h"

#include <vector>
#include <sstream>

Font::Font(const std::string& font_descriptor, Texture& texture, float ratio) :
	texture(texture),
	ratio(ratio) 
{
	std::string Line;
	std::string Read, Key, Value;
	std::size_t i;
	std::stringstream Stream(font_descriptor);

	while (!Stream.eof())
	{
		std::stringstream LineStream;
		std::getline(Stream, Line);
		LineStream << Line;

		//read the line's type
		LineStream >> Read;
		if (Read == "info")
		{
			while (!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				//assign the correct value
				Converter << Value;
				if (Key == "size")
					Converter >> charset.Size;
			}
		}
		else if (Read == "common")
		{
			//this holds common data
			while (!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				//assign the correct value
				Converter << Value;
				if (Key == "lineHeight")
					Converter >> charset.LineHeight;
				else if (Key == "base")
					Converter >> charset.Base;
				else if (Key == "scaleW")
					Converter >> charset.Width;
				else if (Key == "scaleH")
					Converter >> charset.Height;
				else if (Key == "pages")
					Converter >> charset.Pages;
			}
		}
		else if (Read == "char")
		{
			//this is data for a specific char
			short CharID = 0;

			while (!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				//assign the correct value
				Converter << Value;
				if (Key == "id")
					Converter >> CharID;
				else if (Key == "x")
					Converter >> charset.Chars[CharID].x;
				else if (Key == "y")
					Converter >> charset.Chars[CharID].y;
				else if (Key == "width")
					Converter >> charset.Chars[CharID].Width;
				else if (Key == "height")
					Converter >> charset.Chars[CharID].Height;
				else if (Key == "xoffset")
					Converter >> charset.Chars[CharID].XOffset;
				else if (Key == "yoffset")
					Converter >> charset.Chars[CharID].YOffset;
				else if (Key == "xadvance")
					Converter >> charset.Chars[CharID].XAdvance;
				else if (Key == "page")
					Converter >> charset.Chars[CharID].Page;
			}
		}
	}

	std::vector<float> vertices(256*4*9);
	std::vector<unsigned short> indices(256*6);
	for( unsigned int i = 0; i < 256; ++i )
	{
		float CharX = (float)charset.Chars[i].x;
		float CharY = (float)charset.Chars[i].y;
		float Width = (float)charset.Chars[i].Width;
		float Height = (float)charset.Chars[i].Height;
		float OffsetX = (float)charset.Chars[i].XOffset;
		float OffsetY = (float)charset.Chars[i].YOffset;
		float charsetWidth = (float)charset.Width;
		float charsetHeight = (float)charset.Height;
		float size = (float)charset.Size;

		const int ii = i*4*9;

		//lower left
		//x,y,z
		vertices[ii] = OffsetX / size;
		vertices[ii + 1] = 1.f - ((OffsetY + Height) / size);
		vertices[ii+2] = 0.f;
		//u,v
		vertices[ii+3] = CharX / charsetWidth;
		vertices[ii + 4] = (CharY + Height) / charsetHeight;
		//r,g,b,a
		vertices[ii+5] = 1.f;
		vertices[ii+6] = 1.f;
		vertices[ii+7] = 1.f;
		vertices[ii+8] = 1.f;

		//upper left
		//x,y,z
		vertices[ii + 9] = OffsetX / size;
		vertices[ii + 10] = 1.f - (OffsetY / size);
		vertices[ii+11] = 0.f;
		//u,v
		vertices[ii + 12] = CharX / charsetWidth;
		vertices[ii + 13] = CharY / charsetHeight;
		//r,g,b,a
		vertices[ii+14] = 1.f;
		vertices[ii+15] = 1.f;
		vertices[ii+16] = 1.f;
		vertices[ii+17] = 1.f;

		//upper right
		//x,y,z
		vertices[ii + 18] = (OffsetX + Width) / size;
		vertices[ii + 19] = 1.f - (OffsetY / size);
		vertices[ii+20] = 0.f;
		//u,v
		vertices[ii+21] = (CharX + Width) / charsetWidth;
		vertices[ii + 22] = CharY / charsetHeight;
		//r,g,b,a
		vertices[ii+23] = 1.f;
		vertices[ii+24] = 1.f;
		vertices[ii+25] = 1.f;
		vertices[ii+26] = 1.f;

		//lower right
		//x,y,z
		vertices[ii + 27] = (OffsetX + Width) / size;
		vertices[ii + 28] = 1.f - ((OffsetY + Height) / size);
		vertices[ii+29] = 0.f;
		//u,v
		vertices[ii+30] = (CharX + Width) / charsetWidth;
		vertices[ii+31] = (CharY + Height) / charsetHeight;
		//r,g,b,a
		vertices[ii+32] = 1.f;
		vertices[ii+33] = 1.f;
		vertices[ii+34] = 1.f;
		vertices[ii+35] = 1.f;

		indices[i*6]   = i*4+2;
		indices[i*6+1] = i*4+1;
		indices[i*6+2] = i*4;

		indices[i*6+3] = i*4+3;
		indices[i*6+4] = i*4+2;
		indices[i*6+5] = i*4;
	}
	vbo.reset(new VertexBuffer(indices, vertices));

	checkGLError();
}

Font::~Font()
{
}

void Font::Display(const std::string& text, const Vector3D& position, const Matrix& camera, const Shader& shader, float scale)const
{
	float MVM[16];
	Matrix modelView;
	modelView.Scale(scale, scale, scale);
	modelView.Concatenate(camera);
	modelView.GetMatrixArray(MVM);

	//set start position
	MVM[12] = position.x;
	MVM[13] = position.y;
	MVM[14] = position.z;

	int vertexLoc = shader.GetAttribLoc("Position");
	int texCoordLoc = shader.GetAttribLoc("InTexCoord0");
	int colorLoc = shader.GetAttribLoc("ColorIn");

	//bind
	vbo->Use();
	texture.Use();

	const int stride = sizeof(float)*9;
	vbo->Setup(vertexLoc, 3, GL_FLOAT, stride, 0);
	vbo->Setup(texCoordLoc, 2, GL_FLOAT, stride, (sizeof(float)*3));
	vbo->Setup(colorLoc, 4, GL_FLOAT, stride,  (sizeof(float)*5));

	//draw
	for(unsigned int i = 0; i < text.size(); ++i){
		shader.SetUniformMatrix("Model", MVM);
		vbo->Draw(6, sizeof(short)*6*text[i]);
		MVM[12] += (scale*ratio*((float)charset.Chars[(int)text[i]].XAdvance / (float)charset.Size));
	}

	//unbind
	VertexBuffer::Disable(texCoordLoc, vertexLoc);
}

float Font::Width(const std::string& text, float scale)const
{
	float w = 0.f;
	for (unsigned int i = 0; i < text.size(); ++i){
		w += (scale*ratio*((float)charset.Chars[(int)text[i]].XAdvance / (float)charset.Size));
	}
	return w;
}