#pragma once
#include <stdio.h>
#include "PreDefind.h"

namespace FFmpegCppWrapper
{
	
	class EncoderH264
	{
	public:
		EncoderH264(void);
		~EncoderH264(void);
		void setSrcSize(int w,int h);
		void setDecSize(int w,int h);
		void setBitrate(int b);
		void setFramerate(int f);
		void initialize();
		int encode(byte[] ,int,byte[] ,int*);
		void free_stuff();
		int srcW,srcH,decW,decH;
		int bit_rate,frame_rate;
	private:
		AVCodecID codec_id;
		AVCodecContext *c;
		AVFrame *frame;
		AVFrame *m_pRGBFrame;
		uint8_t * rgb_buff;
		SwsContext * scxt;
		AVPacket pkt;
	};

}
