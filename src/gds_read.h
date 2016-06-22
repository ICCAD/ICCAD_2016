#ifndef __gds_read__
#define __gds_read__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdio>

#include "data_structure.h"

using namespace std;


void converter (vector< gds_data > *gds_datas, vector<int> *,const char *);

void writegds(const char *,vector<rect>,int layer, int , int , int);

int Int_Conv(int a, int num);

bool comp_points(points aa, points bb);

bool comp_pointsyy(points aa, points bb);

bool comp_layer(int aa, int bb);

void writegds(const char *,vector<rect>,int layer, int , int , int);





#endif
