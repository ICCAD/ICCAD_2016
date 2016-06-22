#ifndef _CC_H
#define _CC_H

#include <list>
#include <vector>
#include <string>
#include "marker.h"
using namespace std;

bool ACC(marker A, marker B, double e){
	double Intersection = 0;
	for(int i = 0; i < A.countPolygon(); i++){
		for(int j = 0; j < B.countPolygon(); j++){ //Calculate area of intersection by idiot method
			if((A.returnPolygon(i).xLeft <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xLeft >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yUp <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yUp >= B.returnPolygon(j).yDown)){
				Intersection+= ((B.returnPolygon(j).xRight - A.returnPolygon(i).xLeft)*(A.returnPolygon(i).yUp - B.returnPolygon(j).yDown));
			}else if((A.returnPolygon(i).xRight <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xRight >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yUp <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yUp >= B.returnPolygon(j).yDown)){
				Intersection+= ((A.returnPolygon(i).xRight - B.returnPolygon(j).xLeft)*(A.returnPolygon(i).yUp - B.returnPolygon(j).yDown));
			}else if((A.returnPolygon(i).xLeft <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xLeft >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yDown <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yDown >= B.returnPolygon(j).yDown)){
				Intersection+= ((B.returnPolygon(j).xRight - A.returnPolygon(i).xLeft)*(B.returnPolygon(j).yUp - A.returnPolygon(i).yDown));
			}else if((A.returnPolygon(i).xRight <= B.returnPolygon(j).xRight)&&(A.returnPolygon(i).xRight >= B.returnPolygon(j).xLeft)&&
			(A.returnPolygon(i).yDown <= B.returnPolygon(j).yUp)&&(A.returnPolygon(i).yDown >= B.returnPolygon(j).yDown)){
				Intersection+= ((A.returnPolygon(i).xRight - B.returnPolygon(j).xLeft)*(B.returnPolygon(j).yUp - A.returnPolygon(i).yDown));
			}else;
		}
	}
	cout << "Intersection: " << Intersection << endl;
	double XOR = double(A.areaPolygon()) + double(B.areaPolygon()) - 2*Intersection;
	cout << "XOR: " << XOR << endl;
	if(((double(A.areaPolygon())-XOR)/double(A.areaPolygon())) >= e)
		return true;
	else if(((double(B.areaPolygon())-XOR)/double(B.areaPolygon())) >= e)
		return true;
	else
		return false;
}

bool ECC(marker markerA,marker markerB,int constraint){
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
			if(polygons[i].yDown < it->y0 &&  it->y0 <polygons[i].yUp){
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
			if(polygons[i].xLeft < it->x0 &&  it->x0 <polygons[i].xRight){
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
		if(it->x0 == it->x1 && it->y0 == it->y1)
			linesV.erase(it);
	}
	for(std::list<Line>::iterator it = linesH.begin() ; it!=linesH.end(); ++it){
		if(it->x0 == it->x1 && it->y0 == it->y1)
			linesH.erase(it);
	}

	if(linesH.size()==0 && linesH.size()==0)
		return true;
	else 
		return false;
}


#endif
