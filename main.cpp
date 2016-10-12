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
	while(0 <= choice && choice < 5)
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
			default:
                		cout << "Unexpected value: closing the program." << endl;
                		break;
        	}
    	}

    	return 0;
}


int readImage(char filename[], Image& image)
{
    	char *charImage;
	//unsigned char header;
    	ifstream ifp;

   	ifp.open(filename, ios::in | ios::binary);

    	if (!ifp)
    	{
        	cout << "Error reading file " << filename << endl;
        	exit(1);
    	}

    	//ifp.getline(header, 1);

    	charImage = (char *) new char [image.H*image.W*3];

    	ifp.read( reinterpret_cast<char *>(image.red_data), (image.H*image.W)*sizeof(char));
	ifp.read( reinterpret_cast<char *>(image.green_data), (image.H*image.W)*sizeof(char));
	ifp.read( reinterpret_cast<char *>(image.blue_data), (image.H*image.W)*sizeof(char));

    	ifp.close();

    	delete [] charImage;

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
	//If a thread is killed, these next two lines are the cause TODO TODO TODO TODO
	H = (unsigned char) firstInteger[3]+(unsigned char) 256*firstInteger[2]+
		 (unsigned char) 256*256*firstInteger[1]+(unsigned char) 256*256*256*firstInteger[0];
	W = (unsigned char) secondInteger[3]+(unsigned char) 256*secondInteger[2]+
		(unsigned char) 256*256*secondInteger[1]+(unsigned char) 256*256*256*secondInteger[0];
	cout << "H: " << H << endl;
	cout << "W: " << W << endl;
	H = 2;
	W = 2;
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

    	ofp.open(filename, ios::out | ios::binary);

    	if (!ofp) 
    	{
        	cout << "Can't open file " << filename << endl;
        	exit(1);
    	}

    	ofp << (unsigned char) H/256*256*256;
	ofp << (unsigned char) H/256*256;
	ofp << (unsigned char) H/256;
    	ofp << (unsigned char) H % 256;
    	ofp << (unsigned char) W/256*256*256;
	ofp << (unsigned char) W/256*256;
	ofp << (unsigned char) W/256;
    	ofp << (unsigned char) W % 256;

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
