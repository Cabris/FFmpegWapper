#include "EncoderH264.h"
#include "VideoStream.h"
namespace FFmpegCppWrapper
{
	EncoderH264::EncoderH264(void)
	{
		
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

	void EncoderH264::initialize(){
		av_register_all();  
		avcodec_register_all();
		filename="doTest.mp4";
		codec_id=AV_CODEC_ID_H264;//28
		c= NULL;

		printf("Encode video file %s\n", filename);
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
		av_opt_set(c->priv_data, "preset", "superfast", 0); 
		av_opt_set(c->priv_data, "tune", "zerolatency", 0);

		/* put sample parameters */
		c->bit_rate = bit_rate;
		/* resolution must be a multiple of two */
		c->width = decW;
		c->height = decH;
		/* frames per second */
		AVRational rate;  
		rate.num = 1;  
		rate.den = STREAM_FRAME_RATE;  
		c->time_base = rate;
		c->gop_size = 10;
		c->pix_fmt = AV_PIX_FMT_YUV420P;

		if (codec_id == AV_CODEC_ID_H264)
			av_opt_set(c->priv_data, "preset", "slow", 0);

		/* open it */
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			exit(1);
		}

		f = fopen(filename, "wb");
		if (!f) {
			fprintf(stderr, "Could not open %s\n", filename);
			exit(1);
		}

		frame = av_frame_alloc();
		if (!frame) {
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

	}

	int EncoderH264::encode(byte src[],int src_size,byte dec[],int* dec_size){
		while (frame->pts<STREAM_NB_FRAMES) {
			av_init_packet(&pkt);
			pkt.data = NULL;    // packet data will be allocated by the encoder
			pkt.size = 0;

			fflush(stdout);
			/* prepare a dummy image */
			/* Y */
			for (y = 0; y < c->height; y++) {
				for (x = 0; x < c->width; x++) {
					frame->data[0][y * frame->linesize[0] + x] = x + y + frame->pts * 3;
				}
			}

			/* Cb and Cr */
			for (y = 0; y < c->height/2; y++) {
				for (x = 0; x < c->width/2; x++) {
					frame->data[1][y * frame->linesize[1] + x] = 128 + y + frame->pts * 2;
					frame->data[2][y * frame->linesize[2] + x] = 64 + x + frame->pts * 5;
				}
			}

			/* encode the image */
			ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
			if (ret < 0) {
				fprintf(stderr, "Error encoding frame\n");
				exit(1);
			}

			if (got_output) {
				int s=pkt.size;
				printf("Write frame %3d (size=%7d)\n", frame->pts, s);
				fwrite(pkt.data, 1, s, f);
				av_free_packet(&pkt);
			}
			frame->pts++;
		}
		return 1;
	}

	void EncoderH264::free_stuff(){
		uint8_t endcode[] = { 0, 0, 1, 0xb7 };
		fwrite(endcode, 1, sizeof(endcode), f);
		fclose(f);
		avcodec_close(c);
		av_free(c);
		av_freep(&frame->data[0]);
		av_frame_free(&frame);
		printf("\n");
	}

	EncoderH264::~EncoderH264(void)
	{
	}
}