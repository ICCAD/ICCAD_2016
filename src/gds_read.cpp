#include "gds_read.h"
#include "data_structure.h"


ifstream::pos_type getPointer;
int h;extern int u;
extern double chip_hx,chip_hy;
int temp_layer;
char * memblock;
unsigned Int_2u( unsigned hi, unsigned lo );
int Numb_Bytes(int hi, int lo );
int Int_Sign4( int first, int second, int third, int forth ) ;
double Real_8( int X0, int X1, int X2, int X3,int X4, int X5, int X6, int X7 );
bool check_layer(vector<int> *layer_list, int layer);
string CodeID( int hi, int lo );
char str2[100000];
const char *fname;
void converter (vector< gds_data > *gds_datas, vector<int> *layer_list,const char *inputfile)
{
    
	//ofstream fout(outputfile);
	fname = inputfile; //Copy a StreamFile and rename it.
	//fout <<""<< fname << " is the file being read \n" ;
	ifstream file (fname, ios::in|ios::binary|ios::ate);
    vector<points> points_list;
    gds_data temp_gds_data;
    
	if (file.is_open())
	{
		getPointer = file.tellg(); //Get position of the get pointer
		memblock = new char [getPointer];
		file.seekg ( 0, ios::beg); // repositions get pointer to beginning
		getPointer = file.tellg(); //find new position of the get pointer
		file.read ( memblock, getPointer);
		string ID_str, refl_str;
		int numb, val , j; double realVal;
		do
		{
			file.read(str2,2); numb = Numb_Bytes( str2[0], str2[1] ); //first read numb bytes in block
			file.read(str2,2); ID_str = CodeID( str2[0], str2[1] ); //read block ID
			numb = numb - 4; //rest of bytes to read
			file.read(str2,numb); //read rest of bytes into str2
			if ( ID_str == "HEADER Release # ")
			{
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=version F
				//fout <<" " << ID_str << val << " \n" ;
			}
			if ( ID_str == "BGNLIB")
			{
				//fout <<" " << ID_str << " " ; //data_bytes=dates
			}
			if ( ID_str == "LIBNAME = ")
			{
				str2[numb]='\0'; // null terminate str //data_bytes=string
				//fout <<" " << ID_str << str2 << " " ;
			}
			if ( ID_str == "FONTS = ")
			{
				str2[numb]='\0'; // null terminate str //data_bytes=string
				//fout <<" " << ID_str << str2 << " \n" ;
			}
			if ( ID_str == "GENERATIONS")
			{
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=generation
				//fout <<" " << ID_str <<" "<< val << " " ;
			}
			if ( ID_str == "BGNSTR") //data_bytes=dates
			{
				//fout <<" " << ID_str << " " ;
			}
			if ( ID_str == "STRNAME = ")
			{
				str2[numb]='\0'; // null terminate str //data_bytes=string
				//fout <<" " << ID_str << str2 << " \n" ;
			}
			if ( ID_str == "BOUNDARY") //no data_bytes
			{
				//fout <<" " << ID_str << " ";
			}
			if ( ID_str == "LAYER = ")
			{
				temp_layer = Int_2u( str2[0] , str2[1] ); //2_data_bytes=layer
                if (!check_layer(layer_list, temp_layer)) {
                    layer_list->push_back(temp_layer);
					cout << "temp layer  : " << temp_layer << endl;
                }
                temp_gds_data.layer=temp_layer;
				//fout <<" " << ID_str << val << " " ;
			}
			if ( ID_str == "DATATYPE = ")
			{
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=datatype
				//fout <<" " << ID_str << val << " \n" ;
			}
			if ( ID_str == "ENDEL") //no data_bytes
			{
				temp_gds_data.points_list.clear();
                //fout <<"" << ID_str << " \n" ;
			}
			if ( ID_str == "ENDSTR") //no data_bytes
			{
				//fout <<" " << ID_str << " \n" ;
			}
			if ( ID_str == "ENDLIB") //no data_bytes
			{
				//fout <<" " << ID_str << " \n" ;
			}
			if ( ID_str == "SREF") //no data_bytes
			{
				//fout <<" " << ID_str << " " ;
			}
			if ( ID_str == "SNAME")
			{
				str2[numb]='\0'; // null terminate str //data_bytes=string
				//fout <<" " << ID_str<<" " << str2 << " " ;
			}
			if ( ID_str == "STRANS")
			{
				refl_str = "none";
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=relect
				if ( val != 0)
					refl_str = "reflect";
				//fout <<" " << ID_str <<" "<< refl_str << " " ;
			}
			if ( ID_str == "XY = ")
			{
                points temppoint;
                points minpoint,maxpoint;
                minpoint.x=1<<30;
                minpoint.y=1<<30;
                maxpoint.x=0;
                maxpoint.y=0;
                int xrange=0,yrange=0;
                points_list.clear();
                for (j=0; j< numb/8; j=j+1)
				{
					temppoint.x = Int_Sign4( str2[0+8*j], str2[1+8*j], str2[2+8*j], str2[3+8*j] );
					temppoint.y = Int_Sign4( str2[4+8*j], str2[5+8*j], str2[6+8*j], str2[7+8*j] );
                    if (temppoint.x<minpoint.x) {
                        minpoint.x=temppoint.x;
                    }
                    if (temppoint.x>maxpoint.x) {
                        maxpoint.x=temppoint.x;
                    }
                    if (temppoint.y<minpoint.y) {
                        minpoint.y=temppoint.y;
                    }
                    if (temppoint.y>maxpoint.y) {
                        maxpoint.y=temppoint.y;
                    }
					if(j == numb/8-1)
					{
						xrange=maxpoint.x-minpoint.x;
                        yrange=maxpoint.y-minpoint.y;
					}
					else
					{
						points_list.push_back(temppoint);
					}
				}
                if (xrange>yrange) {
                    sort(points_list.begin(), points_list.end(), comp_pointsyy);
                    temp_gds_data.cutfun=false;
                 
                }
                else {
                    sort(points_list.begin(), points_list.end(), comp_points);
                    temp_gds_data.cutfun=true;
                    
                }
                for (int i=0; i<points_list.size()-1; i++) {
                    if (points_list[i].x==points_list[i+1].x && points_list[i].y==points_list[i+1].y) {
                        points_list.erase(points_list.begin()+i, points_list.begin()+i+2);
                        i--;
                    }
                }
                /*for (vector<points>::iterator it=points_list.begin(); it!=points_list.end(); ++it) {
                    cout<<it->x<<" "<<it->y<<endl;
                }
                cout<<endl;
                getchar();*/
                temp_gds_data.points_list=points_list;
                gds_datas->push_back(temp_gds_data);
                
                
			}
			if ( ID_str == "UNITS = ") //data_bytes(real_8) =scale
			{
				realVal = Real_8(str2[0],str2[1],str2[2],str2[3],str2[4],str2[5],str2[6],str2[7] );
				//fout <<" " << ID_str <<"" << realVal << " " ;
				realVal = Real_8(str2[8],str2[9],str2[10],str2[11],str2[12],str2[13],str2[14],str2[15] );
				//fout << realVal << " \n" ;
			}
			if ( ID_str == "TEXT")
			{
				//fout <<" " << ID_str << " " ; //no data_bytes
			}
			if ( ID_str == "TEXTTYPE")
			{
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=texttype
				//fout <<" " << ID_str <<" " << val << " " ;
			}
			if ( ID_str == "PRESENTATION")
			{
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=presentation
				//fout <<" " << ID_str <<" " << val << " " ;
			}
			if ( ID_str == "MAG") //data_bytes(real_8) =magnitude
			{
				realVal = Real_8(str2[0],str2[1],str2[2],str2[3],str2[4],str2[5],str2[6],str2[7] );
				//fout <<" " << ID_str <<" " << realVal << "\n" ;
			}
			if ( ID_str == "ANGLE") //data_bytes(real_8) =angle
			{
				realVal = Real_8(str2[0],str2[1],str2[2],str2[3],str2[4],str2[5],str2[6],str2[7] );
				//fout <<" " << ID_str <<" " << realVal << " " ;
			}
			if ( ID_str == "STRING")
			{
				str2[numb]='\0'; // null terminate str //data_bytes=string
				//fout <<"" << ID_str<<" " << str2 << " \n " ;
			}
			if ( ID_str == "PATHTYPE")
			{
				val = Int_2u( str2[0] , str2[1] ); //2_data_bytes=pathtype
				//fout <<" " << ID_str <<" " << val << " " ;
			}
			if ( ID_str == "WIDTH") //data_bytes(int_4) =width
			{
				val = Int_Sign4( str2[0], str2[1], str2[2], str2[3] );
				//fout <<" " << ID_str <<" " << val << " \n" ;
			}
			if ( ID_str == "PATH") //no data_bytes
			{
				//fout <<" " << ID_str << " " ;
			}
		}
		while ( !file.eof()); /* do readfile until eof */
		file.close();
	} else; //fout << "Unable to open "<< fname << " \n"; //std:://fout << "Hello, World!\n";
    //system("pause");
    //return 0;
}
//==================================================================================================
string CodeID( int hi, int lo )
{
	string str = "" ;
	//if ( hi == 00 && lo == 02 ) str = "HEADER release # ";
	if ( hi == 00 && lo == 02 ) str = "HEADER Release # ";
	if ( hi == 01 && lo == 02 ) str = "BGNLIB"; // 0101
	if ( hi == 02 && lo == 06 ) str = "LIBNAME = "; // 0206
	if ( hi == 32 && lo == 06 ) str = "FONTS = "; // 2006
	if ( hi == 34 && lo == 02 ) str = "GENERATIONS"; // 2202
	if ( hi == 03 && lo == 05 ) str = "UNITS = "; // 0305
	if ( hi == 05 && lo == 02 ) str = "BGNSTR"; // 0502
	if ( hi == 06 && lo == 06 ) str = "STRNAME = "; // 0606
	if ( hi == 8 && lo == 00 ) str = "BOUNDARY"; // 0800
	if ( hi == 13 && lo == 02 ) str = "LAYER = "; // 0d02
	if ( hi == 14 && lo == 02 ) str = "DATATYPE = "; // 0e02
	if ( hi == 16 && lo == 03 ) str = "XY = "; // 1003
	if ( hi == 17 && lo == 0 ) str = "ENDEL"; // 1100
	if ( hi == 7 && lo == 0 ) str = "ENDSTR"; // 0700
	if ( hi == 4 && lo == 0 ) str = "ENDLIB"; // 0400
	if ( hi == 10 && lo == 0 ) str = "SREF"; // 0a00
	if ( hi == 18 && lo == 6 ) str = "SNAME"; // 1206
	if ( hi == 26 && lo == 1 ) str = "STRANS"; // 1a01
	if ( hi == 12 && lo == 0 ) str = "TEXT"; // 0c00
	if ( hi == 22 && lo == 2 ) str = "TEXTTYPE"; // 1602
	if ( hi == 23 && lo == 1 ) str = "PRESENTATION"; // 1701
	if ( hi == 27 && lo == 5 ) str = "MAG"; // 1b05
	if ( hi == 25 && lo == 6 ) str = "STRING"; // 1906
	if ( hi == 33 && lo == 2 ) str = "PATHTYPE"; // 2102
	if ( hi == 15 && lo == 3 ) str = "WIDTH"; // 0f03
	if ( hi == 9 && lo == 0 ) str = "PATH"; // 0900
	if ( hi == 28 && lo == 5 ) str = "ANGLE"; // 1C05
	return str;
}
//==================================================================================================
double Real_8(int X0, int X1, int X2, int X3,int X4, int X5, int X6, int X7 )
{
	if ( X0 <0 ) X0 = X0+256;
	if ( X1 <0 ) X1 = X1+256;
	if ( X2 <0 ) X2 = X2+256;
	if ( X3 <0 ) X3 = X3+256;
	if ( X4 <0 ) X4 = X4+256;
	if ( X5 <0 ) X5 = X5+256;
	if ( X6 <0 ) X6 = X6+256;
	if ( X7 <0 ) X7 = X7+256;
	int exp , i ,bit,testbit, scalebit;
	exp = (X0 & 127); // will bit mask to = 01111111
	exp = 0;
	for (i=1; i< 8 ; i++)
	{
		bit = (int) pow(2,7-i);
		exp = exp + (X0 & bit);
	}
	exp = exp-65; // add offset
	double matval,matvalb , value;
	matval = 0;
	for (i=0; i< 8 ; i++)
	{
		bit = pow(2,7-i);
		scalebit = pow(2,i);
		testbit = (X1 & bit);
		matvalb = (double) 8*(X1 & bit)/(bit*scalebit);
		matval = matval + matvalb;
		matvalb = (double) 8*(X2 & bit)/(256*bit*scalebit);
		matval = matval + matvalb;
		matvalb = (double) 8*(X3 & bit)/(256*256*bit*scalebit);
		matval = matval + matvalb;
	}
	value = matval*pow(16,exp);
	return value;
}
//==================================================================================================
int Int_Sign4( int first, int second, int third, int forth )
{
	int numb ;
	if ( second <0 ) second = second+256;
	if ( third <0 ) third = third +256;
	if ( forth <0 ) forth = forth +256;
	numb = (first<<24) + (second<<16)+ (third<<8) +forth;
	return numb;
}
//==================================================================================================
int Numb_Bytes( int hi, int lo )
{
	int numb ;
	if ( hi <0 ) hi = hi+256; if ( lo <0 ) lo = lo+256;
	numb =(hi<<8) + lo;
	return numb;
}
//==================================================================================================
unsigned Int_2u( unsigned hi, unsigned lo )
{
	int numb ;
	numb =(hi<<8) + lo; // //fout <<hi<<" "<<lo<<""<<numb<< " \n" ;
	return numb;
}

