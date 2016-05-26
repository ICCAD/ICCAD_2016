#ifndef __data_structure__
#define __data_structure__

#include <vector>
#include <deque>
#include <list>

using namespace std;

struct points{
    int x,y,poly_num;
};

struct rect{
    points minp,maxp;
};

struct rectwitharea{
    points minp,maxp;
    double area;
};

struct areadata{
    vector< vector<double> > density; //original
    vector< vector<double> > goal;
    vector< vector<double> > linewidth;
};

struct gds_data {
    int layer;
	bool cutfun;
    vector<points> points_list;
};

struct maxdata{
	int colmax,rowmax;
    double densitymax;
};

struct vase{
	int rect_num,layer,state;
	double a;
};

struct in_rect{
	int rect_num;
};

struct window_rect{
	int r_id,priority;
	double area;
	window_rect(){
		priority = 0;
	}
};

struct PTR_vase{
	int row;
	points point;
};


#endif