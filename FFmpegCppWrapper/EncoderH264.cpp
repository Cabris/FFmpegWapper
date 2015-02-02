#include "EncoderH264.h"
//#include "VideoStream.h"
namespace FFmpegCppWrapper
{
	EncoderH264::EncoderH264(void)
	{
		codec_id=AV_CODEC_ID_H264;//28
		c= NULL;
		frame_rate=30;
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

	void EncoderH264::setFramerate(int f){
		frame_rate=f;
	}

	void EncoderH264::initialize(){
		av_register_all();  
		avcodec_register_all();
		AVCodec *codec;
		int ret;
		
		/* find the mpeg1 video encoder */
		codec = avcodec_find_encoder(codec_id);
		if (!codec) {
			fprintf(stderr, "Codec not found\n");
			exit(1);
		}

		c = avcodec_alloc_context3(codec);
		if (!c) {
			fprintf(stderr, "Could not allocate video codec context\n");
			exit(1);
		}
		av_opt_set(c->priv_data, "preset", "ultrafast", 0); 
		av_opt_set(c->priv_data, "tune", "zerolatency", 0);

		/* put sample parameters */
		c->bit_rate = bit_rate;
		c->rc_max_rate = 4000000;
		c->rc_min_rate = 4000000;
		c->rc_buffer_size = 5000000;

		/* resolution must be a multiple of two */
		c->width = decW;
		c->height = decH;
		/* frames per second */
		AVRational rate;  
		rate.num = 1;  
		rate.den = frame_rate;  
		c->time_base = rate;
		c->gop_size = 12;
		c->pix_fmt = AV_PIX_FMT_YUV420P;
		c->trellis=0;
		c->thread_count=3;
		//c->max_b_frames=5;
		/*if (codec_id == AV_CODEC_ID_H264)
			av_opt_set(c->priv_data, "preset", "slow", 0);*/

		/* open it */
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			exit(1);
		}

		frame = av_frame_alloc();
		m_pRGBFrame =  av_frame_alloc(); //RGB幀數據 
		if (!frame||!m_pRGBFrame) {
			fprintf(stderr, "Could not allocate video frame\n");
			exit(1);
		}

		frame->format = c->pix_fmt;
		frame->width  = c->width;
		frame->height = c->height;
		frame->pts=0;
		/* the image can be allocated by any means and av_image_alloc() is
		* just the most convenient way if av_malloc() is to be used */
		ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
			c->pix_fmt, 32);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate raw picture buffer\n");
			exit(1);
		}
		int size = srcW*srcH*4; 
		rgb_buff = new uint8_t[size];
		//初始化SwsContext 
		scxt = sws_getContext(srcW,srcH,PIX_FMT_RGBA ,c->width,c->height,PIX_FMT_YUV420P,SWS_POINT,NULL,NULL,NULL);  
	}

	int EncoderH264::encode(byte src[],int src_size,byte enc[],int* enc_size){
		int ret, got_output;
		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;
		memcpy(rgb_buff,src,src_size);  
		avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGBA , srcW, srcH);

		m_pRGBFrame->data[0]  += m_pRGBFrame->linesize[0] * (srcH - 1);  
		m_pRGBFrame->linesize[0] *= -1;                     
		m_pRGBFrame->data[1]  += m_pRGBFrame->linesize[1] * (srcH / 2 - 1);  
		m_pRGBFrame->linesize[1] *= -1;  
		m_pRGBFrame->data[2]  += m_pRGBFrame->linesize[2] * (srcH / 2 - 1);  
		m_pRGBFrame->linesize[2] *= -1;  

		sws_scale(scxt,m_pRGBFrame->data,m_pRGBFrame->linesize,0,srcH,frame->data,frame->linesize); 

		/* encode the image */
		ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
		if (ret < 0) {
			fprintf(stderr, "Error encoding frame\n");
			exit(1);
		}

		if (got_output) {
			*enc_size=pkt.size;
			long c=frame->pts;
			if(frame->pts==0){//may contain useless data
			
			}
			memcpy(enc,pkt.data,*enc_size); 
			//printf("Write frame %3d (size=%7d)\n",c, *dec_size);
			av_free_packet(&pkt);
		}
		frame->pts++;
		return 1;
	}

	void EncoderH264::free_stuff(){
		avcodec_close(c);
		av_free(c);
		av_freep(&frame->data[0]);
		av_frame_free(&frame);
		av_frame_free(&m_pRGBFrame);
		sws_freeContext(scxt);
		delete []rgb_buff; 
		printf("\n");
	}

	EncoderH264::~EncoderH264(void)
	{
	}
}