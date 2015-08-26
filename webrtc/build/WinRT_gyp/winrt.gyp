# Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

{
  'includes': ['../common.gypi',],
  'variables': {

  },
  'targets': [
    {
      'target_name': 'winrt_all',
      'type': 'none',
      'dependencies': [
        'UnitTests/LibTest_runner/libTest_runner.gyp:*',
        'UnitTests/gtest_runner/gtest_runner.gyp:*',
        'api/api.gyp:*',
        'etw/etw.gyp:*',
        '<(webrtc_root)/modules/video_capture/test/winrt/video_capture_test_winrt.gyp:*',
        '<(webrtc_root)/modules/audio_device/test/winrt/audio_device_test_winrt.gyp:*',
        '<(webrtc_root)/../talk/libjingle_examples.gyp:peerconnection_client',
        '<(webrtc_root)/../third_party/h264_winrt/h264_winrt.gyp:*',
      ],
    },
  ],
}
