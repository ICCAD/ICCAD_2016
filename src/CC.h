#ifndef _CC_H
#define _CC_H

#include <list>
#include <vector>
#include <string>
#include "marker.h"
using namespace std;

bool ACC(marker A, marker B, double e){	// Perform ACC
	double Intersection = 0;
	for(int i = 0; i < A.countPolygon(); i++){
		for(int j = 0; j < B.countPolygon(); j++){ //Calculate area of intersection by idiot method
			int rLeft, rRight, rUp, rDown;
			if(A.returnPolygon(i).xLeft > B.returnPolygon(j).xLeft)
				rLeft = A.returnPolygon(i).xLeft;
			else
				rLeft = B.returnPolygon(j).xLeft;
			if(A.returnPolygon(i).xRight < B.returnPolygon(j).xRight)
				rRight = A.returnPolygon(i).xRight;
			else
				rRight = B.returnPolygon(j).xRight;
			if(A.returnPolygon(i).yUp < B.returnPolygon(j).yUp)
				rUp = A.returnPolygon(i).yUp;
			else
				rUp = B.returnPolygon(j).yUp;
			if(A.returnPolygon(i).yDown > B.returnPolygon(j).yDown)
				rDown = A.returnPolygon(i).yDown;
			else
				rDown = B.returnPolygon(j).yDown;
			if(rLeft < rRight && rUp > rDown){
				Intersection+= ((rRight - rLeft)*(rUp - rDown));
				//cout << ((rRight - rLeft)*(rUp - rDown)) << endl;
			}
		}
	}
	double XOR = double(A.areaPolygon()) + double(B.areaPolygon()) - 2*Intersection;
	if(((double(A.areaPolygon())-XOR)/double(A.areaPolygon())) >= e)
		return true;
	else if(((double(B.areaPolygon())-XOR)/double(B.areaPolygon())) >= e)
		return true;
	else
		return false;
}

bool ECC(marker markerA,marker markerB,int constraint){	// Perform ECC
	list<Line> linesH;
	list<Line> linesV;
	list<Line> linesB;
	vector<polygon> polygons;

	
	//lines = polygonToLine(markerA);
	for(int i = 0 ; i < markerA.countPolygon() ; i++ ){
		polygon currentpolygon = markerA.returnPolygon(i);
		linesH.push_back(Line(currentpolygon.xLeft,currentpolygon.yDown,currentpolygon.xRight,currentpolygon.yDown));
		linesH.push_back(Line(currentpolygon.xLeft,currentpolygon.yUp,currentpolygon.xRight,currentpolygon.yUp));
		linesV.push_back(Line(currentpolygon.xLeft,currentpolygon.yDown,currentpolygon.xLeft,currentpolygon.yUp));
		linesV.push_back(Line(currentpolygon.xRight,currentpolygon.yDown,currentpolygon.xRight,currentpolygon.yUp));

	}
	for(int i = 0 ; i < markerB.countPolygon() ; i++ ){
		polygon currentpolygon = markerB.returnPolygon(i);
		linesB.push_back(Line(currentpolygon.xLeft,currentpolygon.yDown,currentpolygon.xRight,currentpolygon.yDown));
		linesB.push_back(Line(currentpolygon.xLeft,currentpolygon.yUp,currentpolygon.xRight,currentpolygon.yUp));
		linesB.push_back(Line(currentpolygon.xLeft,currentpolygon.yDown,currentpolygon.xLeft,currentpolygon.yUp));
		linesB.push_back(Line(currentpolygon.xRight,currentpolygon.yDown,currentpolygon.xRight,currentpolygon.yUp));

	}
	//change line

	//polygons = lineToPolygon(polygonToLine(markerB));
	for(std::list<Line>::iterator it = linesB.begin() ; it!=linesB.end(); ++it){
		polygons.push_back(polygon(it->x0 - constraint , it->x1 + constraint , it->y0 - constraint , it->y1 + constraint));
	}
	for(int i = 0 ; i < polygons.size(); i++){
		for(std::list<Line>::iterator it = linesH.begin() ; it!=linesH.end(); ++it){
			if(polygons[i].yDown <= it->y0 &&  it->y0 <= polygons[i].yUp){
				if(polygons[i].xLeft < it->x0 && polygons[i].xRight > it->x1){
					linesH.erase(it);
					--it;
				}
				else if(polygons[i].xLeft > it->x0 && polygons[i].xRight < it->x1){
					it->x1 = polygons[i].xLeft;
					linesH.push_back(Line(polygons[i].xRight,it->y0,it->x1,it->y0));
				}
				else if(polygons[i].xLeft > it->x0 && polygons[i].xRight > it->x1 && polygons[i].xLeft < it->x1){
					it->x1 = polygons[i].xLeft;
				}
				else if(polygons[i].xLeft < it->x0 && polygons[i].xRight > it->x0 && polygons[i].xRight < it->x1){
					it->x0 = polygons[i].xRight;
				}
			}
			
		}

		for(std::list<Line>::iterator it = linesV.begin() ; it!=linesV.end(); ++it){
			if(polygons[i].xLeft <= it->x0 &&  it->x0 <= polygons[i].xRight){
				if(polygons[i].yDown < it->y0 && polygons[i].yUp > it->y1){
					linesV.erase(it);
					--it;
				}
				else if(polygons[i].yDown > it->y0 && polygons[i].yUp < it->y1){
					it->y1 = polygons[i].yDown;
					linesH.push_back(Line(it->x0,polygons[i].yUp,it->x0,it->y1));
				}
				else if(polygons[i].yDown > it->y0 && polygons[i].yUp > it->y1 && polygons[i].yDown < it->y1){
					it->y1 = polygons[i].yDown;
				}
				else if(polygons[i].yDown < it->y0 && polygons[i].yUp > it->y0 && polygons[i].yUp < it->y1){
					it->y0 = polygons[i].yUp;
				}
			}
		}
	}
	for(std::list<Line>::iterator it = linesV.begin() ; it!=linesV.end(); ++it){
		if(it->x0 == it->x1 && it->y0 == it->y1){
			linesH.erase(it);
			--it;
		}
			
	}
	for(std::list<Line>::iterator it = linesH.begin() ; it!=linesH.end(); ++it){
		if(it->x0 == it->x1 && it->y0 == it->y1){
			linesV.erase(it);
			--it;
		}
			
	}

	if(linesH.size()==0 && linesH.size()==0)
		return true;
	else 
		return false;
}


#endif
