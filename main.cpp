#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>

#include "ARCfmt.h"
#include "ARCfmt.cpp"

using namespace std;

int readImageHeader(char[], int&, int&);
int readImage(char[], Image&);
int writeImage(char[], Image&);
int saveMaxMin(char[], Image&);
Image turnToGreyscale(Image&);
int saveHistogram(char[], Image&, int);

int main(int argc, char *argv[])
{
	int H, W;
	int temp_position;
	int choice = 1;
	int temp_red;
	int temp_green;
	int temp_blue;


	readImageHeader(argv[1], H, W);

    	// Allocate memory for the image
	Image image(H,W);

	// Read image
	readImage(argv[1], image);
    
	// Print menu
	while(0 <= choice && choice < 7)
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
        	else if(argc == 3) // One input image and one output file
		{
			cout << "[5] Write file with maximum and minimum color values" << endl;
		}
		else if(argc == 4) // One input image one output file and one integer
		{
			cout << "[6] Write file with histogram" << endl;
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
			default:
                		cout << "Unexpected value: closing the program." << endl;
                		break;
        	}
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
	cout << "read1" << endl;
	cout << "firstInteger: " << firstInteger << endl;
    	ifp.read(secondInteger, 4);
	cout << "read2" << endl;
	cout << "secondInteger: " << secondInteger << endl;
	//LITTLE ENDIAN
	H = (int) firstInteger[0]+(int) 256*firstInteger[1]+
		 (int) 256*256*firstInteger[2]+(int) 256*256*256*firstInteger[3];
	W = (int) secondInteger[0]+(int) 256*secondInteger[1]+
		(int) 256*256*secondInteger[2]+(int) 256*256*256*secondInteger[3];
	cout << "operate" << endl;
	cout << "H: " << H << endl;
	cout << "W: " << W << endl;

    	ifp.close();
	cout << "close" << endl;
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
		greyImage.green_data[i] = (char) ((int)image.green_data[i]*0.59)+image.red_data[i];
	}
	for (int i=0;i<size;i++)
	{
		greyImage.blue_data[i] = (char) ((int)image.blue_data[i]*0.11)+image.green_data[i];
		greyImage.green_data[i] = image.blue_data[i];
	}
	for (int i=0;i<size;i++)
	{
		greyImage.red_data[i] = image.blue_data[i];
	}

    	return greyImage;
}

//TODO
int saveHistogram(char filename[], Image& image, int interval){

	/*int H = image.H, W = image.W;
	int size = H*W;
	int interval_limit = 256/interval;
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
		cout << (int) grey.red_data[i] << " ";
		cout << ((int) grey.red_data[i])/interval_limit << " ";
		if (((int) grey.red_data[i])/interval_limit > interval_limit*interval)
		{
			histogram[interval-1]++;
			cout << histogram[interval-1] << endl;
		}
		else
		{
			histogram[((int) grey.red_data[i])/interval_limit]++;
			cout << histogram[((int) grey.red_data[i])/interval_limit] << endl;
		}
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

	ofp.close();*/

    	return(1);
}
