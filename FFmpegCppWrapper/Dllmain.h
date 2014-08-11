#include <stdio.h>
#include "EncoderH264.h"
#include "VideoStream.cpp"

namespace FFmpegCppWrapper
{
	EncoderH264* encoder=NULL; 
	extern "C" __declspec(dllexport) void startEncoder(int srcW,int srcH,int decW,int decH,int bitrate);
	extern "C" __declspec(dllexport) int encode(byte[] ,int,byte[] ,int*);
	extern "C" __declspec(dllexport) void stopEncoder();

	extern "C" __declspec(dllexport) void streamTest(int w,int h,int br);
}
