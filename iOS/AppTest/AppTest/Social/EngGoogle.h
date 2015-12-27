//
//  EngGoogle.h
//
//  Created by Pascal Viguié on 10/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#include "Main.h"

#ifdef LIBENG_ENABLE_SOCIAL
#import <Foundation/Foundation.h>
#import "EngSocial.h"
#import "EngController.h"

#import "GooglePlus/GPPSignIn.h"
#import "GooglePlus/GPPShare.h"


@interface EngGoogle : EngSocial <SocialOS, GPPSignInDelegate, GPPShareDelegate> {

    bool displayError;
    NSString* userID;

    NSString* userName;
    unsigned char userGender;
    NSDate* userBirthday;
    NSString* userLocation;

    Session::RequestID requestID;
}
-(id)initWithDisplayError:(bool)display on:(EngController*)viewController;

-(bool)login;
-(void)logout;
-(bool)isLogged;

-(bool)getUserInfo;
-(bool)getUserPicture;

-(bool)shareLink:(NSArray*)data;
-(bool)shareVideo:(NSArray*)data;

-(void)resume;
-(void)terminate;
-(void)remove;

//
@property (nonatomic, strong) EngController* controller;
@property (nonatomic, strong) NSString* picURL;
@property (nonatomic, strong) NSMutableString* shareURL;
@property (nonatomic, strong) NSURL* videoURL;

+(BOOL)openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication annotation:annotation;

@end //

#endif // LIBENG_ENABLE_SOCIAL
