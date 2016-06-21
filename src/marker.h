#ifndef _MARKER_H
#define _MARKER_H

#include <list>
#include <vector>
#include <string>
using namespace std;


class polygon {
	public:
		polygon(int x0, int x1, int y0, int y1) { xLeft = x0; xRight = x1; yDown = y0; yUp = y1; }
		int xLeft,xRight,yDown,yUp;
		
};

class rect_marker { //Rectangle in markers
	public:
		rect_marker(int x0, int x1, int y0, int y1) { xLeft = x0; xRight = x1; yDown = y0; yUp = y1; }
	private:
		int xLeft,xRight,yDown,yUp;
};

class marker { //Marker containing all polygon inside it
	public:
		marker(int w, int h, int X, int Y, int I) 
		{ 
		width = w; height = h; centerX = X; centerY = Y; ID = I;
		childPolygon.clear();
		}
		marker() { ID = 0;}
		void insertPolygon(int x0, int x1, int y0, int y1)
		{
			//cout << "Y0 " << y0 << endl;
			//cout << "BORDER " << (centerY-(height/2)) << endl;
			if(x0 < (centerX-(width/2)))
				x0 = 0;
			else
				x0-=(centerX-(width/2));
			if(y0 < (centerY-(height/2)))
				y0 = 0;
			else
				y0-=(centerY-(height/2));
			if(x1 > (centerX+(width/2)))
				x1 = width;
			else
				x1-=(centerX-(width/2));
			if(y1 > (centerY+(height/2)))
				y1 = height;
			else
				y1-=(centerY-(height/2));
			//cout << "Check: " << x0 << "/" << x1 << "/" << y0 << "/" << y1 << endl;
			childPolygon.push_back(rect_marker(x0,x1,y0,y1));
		}
		
	private:
		int width;	//This should be same among all 
		int height; //This should be same among all
		int centerX;
		int centerY;
		int ID;
		vector<rect_marker> childPolygon;
		
};

#endif
