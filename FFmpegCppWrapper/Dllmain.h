#include <stdio.h>
#include "EncoderH264.h"
#include "DecoderH264.h"
#include "VideoStream2.h"


namespace FFmpegCppWrapper
{
	EncoderH264* encoder=NULL; 
	DecoderH264* decoder=NULL;
	extern "C" __declspec(dllexport) void startEncoder(int srcW,int srcH,int decW,int decH,int bitrate,int framerate);
	extern "C" __declspec(dllexport) int encode(byte[] ,int,byte[] ,int*);
	extern "C" __declspec(dllexport) void stopEncoder();

	extern "C" __declspec(dllexport) void startDecoder(int width,int height);
	extern "C" __declspec(dllexport) int decode(byte[] ,int,byte[] ,int*);
	extern "C" __declspec(dllexport) void stopDecoder();

	extern "C" __declspec(dllexport) void streamTest(int w,int h,int br);
}
