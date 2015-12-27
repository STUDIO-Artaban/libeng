//
//  EngDelegate.mm
//
//  Created by Pascal Viguié on 22/01/14.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngDelegate.h"

#import <Foundation/Foundation.h>
#import <QuartzCore/CAEAGLLayer.h>

#ifdef LIBENG_ENABLE_CAMERA
#import <AVFoundation/AVMediaFormat.h>
#import <AVFoundation/AVCaptureDevice.h>
#endif

#ifdef LIBENG_ENABLE_MIC
#import <AudioToolbox/AudioSession.h>
#import <AVFoundation/AVAudioSession.h>
#endif

#ifdef LIBENG_ENABLE_ADVERTISING
#include <libeng/Advertising/Advertising.h>
#endif

#ifdef LIBENG_ENABLE_SOCIAL
#import "Social/EngFacebook.h"
#import "Social/EngTwitter.h"
#import "Social/EngGoogle.h"
#endif

#define ACCEL_TIMER_INTERVAL    1/30.0  // Accelerometer update interval (in seconds)
#define ACCEL_MAX_RANGE         25.f // No way to get the maximum accelerometer value on iOS (so 25)


////////////////////////////////////////////////////////////////////////////////////////////

@implementation OpenGLView

-(id)initWithFrame:(CGRect)frame {

    if ((self = [super initWithFrame:frame])) {

        // Assign a variable to be our CAEAGLLayer temporary.
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        // Construct a dictionary with our configurations.
        NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                              kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                              nil];
        
        // Set the properties to CAEALLayer.
        [eaglLayer setOpaque:YES];
        [eaglLayer setDrawableProperties:dict];
    }
    return self;
}
+(Class)layerClass { return [CAEAGLLayer class]; }

