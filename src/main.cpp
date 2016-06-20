#include <iostream>
#include <fstream>
#include <iomanip>

#include "data_structure.h"
#include "gds_read.h"
#include "RTree.h"

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
		//cout << gds_datas[i].layer << endl;
		if(gds_datas[i].points_list.size() > 4){
			for( int j=0;j<gds_datas[i].points_list.size();++j ){
				cout << gds_datas[i].points_list[j].x << " " << gds_datas[i].points_list[j].y << endl;
			}
			getchar();
		}
		
	}
	
	RTree<int, int, 2, float> * tree = new RTree<int, int, 2, float>;
	int minp[2],maxp[2];
	minp[0] = 0;
	minp[1] = 0;
	maxp[0] = 100;
	maxp[1] = 100;
	tree->Insert(minp, maxp, 1);
	
	vector <int> t_list;
	int sminp[2],smaxp[2];
	minp[0] = 0;
	minp[1] = 0;
	maxp[0] = 0;
	maxp[1] = 0;
	tree->Search(sminp, smaxp, &t_list);
	
	for( int i=0;i<t_list.size();++i ){
		cout << t_list[i] << endl;
	}
	 
	
	getchar();
	
	
	cout << "good start !" << endl;
	
	return 0;
}