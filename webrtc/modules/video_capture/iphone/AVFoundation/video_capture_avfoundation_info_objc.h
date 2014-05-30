/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

//
//  video_capture_avfoundation_info_objc.h
//
//

#ifndef WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IPHONE_AVFOUNDATION_VIDEO_CAPTURE_AVFOUNDATION_INFO_OBJC_H_
#define WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IPHONE_AVFOUNDATION_VIDEO_CAPTURE_AVFOUNDATION_INFO_OBJC_H_

#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "video_capture_avfoundation_utility.h"
#include "video_capture_avfoundation_info.h"

@interface VideoCaptureIPhoneAVFoundationInfoObjC : NSObject{
    bool                                _OSSupportedInfo;
    NSArray*                            _captureDevicesInfo;
    NSAutoreleasePool*                    _poolInfo;
    int                                    _captureDeviceCountInfo;
    int                                    _frontCameraIndex;
    int                                    _backCameraIndex;

}

/**************************************************************************
 *
 *   The following functions are considered to be private
 *
 ***************************************************************************/

- (NSNumber*)getCaptureDevices;
- (NSNumber*)initializeVariables;
- (void)checkOSSupported;


/**************************************************************************
 *
 *   The following functions are considered to be public and called by VideoCaptureMacQTKitInfo class
 *
 ***************************************************************************/

- (NSNumber*)getCaptureDeviceCount;

- (NSNumber*)getDeviceNamesFromIndex:(uint32_t)index
    DefaultName:(char*)deviceName
    WithLength:(uint32_t)deviceNameLength
    AndUniqueID:(char*)deviceUniqueID
    WithLength:(uint32_t)deviceUniqueIDLength
    AndProductID:(char*)deviceProductID
    WithLength:(uint32_t)deviceProductIDLength;

- (NSNumber*)displayCaptureSettingsDialogBoxWithDevice:
        (const char*)deviceUniqueIdUTF8
    AndTitle:(const char*)dialogTitleUTF8
    AndParentWindow:(void*) parentWindow AtX:(uint32_t)positionX
    AndY:(uint32_t) positionY;

- (UIDeviceOrientation)getDeviceOrientation;

- (NSNumber*)getCaptureDeviceIndex:(const char*)deviceUniqueIdUTF8;
- (NSNumber*)getFrontCameraIndex;
- (NSNumber*)getBackCameraIndex;

@end

#endif  // WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_IPHONE_AVFOUNDATION_VIDEO_CAPTURE_AVFOUNDATION_INFO_OBJC_H_
