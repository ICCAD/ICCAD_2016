#include "Bron_Kerbosch.h"

/*input_data::input_data(){
	bottom_left_x=0;
	bottom_left_y=0;
	top_right_x=0;
	top_right_y=0;
	port_x=0;
	port_y=0;
	word_width=0;
}

RAM_datas::RAM_datas(){
	rx=0;
	ry=0;
	lx=0;
	ly=0;
	word_width=0;
	RAM_ID=0;
}*/

void bron_kerbosch::Initialize(vector <int> n,vector <int> RAM_ID){
	for( int i=0;i<n.size();i++ ){
		P.push_back(n[i]);
		P_RAM_ID.push_back(RAM_ID[i]);
	}
}

vector <int> bron_kerbosch::Intersection_set(vector <int> a,vector <int> b,int num_v){
	vector <int> temp_v(num_v,0);
	vector <int> ans;
	for( int i=0;i<a.size();i++ ){
		temp_v[a[i]]++;
	}
	for( int i=0;i<b.size();i++ ){
		temp_v[b[i]]++;
	}
	for( int i=0;i<num_v;i++ ){
		if(temp_v[i] == 2){
			ans.push_back(i);
		}
	}
	return ans;
}

vector <int> bron_kerbosch::Union_set(vector <int> a,vector <int> b,int num_v){
	vector <int> temp_v(num_v,0);
	vector <int> ans;
	for( int i=0;i<a.size();i++ ){
		temp_v[a[i]] = 1;
	}
	for( int i=0;i<b.size();i++ ){
		temp_v[b[i]] = 1;
	}
	for( int i=0;i<num_v;i++ ){
		if(temp_v[i] == 1){
			ans.push_back(i);
		}
	}
	return ans;
}

vector <int> bron_kerbosch::Difference_set(vector <int> a,vector <int> b,int num_v){
	vector <int> temp_v(num_v,0);
	vector <int> ans;
	for( int i=0;i<a.size();i++ ){
		temp_v[a[i]]++;
	}
	for( int i=0;i<b.size();i++ ){
		temp_v[b[i]]--;
	}
	for( int i=0;i<num_v;i++ ){
		if(temp_v[i] == 1){
			ans.push_back(i);
		}
	}
	return ans;
}

bool bron_kerbosch::Is_Duplicate(int n,vector <int> X){
	for( int i=0;i<X.size();i++ ){
		if(n == X[i]){
			return true;
		}
	}
	return false;
}

void bron_kerbosch::start_find_MC(vector <int> P,vector <vertex> *v){
	vector <int> R,X;
	used.resize(P.size(), 0);
	find_maximal_clique(R,P,X,v);
}

void bron_kerbosch::find_maximal_clique(vector <int> R,vector <int> P,vector <int> X,vector <vertex> *v){
	if(P.size() == 0 && X.size() == 0){
		
		/*temp->num = (*m_clique).size();
		for( int i=0;i<R.size();i++ ){
			temp->member.push_back(R[i]]);
		}
		(*m_clique).push_back(temp);*/
		maximal_clique.push_back(R);
		for( int i=0;i<R.size();++i ){
			used[R[i]] = 1;
		}
		
		
		return;
	}
	else{
		for( int i=P.size()-1;i>=0;i-- ){
			if(!Is_Duplicate(P[i],X)){
				vector <int> temp_R = R;
				vector <int> temp_v;
				if(!used[P[i]]){
					temp_v.push_back(P[i]);
					temp_R.push_back(P[i]);
					find_maximal_clique(temp_R,Intersection_set(P,(*v)[P[i]].neightbor,v->size()),Intersection_set(X,(*v)[P[i]].neightbor,v->size()),&(*v));
					X = Union_set(X,temp_v,v->size());
				}
			}
		}
	}
}

/*void bron_kerbosch::get_all_clique_data(vector < vector < vector <int> > > *data_out,vector <int> word_width_list,vector <RAM_datas> RAM_data){
	for( int i=0;i<maximal_clique.size();i++ ){
		vector < vector <int> > temp_data(word_width_list.size());
		for( int j=0;j<maximal_clique[i].size();j++ ){
			temp_data[get_word_width_num(RAM_data[maximal_clique[i][j]].word_width,word_width_list)].push_back(maximal_clique[i][j]);
		}
		data_out->push_back(temp_data);
	}
}*/

