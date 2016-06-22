#ifndef _ACC_H
#define _ACC_H

#include <list>
#include <vector>
#include <string>
#include "marker.h"
using namespace std;

bool ACC(marker A, markerB){
	for(int i = 0; i < A.countPolygon(); i++){
		for(int j = 0; j < B.countPolygon(); j++){
			A.returnPolygon(i);
		}
	}
}

#endif
