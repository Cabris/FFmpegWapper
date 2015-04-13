// DemoCpp.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include "EncoderH264.h"
#include "StreamTcpServer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *f;

	/*char *filename="doTest.mp4";
		printf("Encode video file %s\n", filename);
		f = fopen(filename, "wb");
		if (!f) {
			fprintf(stderr, "Could not open %s\n", filename);
			exit(1);
		}*/

	StreamTcpServer server;

	int srcW=1024,srcH=1024;
	int decW=1024,decH=1024;
	int fps=30;
	int length=10;
	EncoderH264 encoder(srcW,srcH,decW,decH,800000,fps);

	int src_size=srcW*srcH*4;//rgba
	byte *src=new byte[src_size];

	int dec_size=decW*decH*3/2;//yuv
	byte *dec=new byte[dec_size];

	int fc=length*fps;
	for(int c=0;c<fc;c++){
		
		////test frame///////
		byte *_src=src;
		for(int y = 0; y < srcH; y++) {  
			for(int x = 0; x < srcW; x++) {
				*_src++ = (byte)(x + c * 255 / fc); // R
				*_src++ = (byte)(128 + y - c * 255 / fc); // G
				*_src++ = (byte)(64 - x + c * 255 / fc); // B
				*_src++ = (byte)(255); // A
			}
		}
		/////////////////////

		encoder.Encoding(src,src_size,dec,&dec_size);
		server.Send(dec,dec_size);
		//fwrite(dec, 1, dec_size, f);
	}

	/*byte endcode[] = { 0, 0, 1, 0xb7 };
	fwrite(endcode, 1, sizeof(endcode), f);
	fclose(f);*/

	delete []src;
	delete []dec;

	system("pause");
	return 0;

}