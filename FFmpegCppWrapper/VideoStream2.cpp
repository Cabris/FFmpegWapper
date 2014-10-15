#include "VideoStream2.h"

namespace FFmpegCppWrapper
{
	VideoStream2::VideoStream2(void)
	{
		sws_flags = SWS_BICUBIC;
		bit_rate=1500000;
		width=1280;
		height=720;
	}

		/* media file output */
	int VideoStream2::testStream()
	{
		capScreen();
		const char *filename;
		AVOutputFormat *fmt;
		AVFormatContext *oc;
		AVStream  *video_st;
		double video_pts;
		int i;

		/* initialize libavcodec, and register all codecs and formats */
		av_register_all();

		//#define RTMP_STREAM
#ifdef RTMP_STREAM
		filename = "rtmp://192.168.0.239/live/livestream";
#else 
		filename = "1.mp4";
#endif

		/* allocate the output media context */
		avformat_alloc_output_context2(&oc, NULL, NULL, filename);
		if (!oc)
		{
			printf("Could not deduce output format from file extension: using MPEG.\n");
			avformat_alloc_output_context2(&oc, NULL, /*"mpeg"*/"flv", filename);
		}

		if (!oc)
		{
			return 1;
		}

		// 强制指定 264 编码
		oc->oformat->video_codec = CODEC_ID_H264;
		//oc->oformat->audio_codec = CODEC_ID_AAC;

		fmt = oc->oformat;

		/* add the audio and video streams using the default format codecs
		and initialize the codecs */
		video_st = NULL;
		if (fmt->video_codec != CODEC_ID_NONE) {
			video_st = add_video_stream(oc, fmt->video_codec);
		}

		av_dump_format(oc, 0, filename, 1);

		/* now that all the parameters are set, we can open the audio and
		video codecs and allocate the necessary encode buffers */
		if (video_st)
			open_video(oc, video_st);

		/* open the output file, if needed */
		if (!(fmt->flags & AVFMT_NOFILE)) {
			if (avio_open(&oc->pb, filename, AVIO_FLAG_WRITE) < 0) {
				fprintf(stderr, "Could not open '%s'\n", filename);
				return 1;
			}
		}

		/* write the stream header, if any */
		avformat_write_header(oc, NULL);
		picture->pts = 0;

		for(;;)
		{
			if (video_st)
				video_pts = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
			else
				video_pts = 0.0;

			if (!video_st || video_pts >= STREAM_DURATION)
				break;

			/* write interleaved audio and video frames */
			if (video_st) {
				write_video_frame(oc, video_st);
				picture->pts++;
			} 
		}

		/* write the trailer, if any.  the trailer must be written
		* before you close the CodecContexts open when you wrote the
		* header; otherwise write_trailer may try to use memory that
		* was freed on av_codec_close() */
		av_write_trailer(oc);

		/* close each codec */
		if (video_st)
			close_video(oc, video_st);

		/* free the streams */
		for(i = 0; i < oc->nb_streams; i++) {
			av_freep(&oc->streams[i]->codec);
			av_freep(&oc->streams[i]);
		}

		if (!(fmt->flags & AVFMT_NOFILE)) {
			/* close the output file */
			avio_close(oc->pb);
		}

		/* free the stream */
		av_free(oc);

		return 0;
	}

	AVStream* VideoStream2::add_video_stream(AVFormatContext *oc, AVCodecID codec_id)
	{
		AVCodecContext *c;
		AVStream *st;
		AVCodec *codec;

		st = avformat_new_stream(oc, NULL);
		if (!st) {
			fprintf(stderr, "Could not alloc stream\n");
			exit(1);
		}

		c = st->codec;

		/* find the video encoder */
		codec = avcodec_find_encoder(codec_id);
		if (!codec) {
			fprintf(stderr, "codec not found\n");
			exit(1);
		}
		avcodec_get_context_defaults3(c, codec);
		av_opt_set(c->priv_data, "preset", "superfast", 0); 
		av_opt_set(c->priv_data, "tune", "zerolatency", 0);
		//c->profile = FF_PROFILE_H264_BASELINE;
		c->codec_id = codec_id;

		/* put sample parameters */
		c->bit_rate = /*400000*/bit_rate;
		/* resolution must be a multiple of two */
		c->width = /*352*/width;
		c->height = /*288*/height;
		/* time base: this is the fundamental unit of time (in seconds) in terms
		of which frame timestamps are represented. for fixed-fps content,
		timebase should be 1/framerate and timestamp increments should be
		identically 1. */
		c->time_base.den = STREAM_FRAME_RATE;
		c->time_base.num = 1;
		c->gop_size = 10; /* emit one intra frame every twelve frames at most */
		c->pix_fmt = STREAM_PIX_FMT;
		if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
			/* just for testing, we also add B frames */
			c->max_b_frames = 2;
		}
		if (c->codec_id == CODEC_ID_MPEG1VIDEO){
			/* Needed to avoid using macroblocks in which some coeffs overflow.
			This does not happen with normal video, it just happens here as
			the motion of the chroma plane does not match the luma plane. */
			c->mb_decision=2;
		}
		// some formats want stream headers to be separate
		if (oc->oformat->flags & AVFMT_GLOBALHEADER)
			c->flags |= CODEC_FLAG_GLOBAL_HEADER;

