#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>

#include "data_structure.h"
#include "gds_read.h"
#include "RTree.h"
#include "marker.h"
#include "CC.h"
#include "Bron_Kerbosch.h"

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
	vector <polygon> polygons;	//We store all polygons here.
	vector< vector<rect> > rectangles(2);
	vector < vector<bool> > CCresult;	//We store result before clustering here. 
	int CCflag; // 0: ECC, 1: ACC 
	
	//User defined parameter
	int markerHeight, markerWidth;
	double ACCconstraint;
	int ECCconstraint;
	
	//Read in parameter
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
	
	int polygonID = 0;
	for( int i=0;i<gds_datas.size();++i ){	// Read polygons
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
			tree->Insert(minp, maxp, polygonID);
			polygons.push_back(polygon(minp[0],maxp[0],minp[1],maxp[1]));
			polygonID++;
		}
	}
	
	int markerID = 0;
	vector <int> t_list;
	for( int i=0;i<gds_datas.size();++i ){	// Read markers
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
			markers.push_back(marker(markerWidth,markerHeight,centerX,centerY,markerID));
			tree->Search(minp, maxp, &t_list);
			for( int j=0;j<t_list.size();++j ){
				markers[markerID].insertPolygon(polygons[t_list[j]].xLeft,polygons[t_list[j]].xRight,polygons[t_list[j]].yDown,polygons[t_list[j]].yUp); 
			}
			t_list.clear();
			markerID++;
		}
	}
	
	CCresult.resize(markers.size());
	for(int k=0; k < markers.size(); k++){
		for(int l=0; l < markers.size(); l++){
			if(CCflag == 1)
				CCresult[k].push_back(ACC(markers[k],markers[l],ACCconstraint));
			else
				CCresult[k].push_back(ECC(markers[k],markers[l],ECCconstraint));
		}
	}
	
	// Result Checker
	vector <int> start_P(markers.size());
	vector <vertex> v(markers.size());
	
	vector < vector <int> > final_clique;
	
	//vector <clique *> v_clique;
	bron_kerbosch bk;
	for( int i=0;i<markers.size();i++ ){
		start_P[i] = i;
	}
	
	for(int k=0; k < markers.size(); k++){
		for(int l=0; l < markers.size(); l++){
			cout << CCresult[k][l] << " ";
			if(CCresult[k][l] && k != l){
				v[k].neightbor.push_back(l);
			}
		}
		cout << endl;
	}
	bk.start_find_MC(start_P, &v);
	bk.cout_clique();
	bk.find_final_clique(final_clique, markers.size());
	cout << markers.size() << " " << bk.maximal_clique.size() << endl;
	
	
	
	
	return 0;
}
