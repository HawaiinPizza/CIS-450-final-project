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


int DirCreate(string path){ 
	inode child=getInode(path);
	size_t found = path.find_last_of("/");
	inode parent=getInode(path.substr(0,found));
	if(parent.size==inode().size){ // Parent does not exist
		return -1;
	}
	else{
		if (child.size != inode().size){ // Child alreayd exist
			return -2; 
		}
		else if (path.length()>256 ){ // Checking if path exceeds 256 characters
			return -3;
		}
		else{ // Can make dirctioanry.
			child.isFile=false;
			child.size=1;

			dir Dir;
			pos posDir;
			pos posParDir;
			pos posInode;

			posInode=getFreeInode();
			posDir=getFreeDataBlock();
			posParDir=getFirDir( 6+parent.alloc[0]);


			if(posDir.Count!=-1 && posInode.Count!=-1 && posParDir.Count!=-1){ // This means there is free space for a new direcotry
				string temp=path.substr(found+1);
				dir NewDir(temp, 0);
				writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
				writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				dir parDir=readDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count);
				child.alloc[0]=posInode.Alloc;
				child.size=0;
				writeInodeSectInode(WorkDisk[posInode.Sect], posInode.Count, child);

				/* cout << readInodeSectBit(WorkDisk[posInode.Sect], posInode.Count); */
				/* cout << "\nToo busy printin\n"; */
				/* cout << "ParentNode\t" << parent.size << '\t' << parent.alloc[0] << endl; */
				/* cout << "Path\t" << path << '\t' << path.substr(0,found) << endl; */
				/* cout << "Dir\t" << posDir.Count << '\t' << posDir.Sect << endl; */
				/* cout << "ParDir\t" << posParDir.Count << '\t' << posParDir.Sect << endl; */
				/* cout << "ChildNode\t" << child.size << '\t' << child.alloc[0] << endl; */
				/* cout << "New Inode\t" << posInode.Sect << '\t' << posInode.Count << endl; */
				/* cout << "New Dir\t" << parDir.Name << '\t' << parDir.inodePlace  << endl; */
				/* cout << endl; */




				return 0;
			}
			else // There is no more space for a new directory
				return -4;


		}

	}
}

int DirSize(string path){ // Get the size of the dctionary
	inode inodepath=getInode(path);
	if (inodepath.size == inode().size){ // Child alreayd exist
		return -1; 
	}
	else{
		int count =0;
		forloop(0, 10){
			forloop2(0, dirCount){
				if ( readDirSect(WorkDisk[inodepath.alloc[i+6]], j)!=0 ) {
					count++;
					dir Dir=readDirSectDir(WorkDisk[inodepath.alloc[1+6]], j);
					cout << "read\t" << Dir.Name << '\t' << inodepath.alloc[i+6] << '\t' << j    << endl;

				}
			}
		}
		return count;
	}

}

int DirUnlink(string path){ //
}


#endif
