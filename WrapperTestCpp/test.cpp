// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "Dllmain.h"
#include "ColorGen.h"
#include <stdlib.h>

using namespace FFmpegCppWrapper;

int _tmain(int argc, _TCHAR* argv[])
{
	char *filename="doTest.mp4";
	printf("Encode video file %s\n", filename);
	FILE *f;
	f = fopen(filename, "wb");
	if (!f) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
	int srcW=1024,srcH=1024;
	int decW=1024,decH=1024;
	int fps=30;
	int length=5;
	//FFmpegCppWrapper::streamTest(srcW,srcH,800000);
	FFmpegCppWrapper::startEncoder(srcW,srcH,decW,decH,8000000,fps);
	//FFmpegCppWrapper::startDecoder(decW,decH);

	int src_size=srcW*srcH*4;
	byte *src=new byte[src_size];
	byte *_src;
	//PIX_FMT_RGB24
	_src=src;
	for(int y = 0; y < srcH; y++) {  
		for(int x = 0; x < srcW; x++) {
			*_src++ = x%255; // R
			*_src++ = y%255; // G
			*_src++ = x%255; // B
			*_src++ = x%255; // A
		}
	}

	int dec_size=decW*decH*3/2;
	byte *dec=new byte[dec_size];

	byte *dec_out=new byte[src_size];
	int dec_out_size=0;
	//ColorGen cg;
	int fc=length*fps;
	for(int c=0;c<fc;c++){
		_src=src;
		for(int y = 0; y < srcH; y++) {  
			for(int x = 0; x < srcW; x++) {
				//*_src++ = cg.RD(x,y)%255; // R
				//*_src++ = cg.GR(x,y)%255; // G
				//*_src++ = cg.BL(x,y)%255; // B

				*_src++ = (byte)(x + c * 255 / fc); // R
				*_src++ = (byte)(128 + y - c * 255 / fc); // G
				*_src++ = (byte)(64 - x + c * 255 / fc); // B
				*_src++ = (byte)(255); // A

			}
		}
		//cg.P+=0.1f;
		FFmpegCppWrapper::encode(src,src_size,dec,&dec_size);
		fwrite(dec, 1, dec_size, f);
		printf("Write frame %3d (size=%7d)\n",c, dec_size);
		//FFmpegCppWrapper::decode(dec,dec_size,dec_out,&dec_out_size);
		//printf("saving frame, size=  %3d**\n", dec_size);


	}

	FFmpegCppWrapper::stopEncoder();
	//FFmpegCppWrapper::stopDecoder();

	uint8_t endcode[] = { 0, 0, 1, 0xb7 };
	fwrite(endcode, 1, sizeof(endcode), f);
	fclose(f);
	delete []src;
	delete []dec;
	delete []dec_out;
	system("pause");
	return 0;
}

