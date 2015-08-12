﻿
// Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.


#ifndef WEBRTC_BUILD_WINRT_GYP_API_PEERCONNECTIONINTERFACE_H_
#define WEBRTC_BUILD_WINRT_GYP_API_PEERCONNECTIONINTERFACE_H_

#include <collection.h>
#include <vector>
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "webrtc/base/scopedptrcollection.h"
#include "webrtc/base/logging.h"
#include "GlobalObserver.h"
#include "DataChannel.h"

using Platform::String;
using Platform::IBox;
using Windows::Foundation::Collections::IVector;
using Windows::Foundation::IAsyncOperation;
using Windows::Foundation::IAsyncAction;
using webrtc_winrt_api_internal::CreateSdpObserver;
using webrtc_winrt_api_internal::SetSdpObserver;
using webrtc_winrt_api_internal::DataChannelObserver;
using webrtc_winrt_api_internal::GlobalObserver;

namespace webrtc_winrt_api {

ref class MediaStream;
ref class MediaStreamTrack;

public enum class LogLevel
  { 
  LOGLVL_SENSITIVE = rtc::LS_SENSITIVE,
  LOGLVL_VERBOSE = rtc::LS_VERBOSE,
  LOGLVL_INFO = rtc::LS_INFO,
  LOGLVL_WARNING = rtc::LS_WARNING,
  LOGLVL_ERROR = rtc::LS_ERROR
};

public ref class CodecInfo sealed {
private:
    int _id;
    int _clockrate;
    int _channels;
    String^ _name;
public:
    CodecInfo(int id, int clockrate, String^ name) {
        _id = id;
        _clockrate = clockrate;
        _name = name;
    }

    property int Id {
        int get(){
            return _id;
        }
        void set(int value) {
            _id = value;
        }
    }

    property int Clockrate {
        int get(){
            return _clockrate;
        }
        void set(int value) {
            _clockrate = value;
        }
    }

    property String^ Name {
        String^ get(){
            return _name;
        }
        void set(String^ value) {
            _name = value;
        }
    }
};

public ref class WinJSHooks sealed {
public:
  static void initialize();
  static IAsyncOperation<bool>^ requestAccessForMediaCapture();
};

[Windows::Foundation::Metadata::WebHostHidden]
public ref class WebRTC sealed {
public:
  static IAsyncOperation<bool>^ RequestAccessForMediaCapture();
  static void Initialize(Windows::UI::Core::CoreDispatcher^ dispatcher);

  static bool IsTracing();
  static void StartTracing();
  static void StopTracing();
  static bool SaveTrace(Platform::String^ filename);
  static bool SaveTrace(Platform::String^ host, int port);

  static void EnableLogging(LogLevel level);
  static void DisableLogging();

  static IVector<CodecInfo^>^ GetAudioCodecs();
  static IVector<CodecInfo^>^ GetVideoCodecs();
  static void SetPreferredVideoCaptureFormat(int frame_width, int frame_height, int fps);

private:
  // This type is not meant to be created.
  WebRTC();

  static const unsigned char* GetCategoryGroupEnabled(const char* category_group);
  static void __cdecl AddTraceEvent(char phase,
    const unsigned char* category_group_enabled,
    const char* name,
    unsigned long long id,
    int num_args,
    const char** arg_names,
    const unsigned char* arg_types,
    const unsigned long long* arg_values,
    unsigned char flags);
};

public enum class RTCBundlePolicy {
  Balanced,
  MaxBundle,
  MaxCompat,
};

public enum class RTCIceTransportPolicy {
  None,
  Relay,
  NoHost,
  All
};

public enum class RTCIceGatheringState {
  New,
  Gathering,
  Complete
};

public enum class RTCIceConnectionState {
  New,
  Checking,
  Connected,
  Completed,
  Failed,
  Disconnected,
  Closed
};

public enum class RTCSdpType {
  Offer,
  Pranswer,
  Answer,
};

public enum class RTCSignalingState {
  Stable,
  HaveLocalOffer,
  HaveRemoteOffer,
  HaveLocalPranswer,
  HaveRemotePranswer,
  Closed
};

public ref class RTCIceServer sealed {
public:
  property String^ Url;
  property String^ Username;
  property String^ Credential;
};

public ref class RTCConfiguration sealed {
public:
  property IVector<RTCIceServer^>^ IceServers;
  property IBox<RTCIceTransportPolicy>^ IceTransportPolicy;
  property IBox<RTCBundlePolicy>^ BundlePolicy;
  // TODO(WINRT): DOMString PeerIdentity
};

public ref class RTCIceCandidate sealed {
public:
  RTCIceCandidate();
  RTCIceCandidate(String^ candidate, String^ sdpMid,
    uint16 sdpMLineIndex);
  property String^ Candidate;
  property String^ SdpMid;
  property uint16 SdpMLineIndex;
};

public ref class RTCSessionDescription sealed {
public:
  RTCSessionDescription();
  RTCSessionDescription(RTCSdpType type, String^ sdp);
  property IBox<RTCSdpType>^ Type;
  property String^ Sdp;
};

// Events and delegates
// ------------------
public ref class RTCPeerConnectionIceEvent sealed {
public:
  property RTCIceCandidate^ Candidate;
};

// ------------------
public ref class MediaStreamEvent sealed {
public:
  property MediaStream^ Stream;
};

public ref class RTCPeerConnection sealed {
public:
  // Required so the observer can raise events in this class.
  // By default event raising is protected.
  friend class GlobalObserver;

  RTCPeerConnection(RTCConfiguration^ configuration);

  event RTCPeerConnectionIceEventDelegate^ OnIceCandidate;
  event MediaStreamEventEventDelegate^ OnAddStream;
  event MediaStreamEventEventDelegate^ OnRemoveStream;
  event EventDelegate^ OnNegotiationNeeded;
  event EventDelegate^ OnSignalingStateChange;
  event RTCDataChannelEventDelegate^ OnDataChannel;

  IAsyncOperation<RTCSessionDescription^>^ CreateOffer();
  IAsyncOperation<RTCSessionDescription^>^ CreateAnswer();
  IAsyncAction^ SetLocalDescription(RTCSessionDescription^ description);
  IAsyncAction^ SetRemoteDescription(RTCSessionDescription^ description);
  RTCConfiguration^ GetConfiguration();
  IVector<MediaStream^>^ GetLocalStreams();
  IVector<MediaStream^>^ GetRemoteStreams();
  MediaStream^ GetStreamById(String^ streamId);
  void AddStream(MediaStream^ stream);
  void RemoveStream(MediaStream^ stream);
  RTCDataChannel^ CreateDataChannel(String^ label, RTCDataChannelInit^ init);
  IAsyncAction^ AddIceCandidate(RTCIceCandidate^ candidate);
  void Close();

  property RTCSessionDescription^ LocalDescription {
    RTCSessionDescription^ get();
  }
  property RTCSessionDescription^ RemoteDescription {
    RTCSessionDescription^ get();
  }
  property RTCSignalingState SignalingState {
    RTCSignalingState get();
  }
  property RTCIceGatheringState IceGatheringState {
    RTCIceGatheringState get();
  }
  property RTCIceConnectionState IceConnectionState {
    RTCIceConnectionState get();
  }

private:
  ~RTCPeerConnection();
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> _impl;
  GlobalObserver _observer;

  typedef std::vector<rtc::scoped_refptr<CreateSdpObserver>> CreateSdpObservers;
  typedef std::vector<rtc::scoped_refptr<SetSdpObserver>> SetSdpObservers;
  typedef rtc::ScopedPtrCollection<DataChannelObserver> DataChannelObservers;
  CreateSdpObservers _createSdpObservers;
  SetSdpObservers _setSdpObservers;
  DataChannelObservers _dataChannelObservers;
};

namespace globals {
extern rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
  gPeerConnectionFactory;
// The worker thread for webrtc.
extern rtc::Thread gThread;

template <typename T>
T RunOnGlobalThread(std::function<T()> fn) {
  return gThread.Invoke<T, std::function<T()>>(fn);
}

}  // namespace globals

}  // namespace webrtc_winrt_api

#endif  // WEBRTC_BUILD_WINRT_GYP_API_PEERCONNECTIONINTERFACE_H_
