//
//  EngAdvertising.mm
//  Kaleidoscope
//
//  Created by Pascal Viguié on 09/10/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngAdvertising.h"
#ifdef LIBENG_ENABLE_ADVERTISING

#include <libeng/Advertising/Advertising.h>


@implementation EngAdvertising

-(id)initWithController:(UIViewController *)controller andType:(BOOL)interstitial {
    if (self = [super init]) {

        self.status = static_cast<unsigned char>(eng::Advertising::STATUS_READY);
        adInterstitial = nil;
        if (interstitial == FALSE) {

            adBanner = [[GADBannerView alloc] init];
            adBanner.rootViewController = controller;
            [adBanner setDelegate:self];
            [adBanner setHidden:YES];
        }
        else {
            
            viewController = controller;
            adBanner = nil;
        }
    }
    return self;
}

-(GADBannerView*)getBanner { return adBanner; }
-(GADInterstitial*)getInterstitial { return adInterstitial; }
-(UIViewController*)getViewController { return viewController; }

-(void)createNewInterstitial {

    if (adInterstitial != nil) {
        adInterstitial.delegate = nil;
        [adInterstitial release];
    }
    adInterstitial = [[GADInterstitial alloc] init];
    [adInterstitial setDelegate:self];
}

// GADBannerView
-(void)adViewDidReceiveAd:(GADBannerView*)bannerView {
    if (self.status == static_cast<unsigned char>(eng::Advertising::STATUS_LOADING)) {

        if (adBanner.isHidden)
            self.status = static_cast<unsigned char>(eng::Advertising::STATUS_LOADED);
        else
            self.status = static_cast<unsigned char>(eng::Advertising::STATUS_DISPLAYED);
    }
}
-(void)adView:(GADBannerView*)bannerView didFailToReceiveAdWithError:(GADRequestError*)error {

    NSLog(@"Failed to load: %@ (line:%d)", [error localizedDescription], __LINE__);
    self.status = static_cast<unsigned char>(eng::Advertising::STATUS_FAILED);
}
-(void)adViewWillPresentScreen:(GADBannerView*)bannerView { }
-(void)adViewDidDismissScreen:(GADBannerView*)bannerView { }
-(void)adViewWillDismissScreen:(GADBannerView*)bannerView { }
-(void)adViewWillLeaveApplication:(GADBannerView*)bannerView { }

// GADInterstitial
- (void)interstitialDidReceiveAd:(GADInterstitial*)interstitial {
    self.status = static_cast<unsigned char>(eng::Advertising::STATUS_LOADED);
}
- (void)interstitial:(GADInterstitial*)interstitial didFailToReceiveAdWithError:(GADRequestError*)error {

    NSLog(@"Failed to load: %@ (line:%d)", [error localizedDescription], __LINE__);
    self.status = static_cast<unsigned char>(eng::Advertising::STATUS_FAILED);
}
- (void)interstitialWillPresentScreen:(GADInterstitial*)interstitial { }
- (void)interstitialWillDismissScreen:(GADInterstitial*)interstitial {
    self.status = static_cast<unsigned char>(eng::Advertising::STATUS_READY);
}
- (void)interstitialDidDismissScreen:(GADInterstitial*)interstitial { }
- (void)interstitialWillLeaveApplication:(GADInterstitial*)interstitial { }

-(void)dealloc {

    if (adBanner != nil) {
        adBanner.delegate = nil;
        [adBanner release];
    }
    else if (adInterstitial != nil) {
        adInterstitial.delegate = nil;
        [adInterstitial release];
    }
    [super dealloc];
}

@end

#endif
