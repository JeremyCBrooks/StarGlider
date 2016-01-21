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
    
#import "star_catcherViewController.h"
#import "../../../../src/core/Game.h"
#import "PlatformIOS.h"
#import "InAppPurchaseManager.h"

Game* game;

@interface star_catcherViewController () {
    ADBannerView *_iAdBannerView;
    GADBannerView *_admobBannerView;
    InAppPurchaseManager* _iap;
    UIButton* _iapButton;
    
    bool suppressAds;
    bool useAdMob;
    bool gotAdMobAd;
    bool adFreeIAP;
    bool doingIAP;
}
@property (nonatomic, strong) EAGLContext *context;
@property (nonatomic, strong) ADBannerView *iAdBannerView;
@property (nonatomic, strong) GADBannerView *admobBannerView;
@property (nonatomic, strong) InAppPurchaseManager* iap;
@property (nonatomic, strong) UIButton* iapButton;

@end

@implementation star_catcherViewController

@synthesize iAdBannerView = _iAdBannerView;
@synthesize admobBannerView = _admobBannerView;
@synthesize iap = _iap;
@synthesize iapButton = _iapButton;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveAdFreeIAPPurchasedNotification:)
                                                 name:kInAppPurchaseManagerTransactionSucceededNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(receiveAdFreeIAPPurchasedNotification:)
                                                 name:kInAppPurchaseManagerTransactionFailedNotification
                                               object:nil];
    
    _iAdBannerView = nil;
    _admobBannerView = nil;
    suppressAds = false;
    useAdMob = false;
    gotAdMobAd = false;
    doingIAP = false;
    
    adFreeIAP = [[NSUserDefaults standardUserDefaults] boolForKey:@"isAdFreeIAPPurchased"];
    
    if(!adFreeIAP){
        [self addIAPButton];
        [self showAds:YES];
        self.iap = [InAppPurchaseManager alloc];
    }
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    self.preferredFramesPerSecond = 60;
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;

    [EAGLContext setCurrentContext:self.context];
    
    game = new Game(new PlatformIOS(self), (float)self.view.bounds.size.width, (float)self.view.bounds.size.height);
}

- (void)addIAPButton{
    self.iapButton = nil;
    return;
    
    self.iapButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    self.iapButton.frame = CGRectMake(0, 0, 100, 100);
    self.iapButton.backgroundColor = [UIColor clearColor];
    [self.iapButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal ];
    UIImage *buttonImageNormal = [UIImage imageNamed:@"buy_full_version.png"];
//    UIImage *strechableButtonImageNormal = [buttonImageNormal stretchableImageWithLeftCapWidth:12 topCapHeight:0];
    [self.iapButton setBackgroundImage:buttonImageNormal forState:UIControlStateNormal];
    UIImage *buttonImagePressed = [UIImage imageNamed:@"buy_full_version.png"];
//    UIImage *strechableButtonImagePressed = [buttonImagePressed stretchableImageWithLeftCapWidth:12 topCapHeight:0];
    [self.iapButton setBackgroundImage:buttonImagePressed forState:UIControlStateHighlighted];
    [self.iapButton addTarget:self action:@selector(purchaseAdFreeIAP) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:self.iapButton];
}

- (void)purchaseAdFreeIAP{
    if(doingIAP){
        return;
    }
    doingIAP = true;
    [self.iap purchaseAdFreeIAP];
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)gameStarted
{
    suppressAds = true;
    [self showAds:NO];
}

- (void)gameEnded
{
    useAdMob = false;
    suppressAds = false;
    [self destroyAdViews];
    [self showAds:YES];
}


//ad views////////////////////
- (void)adViewDidReceiveAd:(GADBannerView *)adView {
    NSLog(@"adViewDidReceiveAd");

    gotAdMobAd = true;
    //admob success
    if(self.admobBannerView != nil && useAdMob){
        self.admobBannerView.hidden = NO;
    }
}

- (void)adView:(GADBannerView *)view didFailToReceiveAdWithError:(GADRequestError *)error {
    NSLog(@"didFailToReceiveAdWithError");

    gotAdMobAd = false;

    //admob error
    if(useAdMob){
        [self destroyAdViews];
    }
    suppressAds = true;
    useAdMob = false;
//    [self showAds:YES];
}

- (void)bannerViewDidLoadAd:(ADBannerView *)banner {
    NSLog(@"bannerViewDidLoadAd");

    //iad success
    if(self.iAdBannerView != nil){
        self.iAdBannerView.hidden = NO;
    }
}