////// Touch
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {

    // [touches count]: Finger count
    for (UITouch* touch in touches) {
        CGPoint position = [touch locationInView:self];
        NSUInteger touchId = (unsigned int)(size_t)touch;
        platformTouch(touchId, TOUCH_BEGAN, position.x, position.y);
    }
}
-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {

    for (UITouch* touch in touches) {
        CGPoint position = [touch locationInView:self];
        NSUInteger touchId = (unsigned int)(size_t)touch;
        platformTouch(touchId, TOUCH_MOVED, position.x, position.y);
    }
}
-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
    
    for (UITouch* touch in touches) {
        CGPoint position = [touch locationInView:self];
        NSUInteger touchId = (unsigned int)(size_t)touch;
        platformTouch(touchId, TOUCH_ENDED, position.x, position.y);
    }
}
-(void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event {
    
    for (UITouch* touch in touches) {
        CGPoint position = [touch locationInView:self];
        NSUInteger touchId = (unsigned int)(size_t)touch;
        platformTouch(touchId, TOUCH_CANCELLED, position.x, position.y);
    }
}

////// Advertising
#ifdef LIBENG_ENABLE_ADVERTISING
-(void)initAdvUsing:(UIViewController*)controller withAdType:(BOOL)interstitial {

    self.engAd = [[EngAdvertising alloc] initWithController:controller andType:interstitial];
    if (interstitial == FALSE)
        [self addSubview:[self.engAd getBanner]];
}

-(void)dealloc {

    [self.engAd release];
    [super dealloc];
}
#endif

@end

////////////////////////////////////////////////////////////////////////////////////////////

@implementation EngDelegate

#ifdef LIBENG_ENABLE_SOCIAL
@synthesize userGender;
@synthesize userBirthday;
@synthesize userLocation;

-(BOOL)application:(UIApplication*)application openURL:(NSURL*)url
        sourceApplication:(NSString*)sourceApplication
        annotation:(id)annotation {

    return  [EngFacebook openURL:url sourceApplication:sourceApplication] ||
            [EngGoogle openURL:url sourceApplication:sourceApplication annotation:annotation];
}
#endif

-(BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {

    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];

    // Override point for customization after application launch.
    self.window.backgroundColor = [UIColor blackColor];
    [self.window makeKeyAndVisible];

    resources = [[EngResources alloc] init];

    ////// Initialize
    PlatformData platformData;
    float dpi = [resources getDpi];
    platformData.xDpi = dpi;
    platformData.yDpi = dpi;
    platformData.accelRange = ACCEL_MAX_RANGE;
    platformData.os = self;
    init(platformData);

    timeElapsed = [[NSDate alloc] init];
    platformInit(0, &platformData);

    ////// View controller & view
    controller = [[EngController alloc] init];
#ifdef LIBENG_ENABLE_ADVERTISING
    OpenGLView* openGlView = [[OpenGLView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [openGlView initAdvUsing:controller withAdType:engGetAdType()];
    controller.view = openGlView;
#else
    controller.view = [[OpenGLView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
#endif
    [controller.view setMultipleTouchEnabled:YES];
    [self.window setRootViewController:controller];

    ////// EAGL
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:context];
    
    platformStart(static_cast<short>(controller.view.bounds.size.width),
                  static_cast<short>(controller.view.bounds.size.height), context,
                  (CAEAGLLayer*)controller.view.layer);

    ////// Accelerometer
    motion = [[CMMotionManager alloc] init];

    ////// Camera
#ifdef LIBENG_ENABLE_CAMERA
    self.camReady = [self initCamera];
#endif

    ////// Mic
#ifdef LIBENG_ENABLE_MIC
    self.micReady = [self initMic];
    self.micRecorder = nil;
    self.micRecURL = nil;
#endif

#ifdef LIBENG_ENABLE_SOCIAL
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;
#endif
    return YES;
}
-(void)applicationWillResignActive:(UIApplication*)application {

    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.

    ////// Accelerometer
    [motion stopAccelerometerUpdates];
    [accelTimer invalidate];
    accelTimer = nil;

    platformPause();
}
-(void)applicationDidEnterBackground:(UIApplication*)application {

    // Use this method to release shared resources, save user data, invalidate timers, and store enough application
    // state information to restore your application to its current state in case it is terminated later.
}
-(void)applicationWillEnterForeground:(UIApplication*)application {

    // Called as part of the transition from the background to the inactive state;
    // here you can undo many of the changes made on entering the background.
}
-(void)applicationDidBecomeActive:(UIApplication*)application {

    // Restart any tasks that were paused (or not yet started) while the application was inactive.
    [motion startAccelerometerUpdates];
    accelTimer = [NSTimer scheduledTimerWithTimeInterval:ACCEL_TIMER_INTERVAL target:self
                                                selector:@selector(accelerometerUpdate) userInfo:nil repeats:YES];

    platformResume((long)([timeElapsed timeIntervalSinceNow] * -8000.0)); // In milliseconds
}
-(void)applicationWillTerminate:(UIApplication*)application {

    // Called when the application is about to terminate.

    ////// Accelerometer
    [motion stopAccelerometerUpdates];
    [accelTimer invalidate];
    accelTimer = nil;

    platformFree();
}

////// Accelerometer
-(void)accelerometerUpdate {

    platformAccelerometer((float)motion.accelerometerData.acceleration.x,
                          (float)motion.accelerometerData.acceleration.y,
                          (float)motion.accelerometerData.acceleration.z);
}

////// Textures
#define TEXTURE_ID_LOGO         0 // Reserved
#define TEXTURE_ID_FONT         1

-(unsigned char)loadTexture:(unsigned char)Id {

    switch (Id) {
        case TEXTURE_ID_LOGO: {
            unsigned char* data = [resources getBufferPNG:@"logo" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_LOGO, static_cast<short>(resources.pngWidth),
                                static_cast<short>(resources.pngHeight), data);
        }
        case TEXTURE_ID_FONT: {
            unsigned char* data = [resources getBufferPNG:@"font" inGrayScale:engGetFontGrayscale()];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(TEXTURE_ID_FONT, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data,
                                       (engGetFontGrayscale() != NO)? true:false);
        }

#ifdef LIBENG_ENABLE_SOCIAL
        case Facebook::TEXTURE_ID: {
            unsigned char* data = [resources getBufferPNG:@"facebook" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Facebook::TEXTURE_ID, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case Twitter::TEXTURE_ID: {
            unsigned char* data = [resources getBufferPNG:@"twitter" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Twitter::TEXTURE_ID, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
        case Google::TEXTURE_ID: {
            unsigned char* data = [resources getBufferPNG:@"google" inGrayScale:NO];
            if (data == NULL) {
                NSLog(@"ERROR: Failed to get PNG buffer (line:%d)", __LINE__);
                break;
            }
            return platformLoadTexture(Google::TEXTURE_ID, static_cast<short>(resources.pngWidth),
                                       static_cast<short>(resources.pngHeight), data);
        }
#endif
    }
    return engLoadTexture(resources, Id);
}

////// Sounds
#define SOUND_ID_LOGO   0 // Reserved

-(void)loadSound:(unsigned char)Id {

    if (Id != SOUND_ID_LOGO)
        engLoadSound(resources, Id);
    else {

        unsigned char* data = [resources getBufferOGG:@"logo"];
        if (data == NULL) {
            NSLog(@"ERROR: Failed to get OGG buffer (line:%d)", __LINE__);
            return;
        }
        platformLoadOgg(SOUND_ID_LOGO, resources.oggLength, data, true);
    }
}

////// Storage
#define FILE_NOT_FOUND  L"#FILE_NOT_FOUND#"

-(bool)readFile:(const char*)file {
    @autoreleasepool {

        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
        NSString* libDir = [paths objectAtIndex:0];
        NSString* filePath = [NSString stringWithFormat:@"%@/%s", libDir, file];

        if ([[NSFileManager defaultManager] fileExistsAtPath:filePath] == NO) {

            platformLoadFile(file, FILE_NOT_FOUND);
            return true;
        }
        NSError* err;
        NSData* content = [[NSData alloc] initWithContentsOfFile:filePath
                                                    options:NSDataReadingMappedIfSafe error:&err];
        if (content == nil) {

            NSLog(@"ERROR: Failed to read existing '%@' file (%@)", filePath, [err localizedFailureReason]);
            return false;
        }
        platformLoadFile(file, reinterpret_cast<const wchar_t*>([content bytes]));
    }
    return true;
}
-(bool)writeFile:(const char*)file withContent:(const wchar_t*)content length:(size_t)len {
    @autoreleasepool {

        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
        NSString* libDir = [paths objectAtIndex:0];
        NSString* filePath = [NSString stringWithFormat:@"%@/%s", libDir, file];

        NSData* toWrite = [[NSData alloc] initWithBytes:content length:((len + 1) * sizeof(wchar_t))];

        NSError* err;
        if ([toWrite writeToFile:filePath options:NSDataWritingAtomic error:&err] == NO) {

            NSLog(@"ERROR: Failed to write '%@' file (%@)", filePath, [err localizedFailureReason]);
            return false;
        }
    }
    return true;
}

////// Camera
#ifdef LIBENG_ENABLE_CAMERA
-(BOOL)initCamera {

	captureSession = nil;
	videoOutput = nil;
	deviceInput = nil;

	AVCaptureDevice* backCamera = nil;
	NSArray* deviceArray = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	for (AVCaptureDevice* device in deviceArray)
        if ([device position] == AVCaptureDevicePositionBack)
            backCamera = device;

    if (backCamera == nil) {

        NSLog(@"ERROR: No back-facing camera found (line:%d)", __LINE__);
        return FALSE;
    }
	NSError* err = nil;
    if ([backCamera lockForConfiguration:&err] == YES) {

        if ([backCamera isFocusModeSupported:AVCaptureFocusModeLocked])
            [backCamera setFocusMode:AVCaptureFocusModeLocked];
        if ([backCamera isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure])
            [backCamera setExposureMode:AVCaptureExposureModeContinuousAutoExposure];
        if ([backCamera isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance])
            [backCamera setWhiteBalanceMode:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance];
        if ([backCamera isFlashModeSupported:AVCaptureFlashModeOff])
            [backCamera setFlashMode:AVCaptureFlashModeOff];
        if ([backCamera isTorchModeSupported:AVCaptureTorchModeOff])
            [backCamera setTorchMode:AVCaptureTorchModeOff];
        [backCamera unlockForConfiguration];
    }
    else
        NSLog(@"WARNING: Failed to configure camera (line:%d)", __LINE__);

	deviceInput = [[[AVCaptureDeviceInput alloc] initWithDevice:backCamera error:&err] autorelease];
    if (!deviceInput) {

        NSLog(@"ERROR: %@ (line:%d)", err, __LINE__);
        return FALSE;
    }
	captureSession = [[AVCaptureSession alloc] init];
	if ([captureSession canAddInput:deviceInput])
		[captureSession addInput:deviceInput];
	else {

        NSLog(@"ERROR: Failed to add input device (line:%d)", __LINE__);
        return FALSE;
    }
	videoOutput = [[AVCaptureVideoDataOutput alloc] init];
    NSArray* formatArray = [videoOutput availableVideoCVPixelFormatTypes];
    BOOL bgraFormat = NO;
    for (NSNumber* format in formatArray)
        if ([format integerValue] == kCVPixelFormatType_32BGRA)
            bgraFormat = YES;

    if (bgraFormat == NO) {

        NSLog(@"ERROR: 'kCVPixelFormatType_32BGRA' video format not supported (line:%d)", __LINE__);
        return FALSE;
    }
	[videoOutput setAlwaysDiscardsLateVideoFrames:YES];
	[videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber
                        numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    dispatch_queue_t camQueue = dispatch_queue_create("camQueue", NULL);
    [videoOutput setSampleBufferDelegate:self queue:camQueue];
	if ([captureSession canAddOutput:videoOutput])
		[captureSession addOutput:videoOutput];
	else {

        NSLog(@"ERROR: Failed to add video output (line:%d)", __LINE__);
        return FALSE;
    }
    return TRUE;
}

-(bool)startCamera:(short)width andHeight:(short)height {

    if (self.camReady == FALSE) {
        NSLog(@"ERROR: Camera not ready (line:%d)", __LINE__);
        return false;
    }
    if ((width == 640) && (height == 480))
        [captureSession setSessionPreset:AVCaptureSessionPreset640x480];
    else if ((width == 1280) && (height == 720))
        [captureSession setSessionPreset:AVCaptureSessionPreset1280x720];
    else {
        NSLog(@"ERROR: Session preset format %dx%d not supported (line:%d)", width, height, __LINE__);
        return false;
    }
	if (![captureSession isRunning]) {
		[captureSession startRunning];
        return true;
    }
    return false;
}
-(bool)stopCamera {

    if (self.camReady == FALSE) {
        NSLog(@"ERROR: Camera not ready (line:%d)", __LINE__);
        return false;
    }

	if ([captureSession isRunning]) {
		[captureSession stopRunning];
        return true;
    }
    return false;
}
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
      fromConnection:(AVCaptureConnection *)connection {

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    CVImageBufferRef camBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(camBuffer, 0);
    platformLoadCamera(reinterpret_cast<const unsigned char*>((uint8_t*)CVPixelBufferGetBaseAddress(camBuffer)));
    CVPixelBufferUnlockBaseAddress(camBuffer, 0);
    [pool release];
}
#endif

////// Mic
#ifdef LIBENG_ENABLE_MIC

#define MIC_DEFAULT_SPEAKER     1
#define MIC_BUFFER_DURATION     0.02f
#define MIC_SAMPLE_RATE         44100
#define MIC_NO_OUTPUT_DISPLAY   1 // Always != 0 (!= 'noErr')

AudioUnit* g_AudioUnit = NULL;

OSStatus micCallback(void* userData, AudioUnitRenderActionFlags* actionFlags,
                     const AudioTimeStamp* audioTimeStamp, UInt32 busNumber, UInt32 numFrames,
                     AudioBufferList* buffers) {

    OSStatus res = AudioUnitRender(*g_AudioUnit, actionFlags, audioTimeStamp, 1, numFrames, buffers);
    if (res != noErr)
        return res;

    platformLoadMic(static_cast<int>(numFrames), reinterpret_cast<const short*>(buffers->mBuffers->mData));
    return MIC_NO_OUTPUT_DISPLAY; // Do not return 0 value to avoid output displaying (mic only)
}

-(BOOL)initMic {

    if (AudioSessionInitialize(NULL, NULL, NULL, NULL) != kAudioSessionNoError) {
        NSLog(@"ERROR: Failed to initialize audio session (line:%d)", __LINE__);
        return FALSE;
    }
    if (AudioSessionSetActive(true) != kAudioSessionNoError) {
        NSLog(@"ERROR: Failed to set the active audio session (line:%d)", __LINE__);
        return FALSE;
    }

    UInt32 audioCategory = kAudioSessionCategory_PlayAndRecord;
    if (AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,
                                sizeof(UInt32), &audioCategory) != kAudioSessionNoError) {
        NSLog(@"ERROR: Failed to set audio category (line:%d)", __LINE__);
        return FALSE;
    }

    Float32 bufferSizeInSec = MIC_BUFFER_DURATION;
    if (AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration,
                                sizeof(Float32), &bufferSizeInSec) != kAudioSessionNoError) {
        NSLog(@"ERROR: Failed to set duration property (line:%d)", __LINE__);
        return FALSE;
    }
    UInt32 defaultSpeaker = MIC_DEFAULT_SPEAKER;
    if (AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryDefaultToSpeaker,
                                sizeof(UInt32), &defaultSpeaker) != kAudioSessionNoError)
        NSLog(@"WARNING: Failed to set default speaker property (line:%d)", __LINE__);

    // There are many properties you might want to provide callback functions for:
    // kAudioSessionProperty_AudioRouteChange
    // kAudioSessionProperty_OverrideCategoryEnableBluetoothInput
    // etc.

    AudioComponentDescription componentDesc;
    componentDesc.componentType = kAudioUnitType_Output;
    componentDesc.componentSubType = kAudioUnitSubType_RemoteIO;
    componentDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
    componentDesc.componentFlags = 0;
    componentDesc.componentFlagsMask = 0;

    AudioComponent audioComp = AudioComponentFindNext(NULL, &componentDesc);
    g_AudioUnit = (AudioUnit*)malloc(sizeof(AudioUnit));
    if (AudioComponentInstanceNew(audioComp, g_AudioUnit) != noErr) {

        NSLog(@"ERROR: Failed to set duration property (line:%d)", __LINE__);
        free(g_AudioUnit);
        return FALSE;
    }
    UInt32 enableIO = 1;
    if (AudioUnitSetProperty(*g_AudioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1,
                             &enableIO, sizeof(UInt32)) != noErr) {
        NSLog(@"ERROR: Failed to enable IO property (line:%d)", __LINE__);
        free(g_AudioUnit);
        return FALSE;
    }

    AURenderCallbackStruct callbackStruct;
    callbackStruct.inputProc = micCallback; // micCallback
    callbackStruct.inputProcRefCon = NULL;
    if (AudioUnitSetProperty(*g_AudioUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0,
                             &callbackStruct, sizeof(AURenderCallbackStruct)) != noErr) {
        NSLog(@"ERROR: Failed to set render callback (line:%d)", __LINE__);
        free(g_AudioUnit);
        return FALSE;
    }

    // You might want to replace this with a different value, but keep in mind that the
    // iPhone does not support all sample rates. 8kHz, 22kHz, and 44.1kHz should all work.
    AudioStreamBasicDescription streamDesc;
    streamDesc.mSampleRate = MIC_SAMPLE_RATE;

    // Yes, I know you probably want floating point samples, but the iPhone isn't going
    // to give you floating point data. You'll need to make the conversion by hand from
    // linear PCM <-> float.
    streamDesc.mFormatID = kAudioFormatLinearPCM;

    // This part is important!
    streamDesc.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian |
                              kAudioFormatFlagIsPacked;

    // Not sure if the iPhone supports recording >16-bit audio, but I doubt it.
    streamDesc.mBitsPerChannel = 16;

    // 1 sample per frame, will always be 2 as long as 16-bit samples are being used
    streamDesc.mBytesPerFrame = 2;

    // Record in mono. Use 2 for stereo, though I don't think the iPhone does true stereo recording
    streamDesc.mChannelsPerFrame = 1;

    streamDesc.mBytesPerPacket = streamDesc.mBytesPerFrame * streamDesc.mChannelsPerFrame;
    streamDesc.mFramesPerPacket = 1; // Always should be set to 1
    streamDesc.mReserved = 0; // Always set to 0, just to be sure

    // Set up input/output stream with above properties
    if (AudioUnitSetProperty(*g_AudioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0,
                             &streamDesc, sizeof(streamDesc)) != noErr) {
        NSLog(@"ERROR: Failed to set input stream properties (line:%d)", __LINE__);
        free(g_AudioUnit);
        return FALSE;
    }
    if (AudioUnitSetProperty(*g_AudioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1,
                             &streamDesc, sizeof(streamDesc)) != noErr) {
        NSLog(@"ERROR: Failed to set output stream properties (line:%d)", __LINE__);
        free(g_AudioUnit);
        return FALSE;
    }
    return TRUE;
}
-(bool)startMic {

    if (self.micReady == FALSE)
        return false;

    if (AudioUnitInitialize(*g_AudioUnit) != noErr)
        return false;
    if (AudioOutputUnitStart(*g_AudioUnit) != noErr)
        return false;
    return true;
}
-(void)stopMic {

    if (self.micReady == FALSE)
        return;

    AudioUnitUninitialize(*g_AudioUnit);
    AudioOutputUnitStop(*g_AudioUnit);
}

-(void)initMicRecorder:(NSString*)file withFormat:(int)format withSampleRate:(float)rate
       withNumChannels:(int)channels {

    if (self.micRecURL != nil)
        [self.micRecURL release];
    self.micRecURL = [[NSURL alloc] initFileURLWithPath:file];
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{

        NSDictionary* recConfig = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithInt: format], AVFormatIDKey,
                                   [NSNumber numberWithFloat: rate], AVSampleRateKey,
                                   [NSNumber numberWithInt: channels], AVNumberOfChannelsKey,
                                   [NSNumber numberWithInt:16], AVEncoderBitDepthHintKey,
                                   [NSNumber numberWithInt:16], AVEncoderBitRatePerChannelKey,
                                   nil];
        NSError* err = nil;
        self.micRecorder = [[AVAudioRecorder alloc] initWithURL:self.micRecURL settings:recConfig error:&err];
        if ((self.micRecorder == nil) || (![self.micRecorder prepareToRecord])) {

            if (err)
                NSLog(@"ERROR: Failed to initialize mic recorder - %@ (line:%d)", err, __LINE__);
            else
                NSLog(@"ERROR: Failed to initialize mic recorder (line:%d)", __LINE__);
            [self.micRecorder release];
            self.micRecorder = nil;
        }
    }];
}
-(BOOL)startMicRecorder {

    if (self.micRecorder == nil) {
        NSLog(@"ERROR: Failed to start mic recorder coz not initialized (line:%d)", __LINE__);
        return FALSE;
    }
    self.micRecorder.delegate = self;
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{

        AVAudioSession* recSession = [AVAudioSession sharedInstance];
        [recSession setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
        [recSession setActive:YES error:nil];

        if (![self.micRecorder record]) {

            NSLog(@"ERROR: Failed to start mic recorder (line:%d)", __LINE__);
            [self.micRecorder release];
            self.micRecorder = nil;
        }
    }];
    return TRUE;
}
-(BOOL)stopMicRecorder {

    if (self.micRecorder == nil) {
        NSLog(@"WARNING: Failed to stop mic recorder coz not started (line:%d)", __LINE__);
        return FALSE;
    }
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{

        [self.micRecorder stop];

        AVAudioSession* recSession = [AVAudioSession sharedInstance];
        [recSession setActive:NO error:nil];
        [self.micRecorder release];
        self.micRecorder = nil;
    }];
    return TRUE;
}

-(void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag { }
-(void)audioRecorderEncodeErrorDidOccur:(AVAudioRecorder *)recorder error:(NSError *)error {
    NSLog(@"ERROR: Mic recording error - %@ (line:%d)", error, __LINE__);
}
#endif

////// Internet
#ifdef LIBENG_ENABLE_INTERNET
-(unsigned char)isConnected { return [EngController isConnected]; }
-(bool)isOnline { return [EngController isOnline]; }
#endif

////// Advertising
#ifdef LIBENG_ENABLE_ADVERTISING
-(void)loadAd {

    ((OpenGLView*)controller.view).engAd.status = static_cast<unsigned char>(Advertising::STATUS_LOADING);
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{

        GADRequest* request = [GADRequest request];
#ifdef LIBENG_ENABLE_SOCIAL
        switch (userGender) {
            case GENDER_MALE: [request setGender:kGADGenderMale]; break;
            case GENDER_FEMALE: [request setGender:kGADGenderFemale]; break;
        }
        if (userBirthday != nil) {

            NSDateComponents* dateInfo = [[NSCalendar currentCalendar]
                                          components:NSCalendarUnitDay|NSCalendarUnitMonth|NSCalendarUnitYear
                                          fromDate:userBirthday];
            [request setBirthdayWithMonth:[dateInfo month] day:[dateInfo day] year:[dateInfo year]];
        }
        if (userLocation != nil)
            [request setLocationWithDescription:userLocation];
#endif
        engLoadAd(((OpenGLView*)controller.view).engAd, request);
    }];
}
-(void)displayAd:(unsigned char)Id {

    ((OpenGLView*)controller.view).engAd.status = static_cast<unsigned char>(Advertising::STATUS_DISPLAYING);
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        engDisplayAd(((OpenGLView*)controller.view).engAd, Id);
    }];
}
-(void)hideAd:(unsigned char)Id {

    ((OpenGLView*)controller.view).engAd.status = static_cast<unsigned char>(Advertising::STATUS_LOADED);
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        engHideAd(((OpenGLView*)controller.view).engAd, Id);
    }];
}
-(unsigned char)getAdStatus { return ((OpenGLView*)controller.view).engAd.status; }
#endif

