﻿
// Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

// Class1.cpp
#include "webrtc/build/WinRT_gyp/Api/GlobalObserver.h"
#include <ppltasks.h>
#include "PeerConnectionInterface.h"
#include "Marshalling.h"
#include "Media.h"
#include "DataChannel.h"

using Platform::Collections::Vector;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

namespace webrtc_winrt_api_internal {

#define POST_PC_EVENT(fn, evt) \
  if (g_windowDispatcher != nullptr) {\
    g_windowDispatcher->RunAsync(\
      Windows::UI::Core::CoreDispatcherPriority::Normal, \
      ref new Windows::UI::Core::DispatchedHandler([this, evt] {\
      webrtc::CriticalSectionScoped csLock(_lock.get()); \
      if (_pc != nullptr) {\
        _pc->##fn(evt);\
      }\
    }));\
  } else {\
    webrtc::CriticalSectionScoped csLock(_lock.get()); \
    if (_pc != nullptr) {\
      _pc->##fn(evt);\
    }\
  }

#define POST_PC_ACTION(fn) \
  if (g_windowDispatcher != nullptr) {\
    g_windowDispatcher->RunAsync(\
      Windows::UI::Core::CoreDispatcherPriority::Normal, \
      ref new Windows::UI::Core::DispatchedHandler([this] {\
      webrtc::CriticalSectionScoped csLock(_lock.get()); \
      if (_pc != nullptr) {\
        _pc->##fn();\
      }\
    }));\
  } else {\
    webrtc::CriticalSectionScoped csLock(_lock.get()); \
    if (_pc != nullptr) {\
      _pc->##fn();\
    }\
  }

GlobalObserver::GlobalObserver()
  : _lock(webrtc::CriticalSectionWrapper::CreateCriticalSection()) {
}

void GlobalObserver::SetPeerConnection(
  webrtc_winrt_api::RTCPeerConnection^ pc) {
  webrtc::CriticalSectionScoped csLock(_lock.get()); \
  _pc = pc;
}

// Triggered when the SignalingState changed.
void GlobalObserver::OnSignalingChange(
  webrtc::PeerConnectionInterface::SignalingState new_state) {
  POST_PC_ACTION(OnSignalingStateChange);
}

// Triggered when SignalingState or IceState have changed.
// TODO(bemasc): Remove once callers transition to OnSignalingChange.
void GlobalObserver::OnStateChange(StateType state_changed) {
}

// Triggered when media is received on a new stream from remote peer.
void GlobalObserver::OnAddStream(webrtc::MediaStreamInterface* stream) {
  auto evt = ref new webrtc_winrt_api::MediaStreamEvent();
  evt->Stream = ref new webrtc_winrt_api::MediaStream(stream);
  POST_PC_EVENT(OnAddStream, evt);
}

// Triggered when a remote peer close a stream.
void GlobalObserver::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
  auto evt = ref new webrtc_winrt_api::MediaStreamEvent();
  evt->Stream = ref new webrtc_winrt_api::MediaStream(stream);
  POST_PC_EVENT(OnRemoveStream, evt);
}

// Triggered when a remote peer open a data channel.
void GlobalObserver::OnDataChannel(webrtc::DataChannelInterface* data_channel) {
  auto evt = ref new webrtc_winrt_api::RTCDataChannelEvent();
  evt->Channel = ref new webrtc_winrt_api::RTCDataChannel(data_channel);
  // TODO(WINRT): Figure out when this observer can be deleted.
  data_channel->RegisterObserver(new DataChannelObserver(evt->Channel));
  POST_PC_EVENT(OnDataChannel, evt);
}

// Triggered when renegotiation is needed, for example the ICE has restarted.
void GlobalObserver::OnRenegotiationNeeded() {
  POST_PC_ACTION(OnNegotiationNeeded);
}

