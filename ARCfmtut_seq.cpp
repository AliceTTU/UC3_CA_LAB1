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
int turnSelectionToGreyscale(char[], int, Image&);
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
			turnSelectionToGreyscale(argv[filename], atoi(argv[radius]), image);
			break;
			
	}
	return 0;
	/*readImageHeader(argv[1], H, W);

    	// Allocate memory for the image
	Image image(H,W);

	// Read image
	readImage(argv[1], image);
    
	// Print menu
	while(0 <= choice && choice < 9)
	{
        	cout << "Option menu:" << endl;
		cout << "[0] Quit" << endl;
        
		if(argc == 2) // One input image
		{
			cout << "[1] Set the red value of a pixel" << endl;
			cout << "[2] Set the green value of a pixel" << endl;		
			cout << "[3] Set the blue value of a pixel" << endl;
			cout << "[4] Set the value of a pixel" << endl << endl;
        	}
        	else if(argc == 3) 
		{
			cout << "[5] Write file with maximum and minimum color values" << endl;// One input image and one output file
		}
		else if(argc == 4)
		{
			cout << "[6] Write file with histogram" << endl; // One input image one output file and one integer
			cout << "[7] Apply mask file" << endl; // Two input images one output file
			cout << "[8] Apply greyscale selection" << endl;// One input image one output file and one integer
		}
		else
		{
			cout << "ERROR" << endl;
		}
        	cin >> choice;
        
        	switch(choice)
        	{
			case 0: // [0] Quit
				cout << "Closing the program." << endl;
				return 0;
			case 1: // [1] Set the red value of a pixel
				cout << "Write the position of the pixel:" << endl;
                		cin >> temp_position;				
				cout << "Write the red value of the pixel:" << endl;
                		cin >> temp_red;
                		image.setRedPixelValue(temp_position, temp_red);
				temp_position = 0;
				temp_red = 0;
                		writeImage(argv[1], image);
                		break;
			case 2: // [2] Set the green value of a pixel
				cout << "Write the position of the pixel:" << endl;
                		cin >> temp_position;				
				cout << "Write the green value of the pixel:" << endl;
                		cin >> temp_green;
                		image.setRedPixelValue(temp_position, temp_green);
				temp_position = 0;
				temp_green = 0;
                		writeImage(argv[1], image);
                		break;
			case 3: // [3] Set the blue value of a pixel
				cout << "Write the position of the pixel:" << endl;
                		cin >> temp_position;				
				cout << "Write the blue value of the pixel:" << endl;
                		cin >> temp_blue;
                		image.setRedPixelValue(temp_position, temp_blue);
				temp_position = 0;
				temp_blue = 0;
                		writeImage(argv[1], image);
                		break;
			case 4: // [4] Set the value of a pixel
				cout << "Write the position of the pixel:" << endl;
                		cin >> temp_position;
				cout << "Write the red value of the pixel:" << endl;
                		cin >> temp_red;
				cout << "Write the green value of the pixel:" << endl;
                		cin >> temp_green;
				cout << "Write the blue value of the pixel:" << endl;
                		cin >> temp_blue;
                		image.setPixelValue(temp_position, temp_red, temp_green, temp_blue);
				temp_position = 0;
				temp_red = 0;
				temp_green = 0;
				temp_blue = 0;
                		writeImage(argv[1], image);
                		break;
			case 5: // [5] Write file with maximum and minimum color values
				saveMaxMin(argv[2], image);
				cout << "Data written in file " << argv[2] << endl;
				break;
			case 6: // [6] Write file with histogram
				saveHistogram(argv[2], image, atoi(argv[3]));
				cout << "Histogram written in file " << argv[2] << endl;
				break;
			case 7: // [7] Apply mask file
				maskImage(argv[3], argv[2], image);
				cout << "Masked image written in file " << argv[3] << endl;
				break;
			case 8: // [8] Apply grayscale selection
				turnSelectionToGreyscale(argv[2], atoi(argv[3]), image);
				cout << "Greyscale selection image written in file " << argv[2] << endl;
				break;
			default:
                		cout << "Unexpected value: closing the program." << endl;
                		break;
        	}
    	}

    	return 0;*/
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
		greyImage.red_data[i] = (char)((int)image.red_data[i]*0.3);
	}
	for (int i=0;i<size;i++)
	{
		greyImage.green_data[i] = (char) ((int)image.green_data[i]*0.59)+greyImage.red_data[i];
	}
	for (int i=0;i<size;i++)
	{
		greyImage.blue_data[i] = (char) ((int)image.blue_data[i]*0.11)+greyImage.green_data[i];
		greyImage.green_data[i] = greyImage.blue_data[i];
	}
	for (int i=0;i<size;i++)
	{
		greyImage.red_data[i] = greyImage.blue_data[i];
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

int turnSelectionToGreyscale(char filename[], int radius, Image& image)
{
	cout << "radius " << radius;
	int H = image.H, W = image.W;
		/*cout << centerAH << " " << centerAW << endl;
	cout << centerBH << " " << centerBW << endl;
	cout << centerCH << " " << centerCW << endl;
	cout << centerDH << " " << centerDW << endl;*/

	int center[2];
	int yL,xL,xF,yF, xLreset;
	
	center[0] = W/2;
	center[1] = H/2;
	//problem is if the radius extends outside past the whole array
	if((radius + center[0]) > W){
		xLreset = 0;
		xL = 0;
		xF = image.W;
	} else {
		xLreset = center[0] - radius;
		xL = center[0] - radius;
		xF = center[0] + radius;
	}
	if((radius + center[1]) > H) {
		yL = 0;
		yF = image.H;
	} else {
		yL = center[1] - radius;
		yF = center[1] + radius;
	}



	Image greyImage(H, W);
	greyImage.red_data = image.red_data;
	greyImage.blue_data = image.blue_data;
	greyImage.green_data = image.green_data;
	while(yL < yF)
	{
		cout << "iterating through row: " << yL  << "\n";
		while(xL < xF)
		{
			if(((xL - center[0])*(xL - center[0]) + (yL - center[1])*(yL - center[1])) > (radius*radius)){
				image.red_data[xL + yL*W] = floor((double)image.red_data[xL + (yL)*W]  * .3);
				image.blue_data[xL + yL*W] = floor((double)image.blue_data[xL + (yL)*W]  * .11);
				image.green_data[xL + yL*W] = floor((double)image.green_data[xL + (yL)*W]  * .59);
				cout << "made into radius change val\n";
			}
			cout << "xL = " << xL << "\n";
			xL++;
		}
		xL =  xLreset;
		yL++;
	}	
	
	/*for (int i=0;i<H;i++)
	{
		for (int j=0;j<W;j++)
		{
			cout << circle[i*W+j] << " ";
		}
		cout << endl;
	}*/





	writeImage(filename, image);

    return(1);
}

int rotate(char filename[], int deags, Image& image)
{
	int H = image.H, W = image.W;
	int size = H*W;

	Image result(H, W);

	double pi = 3.1415926535897;
	double radians = pi*(double)deags/180;
	int center[2];
	cout << "Radians: " << radians << endl;
	center[0] = image.W/2;
	center[1] = image.H/2;

	int xi, yi, xf, yf;
	// result.red_data = image.red_data;
	// result.blue_data = image.blue_data;
	// result.green_data = result.green_data;

	for (int k = 0; k < size; k++)
    {
    	xi = k % image.W - W/2;
    	yi = k / image.H - H/2;

    	xf = cos(radians) * xi - sin(radians) * yi;
    	yf = sin(radians) * xi + cos(radians) * yi;

    	// result.red_data[k]  = image.red_data[xf+center[0] + (yf + center[1])*image.W]; 
    	// result.blue_data[k]  = image.blue_data[xf+center[0] + (yf + center[1])*image.W]; 
    	// result.green_data[k]  = image.green_data[xf+center[0] + (yf + center[1])*image.W]; 

    	result.red_data[xf+center[0] + (yf + center[1])*image.W]  = image.red_data[k]; 
    	result.blue_data[xf+center[0] + (yf + center[1])*image.W]  = image.blue_data[k]; 
    	result.green_data[xf+center[0] + (yf + center[1])*image.W]  = image.green_data[k]]; 

    }  

	writeImage(filename, result);

    	return(1);
}
