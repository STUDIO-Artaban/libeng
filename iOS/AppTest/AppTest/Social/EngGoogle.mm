//
//  EngGoogle.mm
//
//  Created by Pascal Viguié on 10/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngGoogle.h"

#ifdef LIBENG_ENABLE_SOCIAL

#import "GoogleOpenSource/GTLPlusConstants.h"
#import "GoogleOpenSource/GTLServicePlus.h"
#import "GoogleOpenSource/GTLQueryPlus.h"
#import "GoogleOpenSource/GTLPlusPerson.h"

#import "GooglePlus/GPPURLHandler.h"

static NSString* CLIENT_ID = @"1068450800311-jmk41bdij6i65i040rmfemf767di722p.apps.googleusercontent.com";


//////
@implementation EngGoogle

@synthesize displayError;
@synthesize userID;

@synthesize userName;
@synthesize userGender;
@synthesize userBirthday;
@synthesize userLocation;

-(id)initWithDisplayError:(bool)display on:(EngController*)viewController {
    
    if (!(self = [super initWith:Network::GOOGLE]))
        return nil;
    
    displayError = display;
    self.controller = [viewController retain];
    userID = nil;
    
    userName = nil;
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;

    requestID = Session::REQUEST_NONE;

    //
    self.picURL = nil;
    self.shareURL = [[NSMutableString alloc] init];
    self.videoURL = nil;

    GPPSignIn* signIn = [GPPSignIn sharedInstance];
    signIn.clientID = CLIENT_ID;
    signIn.scopes = [NSArray arrayWithObjects:kGTLAuthScopePlusLogin, nil];
    signIn.delegate = self;
    return self;
}

-(void)finishedWithAuth:(GTMOAuth2Authentication*)auth error:(NSError*)error {
    if (error) {
        
        NSLog(@"Login error: %@ (line:%d)", [error description], __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to login!" during:2.5];
        platformLoadSocial(Network::GOOGLE, Session::REQUEST_LOGIN, Request::RESULT_FAILED, 0, 0, NULL);
        return;
    }
    platformLoadSocial(Network::GOOGLE, Session::REQUEST_LOGIN, Request::RESULT_SUCCEEDED, 0, 0, NULL);
}
-(void)didDisconnectWithError:(NSError*)error {
    if (error) {
        
        NSLog(@"ERROR: Failed to disconnect (line:%d)", __LINE__);
        return;
    }
    if (userID != nil) [userID release];
    if (userName != nil) [userName release];
    if (userBirthday != nil) [userBirthday release];
    if (userLocation != nil) [userLocation release];
    
    userID = nil;
    userName = nil;
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;
    
    if (self.picURL != nil) [self.picURL release];
    self.picURL = nil;
}

-(bool)login {
    
    //[self.controller alert:@"ERROR: Not implemented yet!" during:2.0];
    //return false;
    
    // WARNING: Not implemented due to issues when try to share a link
    //          * No 'finishedSharing' delegate method call
    //          * No deeplink shared but URL shared (without description)
    
    if (![EngController isOnline]) {
        
        NSLog(@"ERROR: Device not connected (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Device not connected!" during:2.5];
        return false;
    }
    
    //
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        if ([[GPPSignIn sharedInstance] trySilentAuthentication] == NO)
            [[GPPSignIn sharedInstance] authenticate];
    }];
    return true;
}
-(void)logout {
    
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        [[GPPSignIn sharedInstance] disconnect];
    }];
}
-(bool)isLogged { return [[GPPSignIn sharedInstance] authentication] != nil; }

#define GOOGLE_PICTURE_SIZE     256

