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
			child.size=0;

			dir Dir;
			pos posDir;
			pos posParDir;
			pos posInode;

			posInode=getFreeInode();
			posDir=getFreeDataBlock();
			posParDir=getFirDir( parent.alloc[0]); // TODO Might be reason of problem



			if(posDir.Count!=-1 && posInode.Count!=-1 && posParDir.Count!=-1){ // This means there is free space for a new direcotry
				string temp=path.substr(found+1);
				dir NewDir(temp, posInode.Count+(posInode.Sect-3)*35);
				/* cout << "NINJA BOY\t" << NewDir.inodePlace; */
				writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
				writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				dir parDir=readDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count);
				int Alloc=(posDir.Sect);
				child.alloc[0]=Alloc; //CAUSE OF BUG
				writeInodeSectInode(WorkDisk[posInode.Sect], posInode.Count, child);
				/* cout << "ACTUALLY WROTE\t" << readInodeSectInode(WorkDisk[posInode.Sect], posInode.Count).alloc[0] << endl; */ 

				// Update size
				parent.size++;
				bool stop=false;
				for(int i=3; i<6 && !stop; i++){
					forloop2(0,35){
						if(parent.alloc[0]==readInodeSectInode(WorkDisk[i],j).alloc[0]){
							writeInodeSectInode(WorkDisk[i],j, parent);
							stop=true;
							break;

						}
					}
				}
				/* cout << "Child info\t" << child.size << '\t' << child.alloc[0] << endl; */
				/* cout << "Parent info\t" << parent.size << '\t' << parent.alloc[0] << endl; */
				/* cout << "pos table\t" << "Sect\t" << "Count" << endl; */
				/* cout << "pos of inode\t" << posInode.Sect << '\t' << posInode.Count << '\t' << Alloc << endl; */
				/* cout << "pos of dir\t" 	 << posDir.Sect << '\t' << posDir.Count << endl; */
				/* cout << "pos of par dir\t" << posParDir.Sect << '\t' << posParDir.Count << endl; */
				

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
	if (inodepath.size == inode().size){ // indoe does not exist
		return -1; 
	}
	else{
		return inodepath.size;
	}

}

int DirUnlink(string path){ //Remove a file.
	inode delNode=getInode(path);
	size_t found = path.find_last_of("/");
	inode parent=getInode(path.substr(0,found));
	if(delNode.size==-1){ // File does not exist
		return -1;
	}
	else if ( DirSize(path)!=0){ //Path is not empty. 
		cout << "NOT EMPTY\t" << DirSize(path) << '\t' << delNode.alloc[0] << endl;
		return -2;
	}
	else if ( delNode.alloc[0]==6){ // 6 is where the root directory is. TODO mkae this look easier to read for Stenier
		return -3;
	}
	else{ // Now we can delete it. Hurray. TODO make a getInode iwth possiton of the inode, so I don't have to ifnd it twice like a stupid

		forloop(3,6){ // Deleting inode.
			forloop2(0,35){
				if(delNode.alloc[0]==readInodeSectInode(WorkDisk[i],j).alloc[0]){
					writeInodeSect(WorkDisk[i], j, "0");
				}
			}
		}
		bitset<dirSize> temp(0);
		writeDirSect(WorkDisk[delNode.alloc[0]], 0,temp); // Deleting the acutal entry.

		// Now we gotta decrease the parent' inode siz
		parent.size--;
		forloop(3,6){
			forloop2(0,35){
				if(parent.alloc[0]==readInodeSectInode(WorkDisk[i],j).alloc[0]){
					writeInodeSectInode(WorkDisk[i],j, parent);
				}
			}
		}

		//Now, we must find the parent dictionary file of del, and remove it
		size_t found = path.find_last_of("/");
		string child=path.substr(found+1);
		bool stop=false;
		for(int i=0; i<dirCount && !stop; i++){
			bitset<dirSize>  dirBit(readDirSect(WorkDisk[parent.alloc[0]], i));
			if(dirBit!=0){
				dir Dir=getBitDir(dirBit);
				if(Dir.Name==child){ // Found the cihld directory in parent
					cout << "I wonder what";
					dirBit=0;
					writeDirSect(WorkDisk[parent.alloc[0]]  , i, dirBit);
					stop=true;
					break;
				}

			}
			

		}
		return 0;
	}

}


#endif
