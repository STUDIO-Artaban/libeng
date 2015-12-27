//
//  EngAdvertising.h
//  Kaleidoscope
//
//  Created by Pascal Viguié on 09/10/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <libeng/Global.h>
#ifdef LIBENG_ENABLE_ADVERTISING

#import "GoogleMobileAds/GADBannerView.h"
#import "GoogleMobileAds/GADInterstitial.h"


@interface EngAdvertising : NSObject <GADBannerViewDelegate, GADInterstitialDelegate> {

    GADBannerView* adBanner;
    GADInterstitial* adInterstitial;

    UIViewController* viewController;
}
@property(atomic) unsigned char status;

-(id)initWithController:(UIViewController*)controller andType:(BOOL)interstitial;

-(GADBannerView*)getBanner;
-(GADInterstitial*)getInterstitial;
-(UIViewController*)getViewController;

-(void)createNewInterstitial;

@end

#endif
