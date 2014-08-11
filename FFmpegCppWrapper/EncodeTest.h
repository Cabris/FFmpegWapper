#pragma once

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#include <stdint.h>
#include <cstdint>
#include <stdio.h>
#include <tchar.h>
extern "C" 
{
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libavutil\mathematics.h"
#include "libavutil\opt.h"
#include "libavutil\imgutils.h"
}

namespace FFmpegCppWrapper
{
	class EncodeTest
	{
	public:
		EncodeTest(void);
		~EncodeTest(void);
		void video_encode_example(const char *filename, AVCodecID codec_id);
		void video_encode_example(const char *filename);
	private:
	};

	extern "C" __declspec(dllexport) void doTest();
	extern "C" __declspec(dllexport) void doTest2(unsigned char[] ,int*);
	extern "C" __declspec(dllexport) void video_encode(const char *, AVCodecID);


}