#include "StdAfx.h"
#include "EncoderH264.h"
#include <stdlib.h>
#include <iostream>


EncoderH264::EncoderH264(int srcW, int srcH, int decW, int decH, int bitrate, int framerate)
{
	hDLL = LoadLibrary(L"FFmpegCppWrapper.dll");
	if (hDLL != NULL)
	{
		startEncoder = (_startEncoder)GetProcAddress(hDLL,"startEncoder");
		encode = (_encode)GetProcAddress(hDLL,"encode");
		stopEncoder = (_stopEncoder)GetProcAddress(hDLL,"stopEncoder");

		startEncoder(srcW,srcH,decW,decH,bitrate,framerate);
		c=0;
	}else{
		printf("FUCK, cant load dll\n");
		exit(1);
	}
}


EncoderH264::~EncoderH264()
{
	stopEncoder();
	
	////////////////////////////////////////////////////////////////////
	FreeLibrary(hDLL);
}

void EncoderH264::Encoding(byte in[],int in_size,byte out[],int* out_size){
	encode(in,in_size,out,out_size);
	printf("Write frame %3d (size=%7d)\n",c++, *out_size);
	//printf("data: %s\n",out);
}