- (void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error {
    NSLog(@"didFailToReceiveAdWithError");

    //iad error
    if(!useAdMob){
        [self destroyAdViews];
    }
	//if iAds fails or does not serve up an ad for some reason, fall back to admob
    useAdMob = true;
    [self showAds:YES];
}

- (void)destroyAdViews
{
    if(self.admobBannerView != nil){
        self.admobBannerView.hidden = YES;
        NSLog(@"destroy admobBannerView");
    }
    
    if(self.iAdBannerView != nil){
        [self.iAdBannerView setDelegate:nil];
        [self.iAdBannerView removeFromSuperview];
        self.iAdBannerView = nil;
        NSLog(@"destroy iAdBannerView");
    }
}

- (void)createAdViews
{
    if(adFreeIAP){return;}
    
    NSLog(@"createAdViews");
    if(useAdMob){
        if(self.admobBannerView == nil){
            [self destroyAdViews];

            //create admob view
            CGSize adMobSize = [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad ? GAD_SIZE_728x90 : GAD_SIZE_320x50;
            self.admobBannerView = [[GADBannerView alloc]initWithFrame:CGRectMake(0.0,self.view.frame.size.height-adMobSize.height, adMobSize.width, adMobSize.height)];
            self.admobBannerView.adUnitID = @"ca-app-pub-3940256099942544/2934735716";//this is the test id, you will need to create your own admob id
            [self.admobBannerView setDelegate:self];
            [self.admobBannerView setRootViewController:self];
            [self.view addSubview:self.admobBannerView];
            
            GADRequest *request = [GADRequest request];
#ifdef DEBUG
            request.testDevices = @[ GAD_SIMULATOR_ID ];
            request.testing = YES;
#endif
            [self.admobBannerView loadRequest:request];
            
            NSLog(@"admob created");
        }

        self.admobBannerView.hidden = !gotAdMobAd;
        
    }else if(self.iAdBannerView == nil){
        [self destroyAdViews];

        //create iad view
        self.iAdBannerView = [[ADBannerView alloc] initWithFrame:CGRectZero];
        CGRect adFrame = self.iAdBannerView.frame;
        adFrame.origin.y = self.view.frame.size.height-self.iAdBannerView.frame.size.height;
        self.iAdBannerView.frame = adFrame;
        [self.iAdBannerView setAutoresizingMask:UIViewAutoresizingFlexibleWidth];
        [self.iAdBannerView setDelegate:self];
        [self.view addSubview:self.iAdBannerView];
        self.iAdBannerView.hidden = YES;
        
        NSLog(@"iad created");

    }
}

- (void)showAds:(bool)show
{
    self.iapButton.hidden = !show;
    
    if(adFreeIAP || !show || suppressAds){
        [self destroyAdViews];
        return;
    }
    
    NSLog(@"showAds");

    [self createAdViews];
}

- (void) receiveAdFreeIAPPurchasedNotification:(NSNotification *) notification
{
    NSLog(@"in receiveAdFreeIAPPurchasedNotification %@", notification.name);
    if([notification.name isEqualToString:kInAppPurchaseManagerTransactionSucceededNotification])
    {
        NSLog(@"disable ads");
        useAdMob = false;
        suppressAds = true;
        adFreeIAP = true;

        [self destroyAdViews];
        [self.admobBannerView setDelegate:nil];
        [self.admobBannerView removeFromSuperview];
        self.admobBannerView = nil;
        
        [self.iapButton removeFromSuperview];
        self.iapButton = nil;
    }
    else if(self.iapButton != nil)
    {
        doingIAP = false;
    }
}
//end ad views/////////////

- (void)dealloc
{
    if(game != NULL){
        delete game;
        game = NULL;
    }
    
    [EAGLContext setCurrentContext:self.context];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [self destroyAdViews];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [EAGLContext setCurrentContext:self.context];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
    [self destroyAdViews];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    NSSet *allTouches = [event allTouches];
    for (UITouch *touch in allTouches)
    {
        CGPoint location = [touch locationInView:touch.view];
        game->TouchDown(location.x, self.view.bounds.size.height-location.y);
        break;
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    NSSet *allTouches = [event allTouches];
    for (UITouch *touch in allTouches)
    {
        CGPoint location = [touch locationInView:touch.view];
        game->TouchUp(location.x, self.view.bounds.size.height-location.y);
        break;
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    NSTimeInterval dt = self.timeSinceLastUpdate;
    dt = (dt >= 0.034 ? 0.034 : dt);
    game->Update(dt);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    game->Display();
}

@end