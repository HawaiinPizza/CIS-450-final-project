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
		else if( WorkDisk[1].to_ulong()>=100) // There are 100 or over files/direcotires already
			return -5;
		else{ // Can make dirctioanry.
			child.isFile=false;
			child.size=0;

			dir Dir;
			pos posDir;
			pos posParDir;
			pos posInode;

			posInode=getFreeInode();
			posDir=getFreeDataBlock(true);
			posParDir=getFirDir( parent.alloc[0]); 



			if(posDir.Count!=-1 && posInode.Count!=-1 && posParDir.Count!=-1){ // This means there is free space for a new direcotry
				// Swap posDir and posParDir{
				cout << path << endl;
				string temp=path.substr(found+1);
				dir NewDir(temp, posInode.Count+(posInode.Sect-3)*35);
				/* cout << "NINJA BOY\t" << NewDir.inodePlace; */
				if(posParDir.isPar==true){
				{
				pos _postemp=posDir;
				posDir=posParDir;
				posParDir=_postemp;
				}
					dir ParentDir;
					pos _temp = getInodePos(path.substr(0,found));

					ParentDir.inodePlace=(_temp.Sect-3)*35+_temp.Count;
					string test2=path.substr(0, found);

					found = test2.find_last_of("/");

					forloop(0, 14){
						ParentDir.Name[i]=test2.substr(found+1)[i];
					}



					writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, ParentDir );
					writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count+1, NewDir );
					writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				}
				else{
					writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
					writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				}
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

				//Bitmap techincally isn't as intended, but i Dont' care. use bitamp of indoe and datablcok to get count of datablocks and indoe.
				uint _bitmapSize=WorkDisk[1].to_ulong();
				_bitmapSize++;
				WorkDisk[1]=_bitmapSize;
				_bitmapSize=WorkDisk[2].to_ulong();
				_bitmapSize++;
				WorkDisk[2]=_bitmapSize;
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
		uint _bitmapSize=WorkDisk[1].to_ulong();
		_bitmapSize--;
		WorkDisk[1]=_bitmapSize;
		_bitmapSize=WorkDisk[2].to_ulong();
		_bitmapSize--;
		WorkDisk[2]=_bitmapSize;
		return 0;
	}

}


void _DirRead(string path){ // Helper function of Read.
	inode Inode=getInode(path);
	cout << "YEA\n";
	forloop(0, 10){
		cout << "CURRENT: " << i << '\t' << Inode.alloc[i] << '\t';
		forloop2(0, dirCount){
			bitset<dirSize> bitRead( readDirSect(WorkDisk[Inode.alloc[i]],j));
			if(bitRead!=0 && !( i==0 && j==0))
				cout << getBitDir(bitRead).Name << endl;
		}
		// See if next alloc is valid 
		if(Inode.alloc[i+1]==1023)
			return;
	}
}

#endif
