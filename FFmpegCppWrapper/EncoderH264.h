#pragma once
#include <stdio.h>
#include "PreDefind.h"

namespace FFmpegCppWrapper
{
	typedef unsigned char byte;
	class EncoderH264
	{
	public:
		EncoderH264(void);
		~EncoderH264(void);
		void setSrcSize(int w,int h);
		void setDecSize(int w,int h);
		void setBitrate(int b);
		void initialize();
		int encode(byte[] ,int,byte[] ,int*);
		void free_stuff();
	private:
		int srcW,srcH,decW,decH;
		int bit_rate;
		char *filename;
		AVCodecID codec_id;
		AVCodec *codec;
		AVCodecContext *c;
		int ret, x, y, got_output;
		FILE *f;
		AVFrame *frame;
		AVPacket pkt;
	};

}