void writegds(const char *outputfile,vector<rect> rectdata,int layer, int xbound, int ybound, int baselayer)
{
    if (layer==0) {
        
        time_t *now;
        struct tm *date;
        
        now  = (time_t *)    malloc( sizeof( time_t ) );
        date = (struct tm *) malloc( sizeof( struct tm ) );
        
        time( now );
        date = localtime( now );

        
        ofstream fout (outputfile, ios::out|ios::binary);
        str2[0]=Int_Conv(6, 1);
        str2[1]=Int_Conv(6, 0);
        fout.write(str2, 2);
        str2[0]=0;
        str2[1]=2;
        fout.write(str2, 2);
        str2[0]=Int_Conv(600, 1);
        str2[1]=Int_Conv(600, 0);
        fout.write(str2, 2);// HEADER Release # 600
        str2[0]=Int_Conv(28, 1);
        str2[1]=Int_Conv(28, 0);
        fout.write(str2, 2);
        str2[0]=1;
        str2[1]=2;
        fout.write(str2, 2);//BGNLIB
        str2[0]=Int_Conv(1900 + date->tm_year,1);
        str2[1]=Int_Conv(1900 + date->tm_year,0);
        //fout.write(str2, 2);
        str2[2]=Int_Conv(1 + date->tm_mon,1);
        str2[3]=Int_Conv(1 + date->tm_mon,0);
        //fout.write(str2, 2);
        str2[4]=Int_Conv(date->tm_mday,1);
        str2[5]=Int_Conv(date->tm_mday,0);
        //fout.write(str2, 2);
        str2[6]=Int_Conv(date->tm_hour,1);
        str2[7]=Int_Conv(date->tm_hour,0);
        //fout.write(str2, 2);
        str2[8]=Int_Conv(date->tm_min,1);
        str2[9]=Int_Conv(date->tm_min,0);
        //fout.write(str2, 2);
        str2[10]=Int_Conv(date->tm_sec,1);
        str2[11]=Int_Conv(date->tm_sec,0);
        fout.write(str2, 12);
        fout.write(str2, 12);//time*2
        str2[0]=Int_Conv(10, 1);
        str2[1]=Int_Conv(10, 0);
        fout.write(str2, 2);
        str2[0]=2;
        str2[1]=6;
        fout.write(str2, 2);
        str2[0]='d';
        str2[1]='r';
        str2[2]='c';
        str2[3]='.';
        str2[4]='d';
        str2[5]='b';
        fout.write(str2, 6);//LIBNAME = drc.db
        str2[0]=Int_Conv(20, 1);
        str2[1]=Int_Conv(20, 0);
        fout.write(str2, 2);
        str2[0]=3;
        str2[1]=5;
        fout.write(str2, 2);
        str2[0]=62;
        str2[1]=65;
        str2[2]=-119+256;
        str2[3]=55;
        str2[4]=75;
        str2[5]=-58+256;
        str2[6]=-89+256;
        str2[7]=-16+256;
        str2[8]=57;
        str2[9]=68;
        str2[10]=-72+256;
        str2[11]=47;
        str2[12]=-96+256;
        str2[13]=-101+256;
        str2[14]=90;
        str2[15]=80;
        fout.write(str2, 16);//UNITS = 0.001 1e-09
        str2[0]=Int_Conv(28, 1);
        str2[1]=Int_Conv(28, 0);
        fout.write(str2, 2);
        str2[0]=5;
        str2[1]=2;
        fout.write(str2, 2);//BGSTR
        str2[0]=Int_Conv(1900 + date->tm_year,1);
        str2[1]=Int_Conv(1900 + date->tm_year,0);
        //fout.write(str2, 2);
        str2[2]=Int_Conv(1 + date->tm_mon,1);
        str2[3]=Int_Conv(1 + date->tm_mon,0);
        //fout.write(str2, 2);
        str2[4]=Int_Conv(date->tm_mday,1);
        str2[5]=Int_Conv(date->tm_mday,0);
        //fout.write(str2, 2);
        str2[6]=Int_Conv(date->tm_hour,1);
        str2[7]=Int_Conv(date->tm_hour,0);
        //fout.write(str2, 2);
        str2[8]=Int_Conv(date->tm_min,1);
        str2[9]=Int_Conv(date->tm_min,0);
        //fout.write(str2, 2);
        str2[10]=Int_Conv(date->tm_sec,1);
        str2[11]=Int_Conv(date->tm_sec,0);
        fout.write(str2, 12);
        fout.write(str2, 12);//time*2
        str2[0]=Int_Conv(12, 1);
        str2[1]=Int_Conv(12, 0);
        fout.write(str2, 2);
        str2[0]=6;
        str2[1]=6;
        fout.write(str2, 2);
        str2[0]=84;
        str2[1]=79;
        str2[2]=80;
        str2[3]=67;
        str2[4]=69;
        str2[5]=76;
        str2[6]=76;
        str2[7]=0;
        fout.write(str2, 8);//STRNAME = TOPCELL
        fout.close();
    }
    ofstream fout (outputfile, ios::out|ios::binary|ios::app);
    
	for (int k=0; k<rectdata.size(); k++) { //until area fit
		str2[0]=Int_Conv(4, 1);
		str2[1]=Int_Conv(4, 0);
		fout.write(str2, 2);
		str2[0]=8;
		str2[1]=0;
		fout.write(str2, 2);//BOUNDARY
		str2[0]=Int_Conv(6, 1);
		str2[1]=Int_Conv(6, 0);
		fout.write(str2, 2);
		str2[0]=13;
		str2[1]=2;
		fout.write(str2, 2);
		str2[0]=Int_Conv(layer+baselayer, 1);
		str2[1]=Int_Conv(layer+baselayer, 0);
		fout.write(str2, 2);//LAYER
		str2[0]=Int_Conv(6, 1);
		str2[1]=Int_Conv(6, 0);
		fout.write(str2, 2);
		str2[0]=14;
		str2[1]=2;
		fout.write(str2, 2);
		str2[0]=Int_Conv(0, 1);
		str2[1]=Int_Conv(0, 0);
		fout.write(str2, 2);//DATATYPE
		str2[0]=Int_Conv(44, 1);
		str2[1]=Int_Conv(44, 0);
		fout.write(str2, 2);
		str2[0]=16;
		str2[1]=3;
		fout.write(str2, 2);//XY=
		points rectpoints[5];
		rectpoints[0]=rectdata[k].minp;
		rectpoints[1].x=rectdata[k].minp.x;
		rectpoints[1].y=rectdata[k].maxp.y;
		rectpoints[2]=rectdata[k].maxp;
		rectpoints[3].x=rectdata[k].maxp.x;
		rectpoints[3].y=rectdata[k].minp.y;
		rectpoints[4]=rectdata[k].minp;
		for (int l=0; l<5; l++) {
			str2[8*l+0]=Int_Conv(rectpoints[l].x,3);
			str2[8*l+1]=Int_Conv(rectpoints[l].x,2);
			str2[8*l+2]=Int_Conv(rectpoints[l].x,1);
			str2[8*l+3]=Int_Conv(rectpoints[l].x,0);
			
			str2[8*l+4]=Int_Conv(rectpoints[l].y,3);
			str2[8*l+5]=Int_Conv(rectpoints[l].y,2);
			str2[8*l+6]=Int_Conv(rectpoints[l].y,1);
			str2[8*l+7]=Int_Conv(rectpoints[l].y,0);
		}
		fout.write(str2, 40);
		str2[0]=Int_Conv(4, 1);
		str2[1]=Int_Conv(4, 0);
		fout.write(str2, 2);
		str2[0]=17;
		str2[1]=0;
		fout.write(str2, 2);//ENDL
	}
    //cout<<temparea<<endl;
    fout.close();
};

int Int_Conv(int a, int num)
{
    return (a>>(num*8))&255;
};

bool comp_points(points aa, points bb){
    
    if (aa.y>bb.y) {
        return true;
    }
    else if (aa.y<bb.y) {
        return false;
    }
    else {
        if (aa.x>bb.x) {
            return true;
        }
        else if (aa.x<=bb.x) {
            return false;
        }
    }
    return false;
}

bool comp_pointsyy(points aa, points bb){
    
    if (aa.x>bb.x) {
        return true;
    }
    else if (aa.x<bb.x) {
        return false;
    }
    else {
        if (aa.y>bb.y) {
            return true;
        }
        else if (aa.y<=bb.y) {
            return false;
        }
    }
    return false;
}

bool comp_layer(int aa, int bb) {
    return bb-aa;
}

bool check_layer(vector<int> *layer_list, int layer) {
    for (int ii=0; ii<layer_list->size(); ii++) {
        if (layer==(*layer_list)[ii]) {
            return true;
        }
    }
    return false;
}

