/*
*  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/

#pragma once

namespace rtpPlayerTests {
using namespace LibTest_runner;
  // TODO(winrt) Add more test with different arguments when/if needed

  // class: CRtpPlayerTest
  // executes rtp_player_test project with default parameters
class CRtpPlayerTest :
  public LibTest_runner::CTestBase {
 private:
    AUTO_ADD_TEST(libSrtpTests::SingleInstanceTestSolutionProvider,
    CRtpPlayerTest);
 protected:
    int InterchangeableExecute();
 public:
    virtual ~CRtpPlayerTest() {}
    TEST_NAME_IMPL(RtpPlayerTest);
    TEST_PROJECT_IMPL(rtp_player_test);
    TEST_LIBRARY_IMPL(rtpPlayer);
};

typedef std::shared_ptr<CRtpPlayerTest> SpRtpPlayerTest_t;

}  // namespace rtpPlayerTests