		return st;
	}

	AVFrame* VideoStream2::alloc_picture(enum PixelFormat pix_fmt, int width, int height)
	{
		AVFrame *picture;
		uint8_t *picture_buf;
		int size;

		picture = avcodec_alloc_frame();
		if (!picture)
			return NULL;
		size = avpicture_get_size(pix_fmt, width, height);
		picture_buf = (uint8_t *)av_malloc(size);
		if (!picture_buf) {
			av_free(picture);
			return NULL;
		}
		avpicture_fill((AVPicture *)picture, picture_buf,
			pix_fmt, width, height);
		return picture;
	}

	void VideoStream2::open_video(AVFormatContext *oc, AVStream *st)
	{
		AVCodec *codec;
		AVCodecContext *c;

		c = st->codec;

		/* find the video encoder */
		codec = avcodec_find_encoder(c->codec_id);
		if (!codec) {
			fprintf(stderr, "codec not found\n");
			exit(1);
		}

		/* open the codec */
		if (avcodec_open2(c, codec,NULL) < 0) {
			fprintf(stderr, "could not open codec\n");
			exit(1);
		}

		video_outbuf = NULL;
		if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
			/* allocate output buffer */
			/* XXX: API change will be done */
			/* buffers passed into lav* can be allocated any way you prefer,
			as long as they're aligned enough for the architecture, and
			they're freed appropriately (such as using av_free for buffers
			allocated with av_malloc) */
			video_outbuf_size = 200000;
			video_outbuf = (uint8_t *)av_malloc(video_outbuf_size);
		}

		/* allocate the encoded raw picture */
		picture = alloc_picture(c->pix_fmt, c->width, c->height);
		if (!picture) {
			fprintf(stderr, "Could not allocate picture\n");
			exit(1);
		}

		/* if the output format is not YUV420P, then a temporary YUV420P
		picture is needed too. It is then converted to the required
		output format */
		tmp_picture = NULL;
		if (c->pix_fmt != PIX_FMT_YUV420P) {
			tmp_picture = alloc_picture(PIX_FMT_YUV420P, c->width, c->height);
			if (!tmp_picture) {
				fprintf(stderr, "Could not allocate temporary picture\n");
				exit(1);
			}
		}
	}

	/* prepare a dummy image */
	void VideoStream2::fill_yuv_image(AVFrame *pict, int frame_index, int width, int height)
	{
		int x, y, i;

		i = frame_index;

		/* Y */
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
			}
		}

		/* Cb and Cr */
		for (y = 0; y < height/2; y++) {
			for (x = 0; x < width/2; x++) {
				pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
				pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
			}
		}
	}

	void VideoStream2::write_video_frame(AVFormatContext *oc, AVStream *st)
	{
		int out_size,ret;
		AVCodecContext *c;
		static struct SwsContext *img_convert_ctx;

		c = st->codec;

		if (frame_count >= STREAM_NB_FRAMES) {
			/* no more frame to compress. The codec has a latency of a few
			frames if using B frames, so we get the last frames by
			passing the same picture again */
		} else {
			if (c->pix_fmt != PIX_FMT_YUV420P) {
				/* as we only generate a YUV420P picture, we must convert it
				to the codec pixel format if needed */
				if (img_convert_ctx == NULL) {
					img_convert_ctx = sws_getContext(c->width, c->height,
						PIX_FMT_YUV420P,
						c->width, c->height,
						c->pix_fmt,
						sws_flags, NULL, NULL, NULL);
					if (img_convert_ctx == NULL) {
						fprintf(stderr, "Cannot initialize the conversion context\n");
						exit(1);
					}
				}
				fill_yuv_image(tmp_picture, frame_count, c->width, c->height);
				sws_scale(img_convert_ctx, tmp_picture->data, tmp_picture->linesize,
					0, c->height, picture->data, picture->linesize);
			} else {
				fill_yuv_image(picture, frame_count, c->width, c->height);
			}
		}


		if (oc->oformat->flags & AVFMT_RAWPICTURE) {
			///* raw video case. The API will change slightly in the near
			//future for that. */
			//AVPacket pkt;
			//av_init_packet(&pkt);

			//pkt.flags |= AV_PKT_FLAG_KEY;
			//pkt.stream_index = st->index;
			//pkt.data = (uint8_t *)picture;
			//pkt.size = sizeof(AVPicture);

			//ret = av_interleaved_write_frame(oc, &pkt);
		} 
		else {
			/* encode the image */  
			out_size = avcodec_encode_video(c, video_outbuf, video_outbuf_size, picture);  
			/* if zero size, it means the image was buffered */  
			if (out_size > 0) {  
				AVPacket pkt;  
				av_init_packet(&pkt);  

				if (c->coded_frame->pts != AV_NOPTS_VALUE)  
					pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);  
				if(c->coded_frame->key_frame)  
					pkt.flags |= AV_PKT_FLAG_KEY;  
				pkt.stream_index = st->index;  
				pkt.data = video_outbuf;  
				pkt.size = out_size;  

				/* write the compressed frame in the media file */  
				ret = av_write_frame(oc, &pkt); 
			} else {  
				ret = 0;  
			}
		}
		if (ret != 0) {
			fprintf(stderr, "Error while writing video frame\n");
			exit(1);
		}
		frame_count++;
	}

	void VideoStream2::close_video(AVFormatContext *oc, AVStream *st)
	{
		avcodec_close(st->codec);
		av_free(picture->data[0]);
		av_free(picture);
		if (tmp_picture) {
			av_free(tmp_picture->data[0]);
			av_free(tmp_picture);
		}
		av_free(video_outbuf);
	}

	void VideoStream2::capScreen(){
		
	}


	VideoStream2::~VideoStream2(void)
	{
	}
}