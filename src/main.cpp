#include <iostream>
#include <fstream>
#include <iomanip>

#include "data_structure.h"
#include "gds_read.h"


using namespace std;

int main(int argc, char **argv){
	
	vector<int> layer_list;
	vector <gds_data> gds_datas;
	vector< vector<rect> > rectangles(2);
	
	
	//read gds file
    converter(&gds_datas,&layer_list, argv[1]);
    sort(layer_list.begin(), layer_list.end(), comp_layer);
	cout<<"converter finish"<<endl;
	
	for( int i=0;i<gds_datas.size();++i ){
		cout << gds_datas[i].layer << endl;
		for( int j=0;j<gds_datas[i].points_list.size();++j ){
			cout << gds_datas[i].points_list[j].x << " " << gds_datas[i].points_list[j].y << endl;
		}
	}
	getchar();
	
	
	cout << "good start !" << endl;
	
	return 0;
}