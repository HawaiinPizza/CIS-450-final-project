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
	dirname.push_back("/");
	string basename="";
	
	if(path[path.length()-1]=='/' && path.length()!=1){  // Exception checking, if path ends with a /, cause we both know i'll forget to not do that :)
		path.erase(path.length()-1, path.length());
	}
	else if (path=="/") // Checkign if it's just root.
		return 0;
	{ // Getting list of dirnames 
		int temp=1;
		forloop(1,path.length()){
			if(path[i]=='/'){ // new directory
				string tempStr=path.substr(temp,i-1);
				dirname.push_back(tempStr);
			}
		}
		dirname.push_back(path.substr(temp));
		foreach(dirname){
			cout << i << '\t';
		}
		cout << endl;
	}
	return 0; //Fix later
}


#endif
