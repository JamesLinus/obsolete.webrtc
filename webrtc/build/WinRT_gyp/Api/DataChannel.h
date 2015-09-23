﻿/*
* Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
*
* Use of this source code is governed by a BSD-style license
* that can be found in the LICENSE file in the root of the source
* tree. An additional intellectual property rights grant can be found
* in the file PATENTS.  All contributing project authors may
* be found in the AUTHORS file in the root of the source tree.
*/

#ifndef WEBRTC_BUILD_WINRT_GYP_API_DATACHANNEL_H_
#define WEBRTC_BUILD_WINRT_GYP_API_DATACHANNEL_H_

#include <collection.h>
#include "GlobalObserver.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "Delegates.h"

using Platform::String;
using Platform::IBox;
using Windows::Foundation::Collections::IVector;

namespace webrtc_winrt_api {

/// <summary>
/// Used to keep track of a <see cref="RTCDataChannel"/>'s state.
/// </summary>
public enum class RTCDataChannelState {
  Connecting,
  Open,
  Closing,
  Closed,
};

/// <summary>
/// Can be used to configure properties of the underlying channel such as data reliability.
/// </summary>
public ref class RTCDataChannelInit sealed {
public:
  property IBox<bool>^ Ordered;
  property IBox<uint16>^ MaxPacketLifeTime;
  property IBox<uint16>^ MaxRetransmits;
  property String^ Protocol;
  property IBox<bool>^ Negotiated;
  property IBox<uint16>^ Id;
};

/// <summary>
/// Type of messages for a <see cref="RTCDataChannel"/>.
/// </summary>
public enum class RTCDataChannelMessageType {
  String,
  Binary
};

public interface class IDataChannelMessage {
  property RTCDataChannelMessageType DataType;
};

/// <summary>
/// Message type used for sending strings (chat messages, for example) over a data channel.
/// </summary>
public ref class StringDataChannelMessage sealed : IDataChannelMessage {
public:
  StringDataChannelMessage(String^ data);
  property String^ StringData;
  property RTCDataChannelMessageType DataType {
    virtual RTCDataChannelMessageType get() {
        return RTCDataChannelMessageType::String;
    }
    virtual void set(RTCDataChannelMessageType) { }
  };
};

/// <summary>
/// Message type used for sending binary data (a file, for example) over a data channel.
/// </summary>
public ref class BinaryDataChannelMessage sealed : IDataChannelMessage {
public:
  BinaryDataChannelMessage(IVector<byte>^ data);
  property IVector<byte>^ BinaryData;
  property RTCDataChannelMessageType DataType {
    virtual RTCDataChannelMessageType get() {
        return RTCDataChannelMessageType::Binary;
    }
    virtual void set(RTCDataChannelMessageType) { }
  };
};

public ref class RTCDataChannelMessageEvent sealed {
public:
  property IDataChannelMessage^ Data;
};

/// <summary>
/// Represents a bi-directional data channel between two peers.
/// </summary>
public ref class RTCDataChannel sealed {
internal:
  RTCDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> impl);
  rtc::scoped_refptr<webrtc::DataChannelInterface> GetImpl();
  friend class webrtc_winrt_api_internal::DataChannelObserver;

public:
  /// <summary>
  /// Can be used to distinguish this <see cref="RTCDataChannel"/> object from other <see cref="RTCDataChannel"/> 
  /// objects. Uniqueness is not guaranteed for labels.
  /// </summary>
  property String^ Label { String^ get(); }

  /// <summary>
  /// true if the<see cref="RTCDataChannel"/> is ordered, and false if other of order delivery is allowed.
  /// </summary>
  property bool Ordered { bool get(); }

  /// <summary>
  /// Length of the time window (in milliseconds) during which transmissions and retransmissions may occur in 
  /// unreliable mode, or nullptr if unset. 
  /// </summary>
  property IBox<uint16>^ MaxPacketLifeTime { IBox<uint16>^ get(); }

  /// <summary>
  /// Maximum number of retransmissions that are attempted in unreliable mode, or nullptr if unset.
  /// </summary>
  property IBox<uint16>^ MaxRetransmits { IBox<uint16>^ get(); }

  /// <summary>
  /// The name of the sub-protocol used with this RTCDataChannel if any.
  /// </summary>
  property String^ Protocol { String^ get(); }

  /// <summary>
  /// true if this RTCDataChannel was negotiated by the application, false otherwise.
  /// </summary>
  property bool Negotiated { bool get(); }

  /// <summary>
  /// Unique identifier for the data channel.
  /// </summary>
  property uint16 Id { uint16 get(); }

  /// <summary>
  /// The state of the RTCDataChannel object.
  /// </summary>
  property RTCDataChannelState ReadyState { RTCDataChannelState get(); }

  /// <summary>
  /// The number of bytes of application data (UTF-8 text and binary data) that have been queued but that, 
  /// as of the last time the event loop started executing a task, had not yet been transmitted to the network.
  /// </summary>
  property unsigned int BufferedAmount { unsigned int get(); }

  /// <summary>
  /// Event triggered when a message is successfully received.
  /// </summary>
  event RTCDataChannelMessageEventDelegate^ OnMessage;

  /// <summary>
  /// Event triggered when a data channel is opened.
  /// </summary>
  event EventDelegate^ OnOpen;

  /// <summary>
  /// Event triggered when a data channel is closed.
  /// </summary>
  event EventDelegate^ OnClose;

  // TODO(WINRT): Figure out how OnError is received.
  event EventDelegate^ OnError;

  void Close();

  /// <summary>
  /// Attempts to send data on channel's underlying data transport.
  /// </summary>
  /// <param name="data">Message to be sent.</param>
  void Send(IDataChannelMessage^ data);

private:
  rtc::scoped_refptr<webrtc::DataChannelInterface> _impl;
};

public ref class RTCDataChannelEvent sealed {
public:
  property RTCDataChannel^ Channel;
};
public delegate void RTCDataChannelEventDelegate(RTCDataChannelEvent^);

}  // namespace webrtc_winrt_api

#endif  // WEBRTC_BUILD_WINRT_GYP_API_DATACHANNEL_H_

