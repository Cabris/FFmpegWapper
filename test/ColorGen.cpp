#include "StdAfx.h"
#include "ColorGen.h"


ColorGen::ColorGen(void)
{
	P=6.03f;
}


ColorGen::~ColorGen(void)
{
}

unsigned char ColorGen::RD(int i,int j){
//#define P 6.03
	float s=3./(j+250),y=(j+sin((i*i+_sq(j-700)*5)/100./DIM+P)*15)*s;
	return (int((i+DIM)*s+y)%2+int((DIM*2-i)*s+y)%2)*127;
}
unsigned char ColorGen::GR(int i,int j){
	float s=3./(j+250);
	float y=(j+sin((i*i+_sq(j-700)*5)/100./DIM+P)*15)*s;
	return (int(5*((i+DIM)*s+y))%2+int(5*((DIM*2-i)*s+y))%2)*127;
}
unsigned char ColorGen::BL(int i,int j){
	float s=3./(j+250);
	float y=(j+sin((i*i+_sq(j-700)*5)/100./DIM+P)*15)*s;
	return (int(29*((i+DIM)*s+y))%2+int(29*((DIM*2-i)*s+y))%2)*127;
}