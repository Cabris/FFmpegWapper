// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
//#include "EncoderH264.h"
//#include "EncodeTest.h"
#include "Dllmain.h"
using namespace FFmpegCppWrapper;

int _tmain(int argc, _TCHAR* argv[])
{
	//EncoderH264* encoder=new EncoderH264();
	//encoder->setSrcSize(1280,720);
	//encoder->setDecSize(1280,720);
	//encoder->setBitrate(400000);
	//delete encoder;
	//FFmpegCppWrapper::doTest();

	FFmpegCppWrapper::startEncoder(1280,720,1280,720,1200000);
	
	int src_size=10;
	byte *src=new byte[src_size];
	int dec_size=20;
	byte *dec=new byte[dec_size];
	
	FFmpegCppWrapper::encode(src,src_size,dec,&dec_size);

	FFmpegCppWrapper::stopEncoder();

	std::cin.get();
	return 0;
}

