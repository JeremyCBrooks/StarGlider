#ifndef Font_H_
#define Font_H_

#include <string>
#include <memory>
#include "VectorMath.h"

class Texture;
class VertexBuffer;
class Shader;

class Font
{
public:
	Font(const std::string& font_descriptor, Texture& texture, float ratio);
    ~Font();
    
	void Display(const std::string& text, const Vector3D& position, const Matrix& camera, const Shader& shader, float scale)const;
	float Width(const std::string& text, float scale)const;

private:

	struct Charset
	{
	private:
		struct CharDescriptor
		{
			short x, y;
			short Width, Height;
			short XOffset, YOffset;
			short XAdvance;
			short Page;
			CharDescriptor() : x(0), y(0), Width(0), Height(0), XOffset(0), YOffset(0), XAdvance(0), Page(0){ }
		};

	public:
		short Size;
		short LineHeight;
		short Base;
		short Width, Height;
		short Pages;
		CharDescriptor Chars[256];
	};

	Charset charset;
	std::unique_ptr<VertexBuffer> vbo;
	Texture& texture;
	float ratio;
};

#endif