#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <math.h>

#include "ARCfmt.h"
#include "ARCfmt.cpp"

using namespace std;

int readImageHeader(char[], int&, int&);
int readImage(char[], Image&);
int writeImage(char[], Image&);
int saveMaxMin(char[], Image&);
Image turnToGreyscale(Image&);
int saveHistogram(char[], Image&, int);
int maskImage(char[], char[], Image&);
int turnSelectionToGreyscale(char[], double, Image&);
int rotate(char[], int, Image&);

int main(int argc, char *argv[])
{
	int H, W;
	int choice = 0;
	int input = 0;
	int filename = 0;
	int interval = 0;
	int maskFile = 0;
	int radius = 0;
	int angle = 0;
	

	std::vector <std::string> myVector;
	std::string dest;
	for(int i=1; i<argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-u")
		{
			choice = i+1;
		}
		if (arg == "-i")
		{
			input = i+1;
		}
		if (arg == "-fm")
		{
			maskFile = i+1;
		}
		if (arg == "-o")
		{
			filename = i+1;
		}
		if (arg == "-t")
		{
			interval = i+1;
		}
		if (arg == "-r")
		{
			radius = i+1;
		}
		if (arg == "-a")
		{
			angle = i+1;
		}
	}

	readImageHeader(argv[input], H, W);

    // Allocate memory for the image
	Image image(H,W);

	// Read image
	readImage(argv[input], image);

	switch (atoi(argv[choice])) 
	{
		case 0:
			saveHistogram(argv[filename], image, atoi(argv[interval]));
			break;
		case 1:
			saveMaxMin(argv[filename], image);
			break;
		case 2:
			maskImage(argv[filename], argv[maskFile], image);
			break;
		case 3:
			rotate(argv[filename], atoi(argv[angle]), image);
			break;
		case 4:
			turnSelectionToGreyscale(argv[filename], atof(argv[radius]), image);
			break;
			
	}
	return 0;
}


int readImage(char filename[], Image& image)
{
    ifstream ifp;

   	ifp.open(filename, ios::in | ios::binary);

    if (!ifp)
    {
        cout << "Error reading file " << filename << endl;
        exit(1);
    }


	ifp.seekg(2*sizeof(int),ifp.beg);
    ifp.read( reinterpret_cast<char *>(image.red_data), (image.H*image.W)*sizeof(char));
	ifp.read( reinterpret_cast<char *>(image.green_data), (image.H*image.W)*sizeof(char));
	ifp.read( reinterpret_cast<char *>(image.blue_data), (image.H*image.W)*sizeof(char));

    ifp.close();

    return (1);
}


int readImageHeader(char filename[], int& H, int& W)
{
	char firstInteger[4], secondInteger[4];
	ifstream ifp;

   	ifp.open(filename, ios::in | ios::binary);

    if (!ifp)
    {
        cout << "Error reading file " << filename << endl;
        exit(1);
    }

	// Read size
    ifp.read(firstInteger, 4);
    ifp.read(secondInteger, 4);
	//LITTLE ENDIAN
	H = (int) firstInteger[0]+(int) 256*firstInteger[1]+
		 (int) 256*256*firstInteger[2]+(int) 256*256*256*firstInteger[3];
	W = (int) secondInteger[0]+(int) 256*secondInteger[1]+
		(int) 256*256*secondInteger[2]+(int) 256*256*256*secondInteger[3];

    ifp.close();
    return(1);
}

int writeImage(char filename[], Image& image)
{
	int H = image.H, W = image.W;    	
	ofstream ofp;

    ofp.open(filename, ios::out | ios::binary | ios::trunc);

    if (!ofp) 
    {
        cout << "Can't open file " << filename << endl;
        exit(1);
    }

	ofp << (char)(H%256);
	ofp << (char)(H/256);
	ofp << (char)(H/256*256);
	ofp << (char)(H/256*256*256);
	ofp << (char)(W%256);
	ofp << (char)(W/256);
	ofp << (char)(W/256*256);
	ofp << (char)(W/256*256*256);	
	ofp.write( reinterpret_cast<char *>(image.red_data), (H*W)*sizeof(char));
    ofp.write( reinterpret_cast<char *>(image.green_data), (H*W)*sizeof(char));
    ofp.write( reinterpret_cast<char *>(image.blue_data), (H*W)*sizeof(char));

    if (ofp.fail()) 
    {
		cout << "Can't write image " << filename << endl;
    	exit(0);
    }

    ofp.close();

    return(1);
}

int saveMaxMin(char filename[], Image& image){

	int *output = image.computeMaxMin();
	ofstream ofp;

    ofp.open(filename, ios::out | ios::binary);

    if (!ofp) 
    {
        cout << "Can't open file " << filename << endl;
        exit(1);
    }
	
	for (int i=0;i<5;i++)
	{
		ofp << output[i];
		ofp << " ";
	}
	ofp << output[5];

	if (ofp.fail()) 
    {
        cout << "Can't write image " << filename << endl;
        exit(0);
    }

	ofp.close();

    return(1);
}

