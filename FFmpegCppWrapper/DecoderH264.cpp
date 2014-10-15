#include "DecoderH264.h"

namespace FFmpegCppWrapper
{
	DecoderH264::DecoderH264(void)
	{
	}


	DecoderH264::~DecoderH264(void)
	{
		c=NULL;
	}

	void DecoderH264::initialize(){

		codec = avcodec_find_decoder(CODEC_ID_H264);
		if (!codec) {
			fprintf(stderr, "codec not found\n");
			exit(1);
		}
		av_init_packet(&avpkt);
		c = avcodec_alloc_context3(codec);

		if (codec->capabilities & CODEC_CAP_TRUNCATED)
			c->flags |= CODEC_FLAG_TRUNCATED;

		//we can receive truncated frames
		c->flags2 |= CODEC_FLAG2_CHUNKS;
		c->thread_count = 4;//TODO: random value. May be changing can make decoding faster

		picture= avcodec_alloc_frame();
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "could not open codec\n");
			exit(1);
		}
		f = 0;
		in_buff=new uint8_t[width*height*3/2];
		out_buff[0]=new uint8_t[width*height*3];
		out_frame= av_frame_alloc();
		out_frame->format = PIX_FMT_RGB24;
		out_frame->width  = width;
		out_frame->height = height;
		out_frame->pts=0;
		avpicture_fill((AVPicture*)out_frame, (uint8_t*)out_buff, PIX_FMT_RGB24, width, height);
		int ret = av_image_alloc(out_frame->data, out_frame->linesize, width, height,
			PIX_FMT_RGB24, 32);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate raw picture buffer\n");
			exit(1);
		}
		scxt = sws_getContext(1280, 720, PIX_FMT_YUV420P, 
				width, height, PIX_FMT_RGB24, SWS_POINT, NULL, NULL, NULL);
	}

	void DecoderH264::build_avpkt(byte src[],int src_size){
		memcpy(in_buff,src,src_size); 
		avpkt.data = in_buff;
		avpkt.size = src_size;
	}

	int DecoderH264::decode(byte src[],int src_size,byte dec[],int* dec_size){
		int got_picture, len;
		build_avpkt(src,src_size);
		len = avcodec_decode_video2(c, picture, &got_picture, &avpkt);
		if (got_picture) {
			printf("saving frame %3d\n", f);
			if(!scxt)
				scxt = sws_getContext(c->width, c->height, c->pix_fmt, 
				width, height, PIX_FMT_RGB24, SWS_POINT, NULL, NULL, NULL);

			sws_scale(scxt,picture->data, picture->linesize, 0, c->height, out_frame->data, out_frame->linesize);
			*dec_size=width*height*3;
			memcpy(dec,out_frame->data[0],*dec_size); 
			f++;
		}else{
			printf("decode frame failed\n");
		}
		av_free_packet(&avpkt);
		return 1;
	}

	void DecoderH264::free_stuff(){
		avcodec_close(c);
		av_free(c);
		//av_freep(&picture->data[0]);
		//av_freep(&picture->data[1]);
		//av_freep(&picture->data[2]);
		av_frame_free(&picture);
		av_freep(&out_frame->data[0]);
		av_frame_free(&out_frame);
		sws_freeContext(scxt);
		delete[] in_buff;
		delete[] out_buff[0];
	}


}