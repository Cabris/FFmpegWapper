#include "EncoderH264.h"

namespace FFmpegCppWrapper
{
	EncoderH264::EncoderH264(void)
	{
		av_register_all();  
		avcodec_register_all();
		codec_id=AV_CODEC_ID_H264;//28
	}

	void EncoderH264::setSrcSize(int w,int h){
		srcW=w;
		srcH=h;
	}

	void EncoderH264::setDecSize(int w,int h){
		decW=w;
		decH=h;
	}

	void EncoderH264::setBitrate(int b){
		bit_rate=b;
	}

	int EncoderH264::encode(byte src[],int src_size,byte dec[],int* dec_size){
		/*int new_size=src_size/2;
		for(int i=0;i<new_size;i++){
			dec[i]=src[i];
		}
		*dec_size=new_size;*/
		return 1;
	}

	EncoderH264::~EncoderH264(void)
	{
	}
}