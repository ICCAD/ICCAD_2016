#ifndef _ACC_H
#define _ACC_H

#include <list>
#include <vector>
#include <string>
#include "marker.h"
using namespace std;

bool ACC(marker A, marker B, double e){
	double Intersection = 0;
	for(int i = 0; i < A.countPolygon(); i++){
		for(int j = 0; j < B.countPolygon(); j++){ //Calculate area of intersection by idiot method
			//cout << "A " << i << " :"  << A.returnPolygon(i).xLeft << "/" << A.returnPolygon(i).xRight << "/" << A.returnPolygon(i).yDown << "/" << A.returnPolygon(i).yUp << endl;  
			//cout << "B " << j << " :"  << B.returnPolygon(j).xLeft << "/" << B.returnPolygon(j).xRight << "/" << B.returnPolygon(j).yDown << "/" << B.returnPolygon(j).yUp << endl;
			if((A.returnPolygon(i).xLeft <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xLeft >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yUp <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yUp >= B.returnPolygon(j).yDown)){
				//cout << "+" << ((B.returnPolygon(j).xRight - A.returnPolygon(i).xLeft)*(A.returnPolygon(i).yUp - B.returnPolygon(j).yDown)) << endl;
				Intersection+= ((B.returnPolygon(j).xRight - A.returnPolygon(i).xLeft)*(A.returnPolygon(i).yUp - B.returnPolygon(j).yDown));
			}else if((A.returnPolygon(i).xRight <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xRight >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yUp <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yUp >= B.returnPolygon(j).yDown)){
				//cout << "+" << ((A.returnPolygon(i).xRight - B.returnPolygon(j).xLeft)*(A.returnPolygon(i).yUp - B.returnPolygon(j).yDown)) << endl;
				Intersection+= ((A.returnPolygon(i).xRight - B.returnPolygon(j).xLeft)*(A.returnPolygon(i).yUp - B.returnPolygon(j).yDown));
			}else if((A.returnPolygon(i).xLeft <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xLeft >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yDown <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yDown >= B.returnPolygon(j).yDown)){
				//cout << "+" << ((B.returnPolygon(j).xRight - A.returnPolygon(i).xLeft)*(B.returnPolygon(j).yUp - A.returnPolygon(i).yDown)) << endl;
				Intersection+= ((B.returnPolygon(j).xRight - A.returnPolygon(i).xLeft)*(B.returnPolygon(j).yUp - A.returnPolygon(i).yDown));
			}else if((A.returnPolygon(i).xRight <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xRight >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yDown <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yDown >= B.returnPolygon(j).yDown)){
				//cout << "+" << ((A.returnPolygon(i).xRight - B.returnPolygon(j).xLeft)*(B.returnPolygon(j).yUp - A.returnPolygon(i).yDown)) << endl;
				Intersection+= ((A.returnPolygon(i).xRight - B.returnPolygon(j).xLeft)*(B.returnPolygon(j).yUp - A.returnPolygon(i).yDown));
			}else;
		}
	}
	//cout << "Intersection: " << Intersection << endl;
	double XOR = double(A.areaPolygon()) + double(B.areaPolygon()) - 2*Intersection;
	//cout << "XOR: " << XOR << endl;
	if(((double(A.areaPolygon())-XOR)/double(A.areaPolygon())) >= e)
		return true;
	else if(((double(B.areaPolygon())-XOR)/double(B.areaPolygon())) >= e)
		return true;
	else
		return false;
}

#endif
