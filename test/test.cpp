// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "EncoderH264.h"
#include "EncodeTest.h"
using namespace FFmpegCppWrapper;

int _tmain(int argc, _TCHAR* argv[])
{
	//EncoderH264* encoder=new EncoderH264();
	//encoder->setSrcSize(1280,720);
	//encoder->setDecSize(1280,720);
	//encoder->setBitrate(400000);
	//delete encoder;
	FFmpegCppWrapper::doTest();
	std::cin.get();
	return 0;
}

