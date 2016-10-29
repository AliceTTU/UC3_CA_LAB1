#ifndef ARCfmt_H
#define ARCfmt_H

class Image
{
	public:
		Image();
		Image(int height, int width);
		Image(int height, int width, char data[]);
		void setPixelValue(int position, unsigned char red_value, unsigned char green_value, unsigned char blue_value);
		void setRedPixelValue(int position, unsigned char red_value);
		void setGreenPixelValue(int position, unsigned char green_value);
		void setBluePixelValue(int position, unsigned char blue_value);
		int *computeMaxMin();
		int H; // HEIGHT
		int W; // WIDTH
		unsigned char *red_data;
		unsigned char *green_data;
		unsigned char *blue_data;
};

#endif
