#pragma once
#include <iostream>     // std::cin, std::cout
#include "Dllmain.h"

using namespace std;
namespace FFmpegCppWrapper
{
	void startEncoder(int srcW,int srcH,int decW,int decH,int bitrate,int framerate){
		encoder=new EncoderH264();
		encoder->setSrcSize(srcW,srcH);
		encoder->setDecSize(decW,decH);
		encoder->setBitrate(bitrate);
		encoder->setFramerate(framerate);
		encoder->initialize();
	}
	int encode(byte src[],int src_size,byte dec[] ,int* dec_size){
		if(encoder==NULL)
			return -1;
		else{
			return encoder->encode(src,src_size,dec,dec_size);
		}
	}
	void stopEncoder(){
		encoder->free_stuff();
		delete encoder;
	}

	void startDecoder(int srcW,int srcH){
		decoder=new DecoderH264();
		decoder->width=srcW;
		decoder->height=srcH;
		decoder->initialize();
	}
	int decode(byte src[],int src_size,byte dec[] ,int* dec_size){
		if(decoder==NULL)
			return -1;
		else{
			return decoder->decode(src,src_size,dec,dec_size);
		}
	}
	void stopDecoder(){
		decoder->free_stuff();
		delete decoder;
	}

	void streamTest(int w,int h,int br){
		VideoStream2 stream;
		stream.width=w;
		stream.height=h;
		stream.bit_rate=br;
		stream.testStream();
	}

}