#include <iostream>
#include <fstream>
#include <iomanip>

#include "data_structure.h"
#include "gds_read.h"
#include "RTree.h"
#include "marker.h"
#include "CC.h"

using namespace std;

void help_message() {
    cout << "usage: ./bin/ICCAD_2016 <input_gds_file> <constrained_method> <marker_width> <marker_height> <constrain_number>" << endl;
	cout << "for <constrained_method>, input either ECC or ACC to decide." << endl;
}

int main(int argc, char **argv){
	
	if(argc != 	6) {
       help_message();
       return 0;
    }
	vector<int> layer_list;
	vector <gds_data> gds_datas;
	vector <marker> markers;	//We store all markers here.
	vector <polygon> polygons;	//We store all markers here.
	vector< vector<rect> > rectangles(2);
	vector < vector<bool> > CCresult;
	int CCflag; // 0: ECC, 1: ACC 
	int markerHeight, markerWidth;
	double ACCconstraint;
	int ECCconstraint;
	
	markerWidth = atoi(argv[3]);
	markerHeight = atoi(argv[4]);
	if(strcmp(argv[2],"ECC") == 0){
		CCflag = 0;
		cout << "Choose ECC to clustering." << endl;
		ECCconstraint = atoi(argv[5]);
	}else if(strcmp(argv[2],"ACC") == 0){
		CCflag = 1;
		cout << "Choose ACC to clustering." << endl;
		if(atof(argv[5]) > 1){
			cout << "Input parameter must between 0 and 1 in ACC. Aborted." << endl;
			return 0;
		}else
			ACCconstraint =atof(argv[5]);
	}else{
		cout << "Please choose either ACC or ECC. Aborted." << endl;
		return 0;
	}
		
	//read gds file
    converter(&gds_datas,&layer_list, argv[1]);
    sort(layer_list.begin(), layer_list.end(), comp_layer);
	cout<<"converter finish"<<endl;
	
	RTree<int, int, 2, float> * tree = new RTree<int, int, 2, float>;
	int minp[2],maxp[2];
	
	cout << gds_datas.size() << endl;
	
	int polygonID = 0;
	for( int i=0;i<gds_datas.size();++i ){
		if(gds_datas[i].layer == 1000){
			minp[0] = gds_datas[i].points_list[0].x;
			minp[1] = gds_datas[i].points_list[0].y;
			maxp[0] = gds_datas[i].points_list[0].x;
			maxp[1] = gds_datas[i].points_list[0].y;
			for( int j=1;j<gds_datas[i].points_list.size();++j ){
				if(minp[0] > gds_datas[i].points_list[j].x)
					minp[0] = gds_datas[i].points_list[j].x;
				if(minp[1] > gds_datas[i].points_list[j].y)
					minp[1] = gds_datas[i].points_list[j].y;
				if(maxp[0] < gds_datas[i].points_list[j].x)
					maxp[0] = gds_datas[i].points_list[j].x;
				if(maxp[1] < gds_datas[i].points_list[j].y)
					maxp[1] = gds_datas[i].points_list[j].y;
			}
			cout << "Polygon " << polygonID << "@ " << minp[0] << "/" << minp[1] << "/" << maxp[0] << "/" << maxp[1] << endl; 
			tree->Insert(minp, maxp, polygonID);
			polygons.push_back(polygon(minp[0],maxp[0],minp[1],maxp[1]));
			polygonID++;
		}
	}
	
	int markerID = 0;
	vector <int> t_list;
	for( int i=0;i<gds_datas.size();++i ){
		if(gds_datas[i].layer == 10000){
			minp[0] = gds_datas[i].points_list[0].x;
			minp[1] = gds_datas[i].points_list[0].y;
			maxp[0] = gds_datas[i].points_list[0].x;
			maxp[1] = gds_datas[i].points_list[0].y;
			for( int j=1;j<gds_datas[i].points_list.size();++j ){
				if(minp[0] > gds_datas[i].points_list[j].x)
					minp[0] = gds_datas[i].points_list[j].x;
				if(minp[1] > gds_datas[i].points_list[j].y)
					minp[1] = gds_datas[i].points_list[j].y;
				if(maxp[0] < gds_datas[i].points_list[j].x)
					maxp[0] = gds_datas[i].points_list[j].x;
				if(maxp[1] < gds_datas[i].points_list[j].y)
					maxp[1] = gds_datas[i].points_list[j].y;
			}
			int centerX = (minp[0]+maxp[0])/2;
			int centerY = (minp[1]+maxp[1])/2;
			minp[0] = centerX-markerWidth/2;
			minp[1] = centerY-markerHeight/2;
			maxp[0] = centerX+markerWidth/2;
			maxp[1] = centerY+markerHeight/2;
			//cout << "Marker " << i << "@ " << minp[0] << "/" << minp[1] << "/" << maxp[0] << "/" << maxp[1] << endl;
			markers.push_back(marker(markerWidth,markerHeight,centerX,centerY,markerID));
			tree->Search(minp, maxp, &t_list);
			//cout << "Element " << t_list.size() << endl;
			for( int j=0;j<t_list.size();++j ){
				markers[markerID].insertPolygon(polygons[t_list[j]].xLeft,polygons[t_list[j]].xRight,polygons[t_list[j]].yDown,polygons[t_list[j]].yUp); 
				cout << t_list[j] << endl;
			}
			//cout << markers[markerID].areaPolygon() << endl;
			t_list.clear();
			markerID++;
		}
	}
	
	CCresult.resize(markers.size());
	for(int k=0; k < markers.size(); k++){
		for(int l=0; l < markers.size(); l++){
			cout << k << " with " << l << endl;
			if(CCflag == 1)
				CCresult[k].push_back(ACC(markers[k],markers[l],ACCconstraint));
			else
				CCresult[k].push_back(ECC(markers[k],markers[l],ECCconstraint));
		}
	}
	
	// Result Checker
	for(int k=0; k < markers.size(); k++){
		for(int l=0; l < markers.size(); l++){
			cout << CCresult[k][l] << " ";
		}
		cout << endl;
	}
	
	
	return 0;
}
