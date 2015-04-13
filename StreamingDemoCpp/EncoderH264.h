#pragma once
#include <windows.h>

class EncoderH264
{
	
	typedef void (*_startEncoder)(int srcW,int srcH,int decW,int decH,int bitrate,int framerate);
	typedef int (*_encode)(byte[] ,int,byte[] ,int*);
	typedef void (*_stopEncoder)();

public:
	EncoderH264(int srcW, int srcH, int decW, int decH, int bitrate, int framerate);
	~EncoderH264();
	void Encoding(byte[] ,int,byte[] ,int*);
private:
	HINSTANCE hDLL;        // Handle to DLL
	_startEncoder startEncoder;    // Function pointer
	_encode encode;    // Function pointer
	_stopEncoder stopEncoder;    // Function pointer
	
	int c;
};