////// Social
#ifdef LIBENG_ENABLE_SOCIAL
-(id<SocialOS>)create:(unsigned char)socialID byDisplayingError:(bool)display {

    switch (socialID) {
        case Network::FACEBOOK: return [[EngFacebook alloc] initWithDisplayError:display on:controller];
        case Network::TWITTER: return [[EngTwitter alloc] initWithDisplayError:display on:controller];
        case Network::GOOGLE: return [[EngGoogle alloc] initWithDisplayError:display on:controller];
    }
    return nil;
}
-(void)setAdReqInfo:(id<SocialOS>)fromSocial {

    if ([fromSocial userGender] != GENDER_NONE)
        userGender = [fromSocial userGender];

    if ([fromSocial userBirthday] != nil) {
        if (userBirthday != nil)
            [userBirthday release];

        userBirthday = [[NSDate alloc] initWithTimeInterval:0 sinceDate:[fromSocial userBirthday]];
    }
    if ([fromSocial userLocation] != nil) {
        if (userLocation == nil)
            userLocation = [[NSMutableString alloc] initWithString:[fromSocial userLocation]];
        else
            [userLocation setString:[fromSocial userLocation]];
    }
}
#endif

////// alertMessage
-(void)alertMessage:(const char*)msg withDuration:(double)duration {
    [controller alert:[NSString stringWithUTF8String:msg] during:duration];
}

//////
-(void)dealloc {

#ifdef LIBENG_ENABLE_CAMERA
    if (captureSession) {

        [captureSession stopRunning];
        [captureSession release];
    }
	if (videoOutput) [videoOutput release];
	if (deviceInput) [deviceInput release];
#endif

#ifdef LIBENG_ENABLE_MIC
    if (self.micReady == TRUE) {

        assert(g_AudioUnit);
        *g_AudioUnit = NULL;
    }
    if (self.micRecorder != nil)
        [self.micRecorder release];
    if (self.micRecURL != nil)
        [self.micRecURL release];
#endif

#ifdef LIBENG_ENABLE_SOCIAL
    if (userBirthday != nil)
        [userBirthday release];
    if (userLocation != nil)
        [userLocation release];
#endif
    [accelTimer release];
    [motion release];

    [timeElapsed release];
    [resources release];
    [context release];
    [controller.view release];
    [controller release];

    [_window release];
    [super dealloc];
}

@end
