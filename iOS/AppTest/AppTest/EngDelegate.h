//
//  EngDelegate.h
//
//  Created by Pascal Viguié on 22/01/14.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Main.h"

#ifdef LIBENG_ENABLE_CAMERA
#import <AVFoundation/AVCaptureOutput.h>
#import <AVFoundation/AVCaptureInput.h>
#endif

#ifdef LIBENG_ENABLE_MIC
#import <AudioUnit/AudioUnit.h>
#import <AVFoundation/AVAudioRecorder.h>
#endif

#import <CoreMotion/CoreMotion.h>
#import "EngResources.h"
#import "EngController.h"

#ifdef LIBENG_ENABLE_ADVERTISING
#import "EngAdvertising.h"
#endif

@interface OpenGLView : UIView {

    CMMotionManager* motion;
    NSTimer* timer;
}
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
-(void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

#ifdef LIBENG_ENABLE_ADVERTISING
@property(nonatomic, strong) EngAdvertising* engAd;

-(void)initAdvUsing:(UIViewController*)controller withAdType:(BOOL)interstitial;
#endif

@end //

#ifdef LIBENG_ENABLE_CAMERA
@interface EngDelegate : UIResponder <UIApplicationDelegate, LibengOS,
#ifdef LIBENG_ENABLE_MIC
                                      AVAudioRecorderDelegate,
#endif
                                      AVCaptureVideoDataOutputSampleBufferDelegate> {
#else
@interface EngDelegate : UIResponder <UIApplicationDelegate,
#ifdef LIBENG_ENABLE_MIC
                                      AVAudioRecorderDelegate,
#endif
                                      LibengOS> {
#endif

    NSDate* timeElapsed;
    EAGLContext* context;
    EngController* controller;

    CMMotionManager* motion;
    NSTimer* accelTimer;

    EngResources* resources;

#ifdef LIBENG_ENABLE_CAMERA
    AVCaptureSession* captureSession;
    AVCaptureDeviceInput* deviceInput;
    AVCaptureVideoDataOutput* videoOutput;
#endif

#ifdef LIBENG_ENABLE_SOCIAL
    unsigned char userGender;
    NSDate* userBirthday;
    NSMutableString* userLocation;
#endif
}

@property(nonatomic, strong) IBOutlet UIWindow* window;
#ifdef LIBENG_ENABLE_CAMERA
@property(atomic) BOOL camReady;
#endif
#ifdef LIBENG_ENABLE_MIC
@property(atomic) BOOL micReady;

@property(nonatomic, strong) AVAudioRecorder* micRecorder;
@property(nonatomic, strong) NSURL* micRecURL;
                                          
-(void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag;
-(void)audioRecorderEncodeErrorDidOccur:(AVAudioRecorder *)recorder error:(NSError *)error;
#endif

-(void)accelerometerUpdate;

-(unsigned char)loadTexture:(unsigned char)Id;
-(void)loadSound:(unsigned char)Id;

-(bool)readFile:(const char*)file;
-(bool)writeFile:(const char*)file withContent:(const wchar_t*)content length:(size_t)len;

#ifdef LIBENG_ENABLE_CAMERA
-(BOOL)initCamera;

-(bool)startCamera:(short)width andHeight:(short)height;
-(bool)stopCamera;
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
      fromConnection:(AVCaptureConnection *)connection;
#endif

#ifdef LIBENG_ENABLE_MIC
-(BOOL)initMic;
-(bool)startMic;
-(void)stopMic;

-(void)initMicRecorder:(NSString*)file withFormat:(int)format withSampleRate:(float)rate
                                      withNumChannels:(int)channels;
-(BOOL)startMicRecorder;
-(BOOL)stopMicRecorder;
#endif

#ifdef LIBENG_ENABLE_INTERNET
-(unsigned char)isConnected;
-(bool)isOnline;
#endif

#ifdef LIBENG_ENABLE_ADVERTISING
-(void)loadAd;
-(void)displayAd:(unsigned char)Id;
-(void)hideAd:(unsigned char)Id;
-(unsigned char)getAdStatus;
#endif

#ifdef LIBENG_ENABLE_SOCIAL
-(id<SocialOS>)create:(unsigned char)socialID byDisplayingError:(bool)display;
-(void)setAdReqInfo:(id<SocialOS>)fromSocial;
#endif

-(void)alertMessage:(const char*)msg withDuration:(double)duration;

@end //
