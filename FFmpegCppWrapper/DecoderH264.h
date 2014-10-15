#pragma once
#include <stdio.h>
#include "PreDefind.h"

namespace FFmpegCppWrapper
{
	class DecoderH264
	{
	public:
		DecoderH264(void);
		~DecoderH264(void);
		void initialize();
		int decode(byte[] ,int,byte[] ,int*);
		void free_stuff();
		int width,height;
	private:
		void build_avpkt(byte[] ,int);
		AVCodec *codec;
		AVCodecContext *c;
		AVPacket avpkt;
		AVFrame *picture;
		AVFrame* out_frame;
		SwsContext * scxt;
		uint8_t * in_buff;
		uint8_t * out_buff[4];
		int f;
	};
}
