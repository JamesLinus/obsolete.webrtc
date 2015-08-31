﻿
// Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#ifndef WEBRTC_BUILD_WINRT_GYP_API_MEDIA_H_
#define WEBRTC_BUILD_WINRT_GYP_API_MEDIA_H_

#include <mfidl.h>
#include <collection.h>
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/mediaconstraintsinterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "GlobalObserver.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/media/devices/winrtdevicemanager.h"
#include "webrtc/modules/audio_device/include/audio_device.h"
#include "Delegates.h"

using Windows::Foundation::IAsyncOperation;
using Platform::String;
using Windows::Foundation::Collections::IVector;
using Windows::Media::Core::IMediaSource;

namespace webrtc_winrt_api {
  public interface class IMediaStreamTrack {
    property String^ Kind { String^ get(); }
    property String^ Id { String^ get(); }
    property bool Enabled { bool get(); void set(bool value); }
    void Stop();
  };

  public ref class MediaVideoTrack sealed : public IMediaStreamTrack {
  internal:
    MediaVideoTrack(rtc::scoped_refptr<webrtc::VideoTrackInterface> impl);
    rtc::scoped_refptr<webrtc::VideoTrackInterface> GetImpl() {
      return _impl;
    }
  public:
    virtual ~MediaVideoTrack();
    virtual property String^ Kind { String^ get(); }
    virtual property String^ Id { String^ get(); }
    virtual property bool Enabled { bool get(); void set(bool value); }
    virtual void Stop();
  internal:
    void SetRenderer(webrtc::VideoRendererInterface* renderer);
    void UnsetRenderer(webrtc::VideoRendererInterface* renderer);
  private:
    rtc::scoped_refptr<webrtc::VideoTrackInterface> _impl;
  };

  public ref class MediaAudioTrack sealed : public IMediaStreamTrack {
  internal:
    MediaAudioTrack(rtc::scoped_refptr<webrtc::AudioTrackInterface> impl);
    rtc::scoped_refptr<webrtc::AudioTrackInterface> GetImpl() {
      return _impl;
    }
  public:
    virtual property String^ Kind { String^ get(); }
    virtual property String^ Id { String^ get(); }
    virtual property bool Enabled { bool get(); void set(bool value); }
    virtual void Stop();
  private:
    rtc::scoped_refptr<webrtc::AudioTrackInterface> _impl;
  };

  public ref class MediaStream sealed {
  internal:
    MediaStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> impl);
    rtc::scoped_refptr<webrtc::MediaStreamInterface> GetImpl();
  public:
    IVector<MediaAudioTrack^>^ GetAudioTracks();
    IVector<MediaVideoTrack^>^ GetVideoTracks();
    IVector<IMediaStreamTrack^>^ GetTracks();
    IMediaStreamTrack^ GetTrackById(String^ trackId);
    void AddTrack(IMediaStreamTrack^ track);
    void RemoveTrack(IMediaStreamTrack^ track);
    property String^ Id { String^ get();}
    void Stop();
    property bool Active { bool get(); }
  private:
    ~MediaStream();
    rtc::scoped_refptr<webrtc::MediaStreamInterface> _impl;
  };

  public ref class CaptureCapability sealed {
  public:
    CaptureCapability(unsigned int width, unsigned int height,
      unsigned int fps,
      Windows::Media::MediaProperties::MediaRatio^ pixelAspect) {
      _width = width;
      _height = height;
      _fps = fps;
      _pixelAspectRatio = pixelAspect;
      wchar_t resolutionDesc[64];
      swprintf_s(resolutionDesc, 64, L"%u x %u (pixel %u:%u)",
        _width, _height, pixelAspect->Numerator, pixelAspect->Denominator);
      _resolutionDescription = ref new String(resolutionDesc);
      wchar_t fpsDesc[64];
      swprintf_s(fpsDesc, 64, L"%u fps", _fps);
      _fpsDescription = ref new String(fpsDesc);
      wchar_t desc[128];
      swprintf_s(desc, 128, L"%s %s", resolutionDesc, fpsDesc);
      _description = ref new String(desc);
    }
    property unsigned int Width {
      unsigned int get() {
        return _width;
      }
    }
    property unsigned int Height {
      unsigned int get() {
        return _height;
      }
    }
    property unsigned int FrameRate {
      unsigned int get() {
        return _fps;
      }
    }
    property Windows::Media::MediaProperties::MediaRatio^ PixelAspectRatio {
      Windows::Media::MediaProperties::MediaRatio^ get() {
        return _pixelAspectRatio;
      }
    }
    property String^ FullDescription {
      String^ get() {
        return _description;
      }
    }
    property String^ ResolutionDescription {
      String^ get() {
        return _resolutionDescription;
      }
    }
    property String^ FrameRateDescription {
      String^ get() {
        return _fpsDescription;
      }
    }
  private:
    unsigned int _width;
    unsigned int _height;
    unsigned int _fps;
    Windows::Media::MediaProperties::MediaRatio^ _pixelAspectRatio;
    String^ _resolutionDescription;
    String^ _fpsDescription;
    String^ _description;
  };

  public ref class MediaDevice sealed {
  private:
    String^ _id;
    String^ _name;
  public:
    MediaDevice(String^ id, String^ name) {
      _id = id;
      _name = name;
    }
    property String^ Id {
      String^ get() {
        return _id;
      }
      void set(String^ value) {
        _id = value;
      }
    }

    property String^ Name {
      String^ get() {
        return _name;
      }
      void set(String^ value) {
        _name = value;
      }
    }

    IAsyncOperation<IVector<CaptureCapability^>^>^
      GetVideoCaptureCapabilities();
  };

  public ref class RTCMediaStreamConstraints sealed {
  public:
    property bool audioEnabled;
    property bool videoEnabled;
  };


  public ref class Media sealed {
  public:
    Media();

    // TODO(WINRT): Arguments
    IAsyncOperation<MediaStream^>^ GetUserMedia(
      RTCMediaStreamConstraints^ mediaStreamConstraints);
    IMediaSource^ CreateMediaStreamSource(
      MediaVideoTrack^ track, uint32 framerate, String^ id);
    IVector<MediaDevice^>^ GetAudioCaptureDevices();

    IVector<MediaDevice^>^ GetVideoCaptureDevices();

    IAsyncOperation<bool>^ EnumerateAudioVideoCaptureDevices();
    void SelectVideoDevice(MediaDevice^ device);
    void SelectAudioDevice(MediaDevice^ device);

    event OnMediaCaptureDeviceFoundDelegate^ OnVideoCaptureDeviceFound;
    event OnMediaCaptureDeviceFoundDelegate^ OnAudioCaptureDeviceFound;

  private:
    rtc::scoped_ptr<cricket::DeviceManagerInterface> _dev_manager;
    cricket::Device _selectedVideoDevice;
    webrtc::AudioDeviceModule *_audioDevice;
    uint16_t _selectedAudioDevice;
  };

}  // namespace webrtc_winrt_api

#endif  // WEBRTC_BUILD_WINRT_GYP_API_MEDIA_H_
