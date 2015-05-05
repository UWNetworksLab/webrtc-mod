/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_VIDEO_VIDEO_RECEIVE_STREAM_H_
#define WEBRTC_VIDEO_VIDEO_RECEIVE_STREAM_H_

#include <vector>

#include "webrtc/base/scoped_ptr.h"
#include "webrtc/call.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"
#include "webrtc/modules/video_render/include/video_render_defines.h"
#include "webrtc/modules/video_render/incoming_video_stream.h"
#include "webrtc/system_wrappers/interface/clock.h"
#include "webrtc/video/encoded_frame_callback_adapter.h"
#include "webrtc/video/receive_statistics_proxy.h"
#include "webrtc/video/transport_adapter.h"
#include "webrtc/video_engine/include/vie_render.h"
#include "webrtc/video_engine/vie_channel.h"
#include "webrtc/video_engine/vie_channel_group.h"
#include "webrtc/video_engine/vie_encoder.h"
#include "webrtc/video_receive_stream.h"

namespace webrtc {

class VideoEngine;
class ViEBase;
class VoiceEngine;

namespace internal {

class VideoReceiveStream : public webrtc::VideoReceiveStream,
                           public I420FrameCallback,
                           public VideoRenderCallback {
 public:
  VideoReceiveStream(webrtc::VideoEngine* video_engine,
                     ChannelGroup* channel_group,
                     const VideoReceiveStream::Config& config,
                     newapi::Transport* transport,
                     webrtc::VoiceEngine* voice_engine,
                     int base_channel);
  virtual ~VideoReceiveStream();

  void Start() override;
  void Stop() override;
  Stats GetStats() const override;

  // Overrides I420FrameCallback.
  void FrameCallback(I420VideoFrame* video_frame) override;

  // Overrides VideoRenderCallback.
  int RenderFrame(const uint32_t /*stream_id*/,
                  const I420VideoFrame& video_frame) override;

  void SignalNetworkState(Call::NetworkState state);

  bool DeliverRtcp(const uint8_t* packet, size_t length);
  bool DeliverRtp(const uint8_t* packet, size_t length);

 private:
  void SetRtcpMode(newapi::RtcpMode mode);

  TransportAdapter transport_adapter_;
  EncodedFrameCallbackAdapter encoded_frame_proxy_;
  const VideoReceiveStream::Config config_;
  Clock* const clock_;

  ChannelGroup* const channel_group_;
  ViEChannel* vie_channel_;
  rtc::scoped_ptr<IncomingVideoStream> incoming_video_stream_;

  ViEBase* video_engine_base_;

  VoEVideoSync* voe_sync_interface_;

  rtc::scoped_ptr<ReceiveStatisticsProxy> stats_proxy_;

  int channel_;
};
}  // namespace internal
}  // namespace webrtc

#endif  // WEBRTC_VIDEO_VIDEO_RECEIVE_STREAM_H_
