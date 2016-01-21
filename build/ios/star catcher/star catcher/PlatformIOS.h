//
//  PlatformIOS.h
//  star catcher
//
//  Created by Brooks on 2/15/14.
//  Copyright (c) 2014 Jeremy Brooks. All rights reserved.
//

#ifndef star_catcher_PlatformIOS_h
#define star_catcher_PlatformIOS_h

#include "IPlatform.h"
#import "star_catcherViewController.h"

class PlatformIOS : public IPlatform
{
public:
    std::unique_ptr<Texture> LoadTexture(const std::string& name, bool wrap=false);
	std::unique_ptr<Shader> LoadShader(const std::string& vert_shader_name, const std::string& frag_shader_name);
	std::unique_ptr<Font> LoadFont(const std::string& name, Texture& texture, float ratio);

	std::string GetUserData(const std::string& key);
	void SetUserData(const std::string& key, const std::string& value);
    
    void GameStarted();
    void GameEnded();
    
	PlatformIOS(star_catcherViewController* viewController);
    ~PlatformIOS();
    
    
private:
    star_catcherViewController* viewController;
};

#endif