-(bool)getUserInfo {
    
    GTMOAuth2Authentication* auth = [[GPPSignIn sharedInstance] authentication];
    if (auth == nil) {
        
        NSLog(@"ERROR (Request info): No user connected (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to get user info!" during:2.5];
        return false;
    }
    GTLServicePlus* plusService = [[[GTLServicePlus alloc] init] autorelease];
    plusService.retryEnabled = YES;
    [plusService setAuthorizer:(id<GTMFetcherAuthorizationProtocol>)auth];
    GTLQueryPlus* query = [GTLQueryPlus queryForPeopleGetWithUserId:@"me"];
    
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        [plusService executeQuery:query
                completionHandler:^(GTLServiceTicket* ticket, GTLPlusPerson* person, NSError* error) {
                    if (error) {
                        
                        NSLog(@"ERROR (Request info): Failed to get user info (line:%d)", __LINE__);
                        if (displayError)
                            [self.controller alert:@"ERROR: Failed to get user info!" during:2.5];
                        platformLoadSocial(Network::GOOGLE, Session::REQUEST_INFO, Request::RESULT_FAILED, 0, 0, NULL);
                    }
                    else {
                        
                        if ((userID == nil) && (person.identifier != nil))
                            userID = [[NSString alloc] initWithString:person.identifier];
                        if ((userID == nil) || (person.gender == nil)) {
                            
                            NSLog(@"ERROR (Request info): Failed to get user info (line:%d)", __LINE__);
                            if (displayError)
                                [self.controller alert:@"ERROR: Failed to get user info!" during:2.5];
                            platformLoadSocial(Network::GOOGLE, Session::REQUEST_INFO, Request::RESULT_FAILED,
                                               0, 0, NULL);
                        }
                        else {
                            
                            if ([person.gender isEqualToString:@"male"])
                                userGender = GENDER_MALE;
                            else
                                userGender = GENDER_FEMALE;
                            if ((userName == nil) && (person.displayName != nil))
                                userName = [[NSString alloc] initWithString:person.displayName];
                            if ((self.picURL == nil) && (person.image != nil) && (person.image.url != nil)) {
                                
                                NSString* URL = [[NSMutableString alloc] initWithString:[person.image.url
                                                                                         substringToIndex:([person.image.url length] - 2)]];
                                
                                // https://lh3.googleusercontent.com/-XdUIqdMkCWA/AAAAAAAAAAI/AAAAAAAAAAA/4252rscbv5M/photo.jpg?sz=50
                                // With: "sz=50" replaced by "sz=256" in order to request expected picture size
                                self.picURL = [[NSString alloc] initWithFormat:@"%@%d", URL, GOOGLE_PICTURE_SIZE];
                                [URL release];
                            }
                            if ((userBirthday == nil) && (person.birthday != nil)) {
                                
                                NSDateFormatter* dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
                                dateFormatter.dateFormat = @"yyyy-MM-dd";
                                [dateFormatter setTimeZone:[NSTimeZone timeZoneWithAbbreviation:@"GMT"]];
                                userBirthday = [[NSDate alloc] initWithTimeInterval:0 sinceDate:[dateFormatter
                                                                                                 dateFromString:person.birthday]];
                            }
                            if ((userLocation == nil) && (person.currentLocation != nil))
                                userLocation = [[NSString alloc] initWithString:person.currentLocation];
                            
                            platformLoadSocial(Network::GOOGLE, Session::REQUEST_INFO, Request::RESULT_SUCCEEDED,
                                               0, 0, NULL);
                        }
                    }
                }];
    }];
    return true;
}
-(bool)getUserPicture {
    
    if ([[GPPSignIn sharedInstance] authentication] == nil) {
        
        NSLog(@"ERROR (Request picture): No user connected (line:%d)", __LINE__);
        return false;
    }
    [super getPictureFrom:self.picURL];
    return true;
}

#define SHARE_FIELD_URL     0 // Link URL (REQUEST_SHARE_LINK) & Video file name (REQUEST_SHARE_VIDEO)

-(bool)shareLink:(NSArray*)data {
    
    if (data == nil) {
        
        NSLog(@"WARNING: No link data to share (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share link!" during:2.5];
        return false;
    }

    //
    if ([[GPPSignIn sharedInstance] authentication] == nil) {
        
        NSLog(@"ERROR (Share link): No user connected (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share link!" during:2.5];
        return false;
    }
    [self.shareURL setString:[data objectAtIndex:SHARE_FIELD_URL]];
    [GPPShare sharedInstance].delegate = self;
    requestID = Session::REQUEST_SHARE_LINK;
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        
        id<GPPShareBuilder> shareDlg = [[GPPShare sharedInstance] shareDialog];
        [shareDlg setURLToShare:[NSURL URLWithString:self.shareURL]];
        [shareDlg setContentDeepLinkID:@"/libeng/"];
        [shareDlg setCallToActionButtonWithLabel:@"VIEW" URL:[NSURL URLWithString:self.shareURL]
                                      deepLinkID:@"/libeng/view"];
        [shareDlg open];
    }];
    return true;
}
-(bool)shareVideo:(NSArray*)data {

    if (data == nil) {
        
        NSLog(@"WARNING: No video data to share (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share video!" during:2.5];
        return false;
    }
    
    //
    if ([[GPPSignIn sharedInstance] authentication] == nil) {
        
        NSLog(@"ERROR (Share link): No user connected (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share video!" during:2.5];
        return false;
    }
    [self.shareURL setString:[data objectAtIndex:SHARE_FIELD_URL]];
    [GPPShare sharedInstance].delegate = self;
    requestID = Session::REQUEST_SHARE_VIDEO;
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{

        id<GPPNativeShareBuilder> shareDlg = [[GPPShare sharedInstance] nativeShareDialog];
        if (self.videoURL != nil)
            [self.videoURL release];
        self.videoURL = [NSURL fileURLWithPath:self.shareURL];
        [shareDlg attachVideoURL:self.videoURL];
        [shareDlg open];
    }];
    return true;
}
-(void)finishedSharing:(BOOL)shared {

    if ((shared) && (displayError)) {
        switch (requestID) {
            case Session::REQUEST_SHARE_LINK:
                [self.controller alert:@"INFO: Google link shared!" during:2.5];
                break;

            case Session::REQUEST_SHARE_VIDEO:
                [self.controller alert:@"INFO: Video shared on Google+!" during:2.5];
                break;
        }
    }
    platformLoadSocial(Network::GOOGLE, requestID, Request::RESULT_SUCCEEDED, 0, 0, NULL);
}

+(BOOL)openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication annotation:annotation {
    return [GPPURLHandler handleURL:url sourceApplication:sourceApplication annotation:annotation];
}

-(void)resume { }
-(void)terminate { }
-(void)remove {
    
    if (self.picURL != nil) [self.picURL release];
    if (self.videoURL != nil) [self.videoURL release];
    [self.shareURL release];

    if (userID != nil) [userID release];
    if (userName != nil) [userName release];
    if (userBirthday != nil) [userBirthday release];
    if (userLocation != nil) [userLocation release];
    
    [self.controller autorelease];
    [self release];
}

@end //

#endif // LIBENG_ENABLE_SOCIAL
