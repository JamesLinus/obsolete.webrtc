# Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

{
  'includes': ['../../../../build/common.gypi',],
  'targets': [
    {
      'target_name': 'video_capture_test_winrt',
      'type': 'executable',
      'dependencies': [
        '<(webrtc_root)/modules/modules.gyp:video_capture_module_internal_impl',
      ],
      'defines': [
        '_HAS_EXCEPTIONS=1',
      ],
      'include_dirs': [
        '../../../../../..',
      ],
      'sources': [
        'App.cpp',
        'Package.appxmanifest',
        'video_capture_test_winrt_TemporaryKey.pfx',
        'Logo.png',
        'SmallLogo.png',
        'SplashScreen.png',
        'StoreLogo.png',
      ],
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)',
          'files':[
            'video_capture_test_winrt_mapping.ini',
          ],
        },
        {
          'destination': '<(PRODUCT_DIR)/video_capture_test_winrt_package',
          'files':[
            'Generated Manifest\AppxManifest.xml',
            'Logo.png',
            'SmallLogo.png',
            'SplashScreen.png',
            'StoreLogo.png',
          ],
        },
        # Hack for MSVS to copy to the Appx folder
        {
          'destination': '<(PRODUCT_DIR)/AppX',
          'files':[
            'Logo.png',
            'SmallLogo.png',
            'SplashScreen.png',
            'StoreLogo.png',
          ],
        },
      ],
      'msvs_package_certificate': {
        'KeyFile': 'video_capture_test_winrt_TemporaryKey.pfx',
        'Thumbprint': 'E3AA95A6CD6D9DF6D0B7C68EBA246B558824F8C1',
      },
      'msvs_settings': {
        'VCCLCompilerTool': {
          # ExceptionHandling must match _HAS_EXCEPTIONS above.
          'ExceptionHandling': '1',
        },
        'VCLinkerTool': {
          'UseLibraryDependencyInputs': "true",
          'AdditionalDependencies': [
           'mfplat.lib',
           'mfuuid.lib',
          ],
          # 2 == /SUBSYSTEM:WINDOWS
          'SubSystem': '2',
          'AdditionalOptions': [
          ],
        },
      },
    },
    {
      'target_name': 'video_capture_test_winrt_appx',
      #'product_name': 'video_capture_test_winrt',
      #'product_extension': 'appx',
      'type': 'none',
      'dependencies': [
        'video_capture_test_winrt',
      ],
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)/video_capture_test_winrt_package',
          'files':[
            '<(PRODUCT_DIR)/video_capture_test_winrt.exe',
          ],
        },
      ],
      'appx': {
        'dep': '<(PRODUCT_DIR)/video_capture_test_winrt_package/video_capture_test_winrt.exe',
        'dir': '<(PRODUCT_DIR)/video_capture_test_winrt_package',
        'cert': 'video_capture_test_winrt_TemporaryKey.pfx',
        'out': '<(PRODUCT_DIR)/video_capture_test_winrt.appx',
      },
    },
  ],
}
