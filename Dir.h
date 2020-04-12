#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>


int getDirPath(string path){ // Given a path like /Dir1/Dir2/File, get hte inode from it. 
	vector<string> dirname;
	string basename="";
	
	{ // Incldue check if the end of string is /. Tht's badddddd
		if(path[path.length()-1]=='/' && path.length()!=1){
			path=path.substr(0,path.length()-1) ;
		}
		int temp=0;
		forloop(0, path.length()){
			if(path[i]=='/'){
				temp=i;

			}

		}
		basename=path.substr(temp+1);
	}


	return -1; //Fix later


}
