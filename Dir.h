#ifndef DIR_H
#define DIR_H
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#include "BitData.h"


int getDirPath(string path){ // Given a path like /Dir1/Dir2/File, get hte inode from it. 
	vector<string> dirname;
	string basename="";
	
	if(path[path.length()-1]=='/' && path.length()!=1){  // Incldue check if the end of string is /. Tht's badddddd
		path.erase(path.length()-1, path.length());
	}
	else if (path=="/")
		return 0;
	cout << path << endl;
	forloop(0,path.length()){


	}




	return 0; //Fix later


}


#endif