int bron_kerbosch::get_word_width_num(int a,vector <int> b){
	for( int i=0;i<b.size();i++ ){
		if(a == b[i]){
			return i;
		}
	}
}

/*void data_transform(vector <input_data> data_in, vector < vector <RAM_datas> > *data_out, double routing_distance,vector <int> *word_width_list){
	vector <RAM_datas> temp_list;
	RAM_datas temp;
	int temp_word_width = 0;
	for( int i=0;i<data_in.size();i++ ){
		temp.lx = data_in[i].port_x-routing_distance-data_in[i].port_y;
		temp.ly = data_in[i].port_x-routing_distance+data_in[i].port_y;
		temp.rx = data_in[i].port_x+routing_distance-data_in[i].port_y;
		temp.ry = data_in[i].port_x+routing_distance+data_in[i].port_y;
		temp.word_width = data_in[i].word_width;
		temp.RAM_ID = i;
		temp_list.push_back(temp);
	}
	sort(temp_list.begin(),temp_list.end(),wort_width_comp);
	
	for( int i=0;i<temp_list.size();i++ ){
		if(temp_word_width != temp_list[i].word_width){
			word_width_list->push_back(temp_list[i].word_width);
			temp_word_width = temp_list[i].word_width;
			vector <RAM_datas> temp_data;
			data_out->push_back(temp_data);
		}
		(*data_out)[(*data_out).size()-1].push_back(temp_list[i]);
	}
}*/

/*void P_graph_gen(vector <RAM_datas> RAM_data,vector <vertex> *P_graph){
	int minp[2],maxp[2];
	P_graph->resize(RAM_data.size());
	RTree<int, int, 2, float>* rtree = new RTree<int, int, 2, float>;
	for( int i=0;i<RAM_data.size();i++ ){
		minp[0] = RAM_data[i].lx;
		minp[1] = RAM_data[i].ly;
		maxp[0] = RAM_data[i].rx;
		maxp[1] = RAM_data[i].ry;
		rtree->Insert(minp,maxp,i);
	}
	for( int i=0;i<RAM_data.size();i++ ){
		minp[0] = RAM_data[i].lx;
		minp[1] = RAM_data[i].ly;
		maxp[0] = RAM_data[i].rx;
		maxp[1] = RAM_data[i].ry;
		rtree->Search(minp,maxp,&(*P_graph)[i].neightbor);
		for( int j=0;j<(*P_graph)[i].neightbor.size();j++ ){
			if((*P_graph)[i].neightbor[j] == i){
				(*P_graph)[i].neightbor.erase((*P_graph)[i].neightbor.begin()+j);
			}
		}
		
	}
	
	
	//getchar();
}*/


/*bool wort_width_comp(RAM_datas a,RAM_datas b){
	return a.word_width > b.word_width;
}*/

void bron_kerbosch::find_final_clique(vector < vector <int> > &final_clique, int v_size){
	
	vector <int> used(v_size, 0);
	int init_v = v_size;
	while(init_v > 0){
		//cout << init_v << "   33333" << endl;
		//getchar();
		int max_v = 0, max_clique;
		for( int i=0;i<maximal_clique.size();++i ){
			int temp_v = 0;
			//cout << "0" << endl;
			for( int j=0;j<maximal_clique[i].size();++j ){
				//cout << maximal_clique[i][j] << endl;
				if(!used[maximal_clique[i][j]]){
					temp_v ++;
				}
			}
			if(temp_v > max_v){
				max_v = temp_v;
				max_clique = i;
			}
		}
		vector <int> temp_c;
		for( int i=0;i<maximal_clique[max_clique].size();++i ){
			if(!used[maximal_clique[max_clique][i]]){
				used[maximal_clique[max_clique][i]] = 1;
				temp_c.push_back(maximal_clique[max_clique][i]);
				init_v --;
			}
		}
		final_clique.push_back(temp_c);
	}
}

//------------------------------------cout-------------------------------------------------------
void bron_kerbosch::cout_clique(){
	cout<<"number of maximal clique: "<<maximal_clique.size()<<endl;
	for( int i=0;i<maximal_clique.size();i++ ){
		for( int j=0;j<maximal_clique[i].size();j++ ){
			cout<<maximal_clique[i][j]<<" ";
		}
		cout<<endl;
	}
}
//-----------------------------------------------------------------------------------------------




