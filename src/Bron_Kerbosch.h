#ifndef BRON_KERBOSCH_H
#define BRON_KERBOSCH_H



#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;



/*struct input_data{
	double bottom_left_x,bottom_left_y,top_right_x,top_right_y;
	double port_x,port_y;
	int RAM_ID,word_width;	
	input_data();
};*/

/*struct RAM_datas{
	double rx,ry,lx,ly;
	int RAM_ID,word_width;
	RAM_datas();
};*/

struct vertex{
	vector <int> neightbor;
};

class bron_kerbosch{
	public:
		//bron_kerbosch();
		vector <int> Intersection_set(vector <int>,vector <int>,int);
		vector <int> Union_set(vector <int>,vector <int>,int);
		vector <int> Difference_set(vector <int>,vector <int>,int);
		void Initialize(vector <int>,vector <int>);
		bool Is_Duplicate(int,vector <int>);
		void start_find_MC(vector <int>,vector <vertex> *);
		void find_maximal_clique(vector <int>,vector <int>,vector <int>,vector <vertex> *);
		void cout_clique();
		//void get_all_clique_data(vector < vector < vector <int> > > *,vector <int>,vector <RAM_datas>);
		int get_word_width_num(int,vector <int>);
		void find_final_clique(vector < vector <int> > &, int);
		friend class clique_permutation;
		
		vector < vector <int> > maximal_clique; 
	private:
		vector <int> R,P,X;
		vector <int> used;
		vector <int> P_RAM_ID;
		
};

//void data_transform(vector <input_data>,vector < vector <RAM_datas> > *,double,vector <int> *);

//void P_graph_gen(vector <RAM_datas>,vector <vertex> *);

//bool wort_width_comp(RAM_datas,RAM_datas);






#endif