Image turnToGreyscale(Image& image)
{
	int H = image.H, W = image.W;
	int size = H*W;
	Image greyImage(H, W);
	// Experimental order for performance (may be worse than operating directly)
	for (int i=0;i<size;i++)
	{
		greyImage.red_data[i] = (char)((int)image.red_data[i]*0.3+image.green_data[i]*0.59+image.blue_data[i]*0.11);
		greyImage.green_data[i] = greyImage.red_data[i];
		greyImage.blue_data[i] = greyImage.red_data[i];
	}

    return greyImage;
}


int saveHistogram(char filename[], Image& image, int interval){

	int H = image.H, W = image.W;
	int size = H*W;
	int interval_limit = 256/interval;
	int interval_limit2 = 256/(256%interval);
	static int *histogram = new int [interval];
	Image grey(H, W);
	grey = turnToGreyscale(image);
	ofstream ofp;

    ofp.open(filename, ios::out | ios::binary);

    if (!ofp) 
    {
        cout << "Can't open file " << filename << endl;
        exit(1);
    }

	for (int i=0;i<interval;i++)
	{
		histogram[i] = 0;
	}

	for (int i=0;i<size;i++)
	{
		histogram[((int) grey.red_data[i]-(grey.red_data[i]/interval_limit2))/interval_limit]++;
	}

	for (int i=0;i<interval-1;i++)
	{
		ofp << histogram[i];
		ofp << " ";
	}
	ofp << histogram[interval-1];

	if (ofp.fail()) 
    {
        cout << "Can't write image " << filename << endl;
        exit(0);
    }

	ofp.close();

    return(1);
}


int maskImage(char filename[], char filenameMask[], Image& image)
{
	int H = image.H, W = image.W;
	int HMask, WMask;
	readImageHeader(filenameMask, HMask, WMask);
	Image mask(HMask,WMask);
	readImage(filenameMask, mask);

	ofstream ofp;

    ofp.open(filename, ios::out | ios::binary | ios::trunc);

    if (!ofp) 
    {
        cout << "Can't open file " << filename << endl;
        exit(1);
    }

	ofp << (char)(H%256);
	ofp << (char)(H/256);
	ofp << (char)(H/256*256);
	ofp << (char)(H/256*256*256);
	ofp << (char)(W%256);
	ofp << (char)(W/256);
	ofp << (char)(W/256*256);
	ofp << (char)(W/256*256*256);

	//No matter the size of the images, it is always done correctly from 0,0 with the use of these variables
	int Hlimit = HMask, Wlimit = WMask;
	if (H<=HMask)
	{
		Hlimit = H;
	}
	if (W<=WMask)
	{
		Wlimit = W;
	}

	// Experimental order for performance (may be worse than operating directly)
	int i,j;
	for (i=0;i<Hlimit;i++)
	{
		for (j=0;j<Wlimit;j++)
		{
			image.red_data[i*W+j]=image.red_data[i*W+j]*mask.red_data[i*WMask+j];
		}
	}
	for (i=0;i<Hlimit;i++)
	{
		for (j=0;j<Wlimit;j++)
		{
			image.green_data[i*W+j]=image.green_data[i*W+j]*mask.green_data[i*WMask+j];
		}
	}
	for (i=0;i<Hlimit;i++)
	{
		for (j=0;j<Wlimit;j++)
		{
			image.blue_data[i*W+j]=image.blue_data[i*W+j]*mask.blue_data[i*WMask+j];
		}
	}


	ofp.write( reinterpret_cast<char *>(image.red_data), (H*W)*sizeof(char));
    ofp.write( reinterpret_cast<char *>(image.green_data), (H*W)*sizeof(char));
    ofp.write( reinterpret_cast<char *>(image.blue_data), (H*W)*sizeof(char));

    if (ofp.fail()) 
    {
        cout << "Can't write image " << filename << endl;
        exit(0);
    }

    ofp.close();

    return(1);
}