// Called any time the IceConnectionState changes
void GlobalObserver::OnIceConnectionChange(
  webrtc::PeerConnectionInterface::IceConnectionState new_state) {
  if (new_state == webrtc::PeerConnectionInterface::kIceConnectionConnected) {
    if (!_stats_observer_etw.get()) {
      _stats_observer_etw =
        new rtc::RefCountedObject<webrtc::StatsObserverETW>();
    }

    _stats_observer_etw->PollStats(_pc->_impl);
  }
}

// Called any time the IceGatheringState changes
void GlobalObserver::OnIceGatheringChange(
  webrtc::PeerConnectionInterface::IceGatheringState new_state) {
  LOG(LS_INFO) << "OnIceGatheringChange";
}

// New Ice candidate have been found.
void GlobalObserver::OnIceCandidate(
  const webrtc::IceCandidateInterface* candidate) {
  std::string c;
  candidate->ToString(&c);
  LOG(LS_INFO) << "Ice candidate = " << c;
  auto evt = ref new webrtc_winrt_api::RTCPeerConnectionIceEvent();
  webrtc_winrt_api::RTCIceCandidate^ cxCandidate;
  if (candidate == nullptr) {
    evt->Candidate = nullptr;
  } else {
    ToCx(*candidate, &cxCandidate);
    evt->Candidate = cxCandidate;
  }
  POST_PC_EVENT(OnIceCandidate, evt);
}

// TODO(bemasc): Remove this once callers transition to OnIceGatheringChange.
// All Ice candidates have been found.
void GlobalObserver::OnIceComplete() {
  auto evt = ref new webrtc_winrt_api::RTCPeerConnectionIceEvent();
  evt->Candidate = nullptr;
  POST_PC_EVENT(OnIceCandidate, evt);
}

//============================================================================

CreateSdpObserver::CreateSdpObserver(
  Concurrency::task_completion_event<webrtc::SessionDescriptionInterface*> tce)
  : _tce(tce) {
}

void CreateSdpObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
  _tce.set(desc);
}

void CreateSdpObserver::OnFailure(const std::string& error) {
  _tce.set_exception(error);
}

//============================================================================

SetSdpObserver::SetSdpObserver(Concurrency::task_completion_event<void> tce)
  : _tce(tce) {
}

void SetSdpObserver::OnSuccess() {
  _tce.set();
}

void SetSdpObserver::OnFailure(const std::string& error) {
  _tce.set_exception(error);
}

//============================================================================

DataChannelObserver::DataChannelObserver(
  webrtc_winrt_api::RTCDataChannel^ channel)
  : _channel(channel) {
}

void DataChannelObserver::OnStateChange() {
  switch (_channel->GetImpl()->state()) {
  case webrtc::DataChannelInterface::kOpen:
    _channel->OnOpen();
    break;
  case webrtc::DataChannelInterface::kClosed:
    _channel->OnClose();
    break;
  }
}

void DataChannelObserver::OnMessage(const webrtc::DataBuffer& buffer) {
  auto evt = ref new webrtc_winrt_api::RTCDataChannelMessageEvent();

  if (!buffer.binary) {
    // convert buffer data from uint_8[] to char*
    String^ receivedString = ToCx(std::string(reinterpret_cast<const char*>(buffer.data.data()), buffer.size()));

    evt->Data = ref new webrtc_winrt_api::StringDataChannelMessage(receivedString);
  }
  else {
    // convert byte[] from buffer to Vector
    std::vector<byte> bytesFromBuffer = std::vector<byte>();
    bytesFromBuffer.insert(bytesFromBuffer.end(), buffer.data.data(), buffer.data.data() + buffer.size());
    Vector<byte>^ convertedBytes = ref new Vector<byte>();
    ToCx(&bytesFromBuffer, convertedBytes);

    evt->Data = ref new webrtc_winrt_api::BinaryDataChannelMessage(convertedBytes);
  }

  if (g_windowDispatcher != nullptr) {
    g_windowDispatcher->RunAsync(
      Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([this, evt] {
      _channel->OnMessage(evt);
    }));
  }
  else {
    _channel->OnMessage(evt);
  }
}

}  // namespace webrtc_winrt_api_internal

