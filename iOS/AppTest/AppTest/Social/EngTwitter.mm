//
//  EngTwitter.mm
//
//  Created by Pascal Viguié on 10/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngTwitter.h"

#ifdef LIBENG_ENABLE_SOCIAL

//////
@implementation EngTwitter

@synthesize displayError;
@synthesize userID;

@synthesize userName;
@synthesize userGender;
@synthesize userBirthday;
@synthesize userLocation;

-(id)initWithDisplayError:(bool)display on:(EngController*)viewController {

    if (!(self = [super initWith:Network::TWITTER]))
        return nil;

    displayError = display;
    self.controller = [viewController retain];
    userID = nil;

    userName = nil;
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;

    //
    return self;
}

-(bool)login {

    [self.controller alert:@"ERROR: Not implemented yet!" during:2.0];
    return false;
}
-(void)logout {

    if (userID != nil) [userID release];
    if (userName != nil) [userName release];
    if (userBirthday != nil) [userBirthday release];
    if (userLocation != nil) [userLocation release];

    userID = nil;
    userName = nil;
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;
}
-(bool)isLogged { return false; }

-(bool)getUserInfo { return false; }
-(bool)getUserPicture { return false; }

-(bool)shareLink:(NSArray*)data { return false; }
-(bool)shareVideo:(NSArray*)data { return false; }

-(void)resume { }
-(void)terminate { }
-(void)remove {

    if (userID != nil) [userID release];
    if (userName != nil) [userName release];
    if (userBirthday != nil) [userBirthday release];
    if (userLocation != nil) [userLocation release];

    [self.controller autorelease];
    [self release];
}

@end //

#endif // LIBENG_ENABLE_SOCIAL
