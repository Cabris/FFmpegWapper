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
		int encode(byte[] ,int,byte[] ,int*);
	private:
		int srcW,srcH,decW,decH;
		int bit_rate;
		AVCodecID codec_id;
	};

}
