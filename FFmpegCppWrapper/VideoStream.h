#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "PreDefind.h"
namespace FFmpegCppWrapper
{
#define STREAM_DURATION   5.0/* 5 seconds stream duration */
#define STREAM_FRAME_RATE 30 /* 25 images/s */
#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

	class VideoStream{

	public:
		VideoStream(void);
		~VideoStream(void);
		int testStream();
		int width,height,bit_rate;
	private:
		int sws_flags;
		void capScreen();
		
		/* video output */
		AVFrame *picture, *tmp_picture;
		uint8_t *video_outbuf;
		int frame_count, video_outbuf_size;

		/* add a video output stream */
		AVStream *add_video_stream(AVFormatContext *oc, AVCodecID codec_id);
		AVFrame *alloc_picture(enum PixelFormat pix_fmt, int width, int height);
		void open_video(AVFormatContext *oc, AVStream *st);

		/* prepare a dummy image */
		void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height);
		void write_video_frame(AVFormatContext *oc, AVStream *st);
		void close_video(AVFormatContext *oc, AVStream *st);
	};
}