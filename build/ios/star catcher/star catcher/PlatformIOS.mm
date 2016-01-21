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

#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "PlatformIOS.h"

PlatformIOS::PlatformIOS(star_catcherViewController* viewController):viewController(viewController)
{
}

PlatformIOS::~PlatformIOS()
{
}

NSString* getNSString(const std::string& value)
{
    return [NSString stringWithCString:value.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
}

NSString* getBundlePath(const std::string& name)
{
    return [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:getNSString(name)];
}

std::unique_ptr<Texture> PlatformIOS::LoadTexture(const std::string& name, bool wrap)
{
    UIImage *img = [[UIImage alloc] initWithContentsOfFile:getBundlePath(name)];
    
    CGImageRef spriteImage = img.CGImage;
    if (!spriteImage) {
        fprintf(stderr, "Could not load image %s\n", name.c_str());
        return NULL;
    }
    
    Image image;
	image.channels = 4;
	image.width = (int)CGImageGetWidth(spriteImage);
	image.height = (int)CGImageGetHeight(spriteImage);
    
    image.pixels = (GLubyte *) calloc(image.width*image.height*image.channels, sizeof(GLubyte));
    
    CGContextRef spriteContext = CGBitmapContextCreate(image.pixels, image.width, image.height, 8, image.width*image.channels,
                                                       CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, image.width, image.height), spriteImage);

    Texture* texture = new Texture(image, wrap);

    free(image.pixels);
    
    return std::unique_ptr<Texture>(texture);
}

std::unique_ptr<Shader> PlatformIOS::LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name)
{
    NSString* vert_contents = [NSString stringWithContentsOfFile:getBundlePath(vert_shader_name)
                                                  encoding:NSUTF8StringEncoding
                                                     error:NULL];
    
    NSString* frag_contents = [NSString stringWithContentsOfFile:getBundlePath(frag_shader_name)
                                                        encoding:NSUTF8StringEncoding
                                                           error:NULL];
    
    return std::unique_ptr<Shader>(new Shader(std::string([vert_contents UTF8String]), std::string([frag_contents UTF8String])));
}

std::unique_ptr<Font> PlatformIOS::LoadFont(const std::string& name, Texture& texture, float ratio)
{
    NSString* content = [NSString stringWithContentsOfFile:getBundlePath(name)
                                                        encoding:NSUTF8StringEncoding
                                                           error:NULL];
    return std::unique_ptr<Font>(new Font(std::string([content UTF8String]), texture, ratio));
}

std::string PlatformIOS::GetUserData(const std::string& key)
{
    NSString *value = [[NSUserDefaults standardUserDefaults] stringForKey:getNSString(key)];
    return value==nil ? "" : std::string([value UTF8String]);
}

void PlatformIOS::SetUserData(const std::string& key, const std::string& value)
{
    [[NSUserDefaults standardUserDefaults] setObject:getNSString(value) forKey:getNSString(key)];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

void PlatformIOS::GameStarted()
{
    [viewController gameStarted];
}

void PlatformIOS::GameEnded()
{
    [viewController gameEnded];
}
