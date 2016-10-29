#include <stdlib.h>
#include <iostream>
#include "ARCfmt.h"
#include <cmath>
using namespace std;

// Create an empty image
Image::Image()
{
	H = 0;
	W = 0;
    	red_data = NULL;
	green_data = NULL;
	blue_data = NULL;
}

// Create an black image of size height*width
Image::Image(int height, int width)
{    
	int size = height*width;	
	H = height;
	W = width;
	red_data = new unsigned char [size];
	green_data = new unsigned char [size];
	blue_data = new unsigned char [size];

	for(int i = 0; i < size; i++)
	{
		red_data[i]=0;
		green_data[i]=0;
		blue_data[i]=0;
	}
}

// Create an image of size height*width
Image::Image(int height, int width, char data[])
{    
	int size = height*width;
	int sizeTwo = height*width*2;	
	H = height;
	W = width;
	red_data = new unsigned char [size];
	green_data = new unsigned char [size];
	blue_data = new unsigned char [size];

	for(int i = 0; i < size; i++)
	{
		red_data[i] = data[i];
		green_data[i] = data[size+i];
		blue_data[i] = data[sizeTwo+i];
	}
}

//Set the value of three colours of one pixel
void Image::setPixelValue(int position, unsigned char red_value, unsigned char green_value, unsigned char blue_value)
{
	if (0 <= position && position < H*W)
	{
		red_data[position] = red_value;
		green_data[position] = green_value;
		blue_data[position] = blue_value;
		return;
	}
	cout << "That position doesn't exist" << endl;
}

//Set the value of the red colour of one pixel
void Image::setRedPixelValue(int position, unsigned char red_value)
{
	if (0 <= position && position < H*W)
	{
		red_data[position] = red_value;
		return;
	}
	cout << "That position doesn't exist" << endl;
}

//Set the value of the green colour of one pixel
void Image::setGreenPixelValue(int position, unsigned char green_value)
{
	if (0 < position && position < H*W)
	{
		green_data[position] = green_value;
		return;	
	}
	cout << "That position doesn't exist" << endl;
}

//Set the value of the blue colour of one pixel
void Image::setBluePixelValue(int position, unsigned char blue_value)
{
	if (0 <= position && position < H*W)
	{
		blue_data[position] = blue_value;
		return;
	}
	cout << "That position doesn't exist" << endl;
}

int *Image::computeMaxMin()
{
	int size = H*W;	
	static int output[6] = {0,255,0,255,0,255};
	for (int i=0;i<size;i++)
	{
		if(red_data[i]>output[0])
		{
			output[0]=red_data[i];
		}
		if(red_data[i]<output[1])
		{
			output[1]=red_data[i];
		}
	}
	for (int i=0;i<size;i++)
	{
		if(green_data[i]>output[2])
		{
			output[2]=green_data[i];
		}
		if(green_data[i]<output[3])
		{
			output[3]=green_data[i];
		}
	}
	for (int i=0;i<size;i++)
	{
		if(blue_data[i]>output[4])
		{
			output[4]=blue_data[i];
		}
		if(blue_data[i]<output[5])
		{
			output[5]=blue_data[i];
		}
	}
	return output;

}

