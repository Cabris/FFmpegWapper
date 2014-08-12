// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include "Dllmain.h"
#include <GL/glut.h>
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
	int srcW=1280,srcH=720;
	int decW=1280,decH=720;
	int fps=25;

	FFmpegCppWrapper::startEncoder(srcW,srcH,decW,decH,800000,fps);


	int src_size=srcW*srcH*3;
	byte *src=new byte[src_size];
	byte *_src;
	//PIX_FMT_BGR24
	_src=src;
	for(int y = 0; y < srcH; y++) {  
		for(int x = 0; x < srcW; x++) {
			*_src++ = x; // B
			*_src++ = y; // G
			*_src++ = x; // R
		}
	}

	int dec_size=decW*decH*3/2;
	byte *dec=new byte[dec_size];
	int length=10;
	int fc=length*fps;
	for(int c=0;c<fc;c++){
		_src=src;
		for(int y = 0; y < srcH; y++) {  
			for(int x = 0; x < srcW; x++) {
				*_src++ = x+c*255/fc; // B
				*_src++ = 128+y-c*255/fc; // G
				*_src++ = 64-x+c*255/fc; // R
			}
		}
		
		FFmpegCppWrapper::encode(src,src_size,dec,&dec_size);
		fwrite(dec, 1, dec_size, f);
	}

	FFmpegCppWrapper::stopEncoder();
	uint8_t endcode[] = { 0, 0, 1, 0xb7 };
	fwrite(endcode, 1, sizeof(endcode), f);
	fclose(f);
	delete []src;
	delete []dec;
	std::cin.get();
	return 0;
}