int turnSelectionToGreyscale(char filename[], double radius, Image& image)
{
	int H = image.H, W = image.W;
	int size = H*W;
	int centerAH = H/2-1;
	int centerAW = W/2-1;
	int centerBH = H/2-1;
	int centerBW = W/2;
	int centerCH = H/2;
	int centerCW = W/2-1;
	int centerDH = H/2;
	int centerDW = W/2;
	/*cout << centerAH << " " << centerAW << endl;
	cout << centerBH << " " << centerBW << endl;
	cout << centerCH << " " << centerCW << endl;
	cout << centerDH << " " << centerDW << endl;*/
	int posH = 0;
	int posW = 0;
	double circle_limit = radius * radius;
	int circle[size];
	Image greyImage(H, W);

	if ((H*W)%2==0)
	{
		for (int i=0;i<size;i++)
		{
			posH = i/W;
			posW = i%W;
			int distanceH = centerDH-posH;
			int distanceW = centerDW-posW;
			if ((posW > centerAW)&&(posH > centerAH))
			{
				distanceH = centerAH-posH;
				distanceW = centerAW-posW;
			}
			if ((posW < centerBW)&&(posH > centerBH))
			{
				distanceH = centerBH-posH;
				distanceW = centerBW-posW;
			}
			if ((posW > centerCW)&&(posH < centerCH))
			{
				distanceH = centerCH-posH;
				distanceW = centerCW-posW;
			}
			distanceH = distanceH * distanceH;
			distanceW = distanceW * distanceW;
			circle[i] = 0;
			if ( (distanceH + distanceW) < circle_limit )
			{
				circle[i] = 1;
			}
		}
	} else
	{
		for (int i=0;i<size;i++)
		{
			posH = i/W;
			posW = i%W;
			int distanceH = centerDH-posH;
			int distanceW = centerDW-posW;
			distanceH = distanceH * distanceH;
			distanceW = distanceW * distanceW;
			circle[i] = 0;
			if ( (distanceH + distanceW) < circle_limit )
			{
				circle[i] = 1;
			}
		}
	}
	/*for (int i=0;i<H;i++)
	{
		for (int j=0;j<W;j++)
		{
			cout << circle[i*W+j] << " ";
		}
		cout << endl;
	}*/

	for (int i=0;i<size;i++)
	{
		if (circle[i] == 0)
		{
			greyImage.red_data[i] = (char)((int)image.red_data[i]*0.3+image.green_data[i]*0.59+image.blue_data[i]*0.11);
			greyImage.green_data[i] = greyImage.red_data[i];
			greyImage.blue_data[i] = greyImage.red_data[i];
		}
		else
		{
			greyImage.red_data[i] = (char)((int)image.red_data[i]);
			greyImage.green_data[i] = (char)((int)image.green_data[i]);
			greyImage.blue_data[i] = (char)((int)image.blue_data[i]);
		}
	}

	writeImage(filename, greyImage);

    return(1);
}

int rotate(char filename[], int angle, Image& image)
{
	int H = image.H, W = image.W;
	int size = H*W;
	float radians = (angle * 3.14159265) / 180.0;
	int centerAH = H/2-1;
	int centerAW = W/2-1;
	int centerBH = H/2-1;
	int centerBW = W/2;
	int centerCH = H/2;
	int centerCW = W/2-1;
	int centerDH = H/2;
	int centerDW = W/2;
	/*cout << centerAH << " " << centerAW << endl;
	cout << centerBH << " " << centerBW << endl;
	cout << centerCH << " " << centerCW << endl;
	cout << centerDH << " " << centerDW << endl;*/
	int posH = 0;
	int posW = 0;
	int pixelX[size];
	int pixelY[size];
	Image rotImage(H, W);

	if ((H*W)%2==1)
	{
		for (int i=0;i<size;i++)
		{
			posH = i/W;
			posW = i%W;
			int distanceH = centerDH-posH;
			int distanceW = centerDW-posW;
			if ((posW > centerAW)&&(posH > centerAH))
			{
				distanceH = centerAH-posH;
				distanceW = centerAW-posW;
				pixelX[i] = (int) ceil(centerAW+(cos(radians) * distanceW) - (sin(radians) * distanceH));
				pixelY[i] = (int) ceil(centerAH+(sin(radians) * distanceW) + (cos(radians) * distanceH));
			}
			if ((posW < centerBW)&&(posH > centerBH))
			{
				distanceH = centerBH-posH;
				distanceW = centerBW-posW;
				pixelX[i] = (int) ceil(centerBW+(cos(radians) * distanceW) - (sin(radians) * distanceH));
				pixelY[i] = (int) ceil(centerBH+(sin(radians) * distanceW) + (cos(radians) * distanceH));
			}
			if ((posW > centerCW)&&(posH < centerCH))
			{
				distanceH = centerCH-posH;
				distanceW = centerCW-posW;
				pixelX[i] = (int) ceil(centerCW+(cos(radians) * distanceW) - (sin(radians) * distanceH));
				pixelY[i] = (int) ceil(centerCH+(sin(radians) * distanceW) + (cos(radians) * distanceH));
			}
			if ((posW < centerDW)&&(posH < centerDH))
			{
				pixelX[i] = (int) ceil(centerDW+(cos(radians) * distanceW) - (sin(radians) * distanceH));
				pixelY[i] = (int) ceil(centerDH+(sin(radians) * distanceW) + (cos(radians) * distanceH));
			}
		}
	} else
	{
		for (int i=0;i<size;i++)
		{
			posH = i/W;
			posW = i%W;
			int distanceH = centerDH-posH;
			int distanceW = centerDW-posW;
			pixelX[i] = (int) ceil(centerDW+(cos(radians) * distanceW) - (sin(radians) * distanceH));
			pixelY[i] = (int) ceil(centerDH+(sin(radians) * distanceW) + (cos(radians) * distanceH));
		}
	}
	

	for (int i=0;i<size;i++)
	{
		posH = i/W;
		posW = i%W;
		int posX = pixelX[i];
		int posY = pixelY[i];
		if (0 <= posX && posX < W && 0 <= posY && posY < H)
		{
			rotImage.red_data[posY*W+posX] = image.red_data[i];
			rotImage.green_data[posY*W+posX] = image.green_data[i];
			rotImage.blue_data[posY*W+posX] = image.blue_data[i];
		}	
	}

	writeImage(filename, rotImage);

    return(1);
}
