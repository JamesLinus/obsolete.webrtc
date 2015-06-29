# Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

{
  'includes': ['../../../common.gypi',],
  'variables': {

  },
  'targets': [
    {
      'target_name': 'gtest_runner',
      'type': 'executable',
      'dependencies': [
        '../../../../../talk/libjingle_tests.gyp:libjingle_media_unittest',
        '../../../../../talk/libjingle_tests.gyp:libjingle_p2p_unittest',
        # TODO(winrt) uncomment when the issue with microphone access is fixed
        #'../../../../../talk/libjingle_tests.gyp:libjingle_peerconnection_unittest',
        '../../../../../webrtc/common_audio/common_audio.gyp:common_audio_unittests',
        '../../../../../webrtc/common_video/common_video_unittests.gyp:common_video_unittests',
        # TODO(winrt) uncomment when the issue with microphone access is fixed
        #'../../../../../webrtc/modules/modules.gyp:audio_device_tests',
        '../../../../../webrtc/modules/modules.gyp:modules_unittests',
        '../../../../../webrtc/modules/modules.gyp:video_capture_tests',
        '../../../../../webrtc/system_wrappers/system_wrappers_tests.gyp:system_wrappers_unittests',
        '../../../../../webrtc/test/test.gyp:test_support',
        '../../../../../webrtc/test/test.gyp:test_support_main',
        '../../../../../webrtc/test/test.gyp:test_support_unittests',
        '../../../../../webrtc/video_engine/video_engine.gyp:video_engine_core_unittests',
        '../../../../../webrtc/voice_engine/voice_engine.gyp:voice_engine_unittests',
        '../../../../../webrtc/webrtc.gyp:rtc_unittests',
        '../../../../../third_party/libyuv/libyuv_test.gyp:libyuv_unittest',
      ],
      'defines': [
        'GTEST_RELATIVE_PATH',
        '_HAS_EXCEPTIONS=1',
      ],
      'include_dirs': [
        '../../../../../testing/gtest/include',
      ],
      'sources': [
        'App.cpp',
        'gtest_runner_TemporaryKey.pfx',
        'Logo.png',
        'SmallLogo.png',
        'StoreLogo.png',
      ],
      'forcePackage': [
            '../../../../../resources/',
            '../../../../../data/',
            '../../../../../talk/media/testdata/',
      ],
      'conditions': [
        ['OS_RUNTIME=="winrt" and winrt_platform=="win_phone"', {
          'sources': [
            'Package.phone.appxmanifest',
            'Logo71x71.png',
            'Logo44x44.png',
            'SplashScreen480x800.png',
          ],
        }],
          ['OS_RUNTIME=="winrt" and winrt_platform=="win"', {
          'sources': [
            'Package.appxmanifest',
            'SplashScreen.png',
          ],
        }],
        ['OS_RUNTIME=="winrt" and (winrt_platform=="win10" or winrt_platform=="win10_arm")', {
          'sources': [
            'Generated Manifest Win10\AppxManifest.xml',
            'Package.Win10.appxmanifest',
            'SplashScreen.png',
          ],
        }],
      ],
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)/gtest_runner_package',
          'conditions': [
            ['OS_RUNTIME=="winrt" and winrt_platform=="win_phone"', {
              'files': [	
                 'Generated Manifest Phone\AppxManifest.xml',								
                 'Logo71x71.png',
                 'Logo44x44.png',
                 'SplashScreen480x800.png',
              ],
            }],
            ['OS_RUNTIME=="winrt" and winrt_platform=="win"', {
              'files': [
                'Generated Manifest\AppxManifest.xml',								
                'SplashScreen.png',
              ],
            }],
            ['OS_RUNTIME=="winrt" and winrt_platform=="win10"', {
              'files': [
                'Generated Manifest Win10\AppxManifest.xml',
                'SplashScreen.png',
              ],
            }],
          ],
          'files':[
            'Logo.png',
            'SmallLogo.png',
            'StoreLogo.png',
            '../../../../../data/',
            '../../../../../resources/',
            '../../../../../talk/media/testdata/',
          ],
        },
        # Hack for MSVS to copy to the Appx folder
        {
          'destination': '<(PRODUCT_DIR)/AppX',
          'conditions': [
              ['OS_RUNTIME=="winrt" and winrt_platform=="win_phone"', {
                'files': [	
                   'Logo71x71.png',
                   'Logo44x44.png',	
                   'SplashScreen480x800.png',							
                ],
            }],
             ['OS_RUNTIME=="winrt" and winrt_platform!="win_phone"', {
                'files': [
                  'SplashScreen.png',
                ],
            }],
          ],
          'files':[
            'Logo.png',
            'SmallLogo.png',
            'StoreLogo.png',
            '../../../../../data/',
            '../../../../../resources/',
            '../../../../../talk/media/testdata/',
          ],
        },
      ],
      'msvs_disabled_warnings': [
        4453,  # A '[WebHostHidden]' type should not be used on the published surface of a public type that is not '[WebHostHidden]'
      ],
      'msvs_package_certificate': {
        'KeyFile': 'gtest_runner_TemporaryKey.pfx',
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
           'ws2_32.lib',
          ],
          # 2 == /SUBSYSTEM:WINDOWS
          'SubSystem': '2',
          'conditions': [
            ['OS_RUNTIME=="winrt" and winrt_platform=="win_phone"', {
              'AdditionalOptions': [
                # Fixes linking for assembler opus source files 
                '<(PRODUCT_DIR)/obj/opus/celt_pitch_xcorr_arm.obj',
                '<(SHARED_INTERMEDIATE_DIR)/third_party/libvpx/*.obj',
                '../../../../../third_party/libyuv/source/*.obj',
                '<(PRODUCT_DIR)/obj/openmax_dl_armv7/*.obj',
                '<(PRODUCT_DIR)/obj/openmax_dl_neon/*.obj',
              ],
            }],
            ['OS_RUNTIME=="winrt" and (winrt_platform=="win10" or winrt_platform=="win10_arm")', {
			  'AdditionalDependencies': [
			   'ws2_32.lib',
		       'WindowsApp.lib',
              ],
            }],
          ],
        },
      },
    },
    {
      'target_name': 'gtest_runner_appx',
      'type': 'none',
      'dependencies': [
        'gtest_runner',
      ],
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)/gtest_runner_package',
          'files':[
            '<(PRODUCT_DIR)/gtest_runner.exe',
          ],
        },
      ],
      'appx': {
        'dep': '<(PRODUCT_DIR)/gtest_runner_package/gtest_runner.exe',
        'dir': '<(PRODUCT_DIR)/gtest_runner_package',
        'cert': 'gtest_runner_TemporaryKey.pfx',
        'out': '<(PRODUCT_DIR)/gtest_runner.appx',
      },
    },
  ],
}
