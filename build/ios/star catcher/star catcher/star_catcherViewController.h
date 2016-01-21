//
//  star_catcherViewController.h
//  star catcher
//
//  Created by Brooks on 2/15/14.
//  Copyright (c) 2014 Jeremy Brooks. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <iAd/iAd.h>

#ifdef __cplusplus
extern "C" {
#endif
#import "GADBannerView.h"
#ifdef __cplusplus
}
#endif

@interface star_catcherViewController : GLKViewController <ADBannerViewDelegate, GADBannerViewDelegate>

- (void)gameStarted;
- (void)gameEnded;

@end
