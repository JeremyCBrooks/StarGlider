#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

struct Image
{
	int width;
	int height;
	unsigned char channels;
	void* pixels;
};

class Texture
{
public:
	Texture(const Image& image, bool wrap=false);
	Texture(int textureId);
	~Texture();

	void Use()const;
	static void Disable();

	Image image;

private:
	unsigned int textureID;
};

#endif