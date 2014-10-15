#pragma once
#include <iostream>
#include <cmath>
#include <cstdlib>
#define DIM 1024
#define DM1 (DIM-1)
#define _sq(x) ((x)*(x)) // square
#define _cb(x) abs((x)*(x)*(x)) // absolute value of cube
#define _cr(x) (unsigned char)(pow((x),1.0/3.0)) // cube root
class ColorGen
{
public:
	ColorGen(void);
	~ColorGen(void);
	float P;
	unsigned char RD(int i,int j);
	unsigned char GR(int i,int j);
	unsigned char BL(int i,int j);
};

