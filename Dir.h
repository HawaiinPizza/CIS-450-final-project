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

//Description: Create a direcotry given a path
///Pre-condition: string path with it's parent made
//Post-condition: The direcotry is made in the parent
int DirCreate(string path){ 
	f;
	inode child=getInode(path);
	size_t found = path.find_last_of("/");
	inode parent=getInode(path.substr(0,found));
	if(parent.size==inode().size){ // Parent does not exist
		osErrMsg="E_DIR_CREATE";
		Log(-1, path);	
		return -1;
	}
	else{
		if (child.size != inode().size){ // Child alreayd exist
			osErrMsg="E_DIR_CREATE";
			Log(-1, path);
			return -1;
		}
		else if (path.length()>256 ){ // Checking if path exceeds 256 characters
			osErrMsg="E_DIR_CREATE";
			Log(-1, path);
			return -1;
		}
		else if( WorkDisk[1].to_ulong()>=100){ // There are 100 or over files/direcotires already
			osErrMsg="E_DIR_CREATE";
			Log(-1, path);
			return -1;
		}
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
				string temp=path.substr(found+1);
				dir NewDir(temp, posInode.Count+(posInode.Sect-3)*35);
				if(posParDir.isPar==true){
					osErrMsg="E_DIRCREATE";
					Log(-1, path);
					return -1;
				}
				else{
					writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
					writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				}
				dir parDir=readDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count);
				int Alloc=(posDir.Sect);
				child.alloc[0]=Alloc; 
				writeInodeSectInode(WorkDisk[posInode.Sect], posInode.Count, child);
				// Update size
				parent.size++;
				bool stop=false;
				for(int i=3; i<6 && !stop; i++){
					for(int k=0; k<10 && !stop; i++){
						forloop2(0,35){
							if(parent.alloc[k]==readInodeSectInode(WorkDisk[i],j).alloc[k]){
								writeInodeSectInode(WorkDisk[i],j, parent);
								stop=true;
								break;
							}
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
				Log(0, path);
				return 0;
			}
			else // There is no more space for a new directory
				osErrMsg="E_DIR_CREATE";
			Log(-1, path);
			return -1;
			}
		}
	}

	//Description: Gets the size of the direcotry
	///Pre-condition: The direcotry exist
	//Post-condition: Returns the number of directoreis in a direcotry

	int DirSize(string path){ // Get the size of the dctionary
	f;
		inode inodepath=getInode(path);
		if (inodepath.size == inode().size){ // indoe does not exist
			osErrMsg="E_DIR_SIZE";
			Log(-1, path);
			return -1;
		}
		else{
			Log(inodepath.size, path);
			return inodepath.size;
		}
	}

	//Description: Get each direcotry in the direcotry
	///Pre-condition: The direcotry exist, and the buffer is of appoeriate size
	//Post-condition: Writes each direcotyr name in buffer
	int DirRead(string path, string &buffer){ // Helper function of Read.
	f;
		inode Inode=getInode(path);
		if(Inode.isFile==true || Inode.alloc[0]==-1 || Inode.alloc[0]==1023){ // Check for is file. Save myself soem headachek
			osErrMsg="E_DIR_READ";
			Log(-1, path);
			return -1;
		}
		if(buffer.length()!= DirSize(path)){
			osErrMsg="E_BUFFER_TOO_SMALL";
			Log(-1, path);
			return -1;
		}
		forloop2(1, dirCount){
			bitset<dirSize> bitRead( readDirSect(WorkDisk[Inode.alloc[0]],j));
			if(bitRead!=0 ){
				buffer+=getBitDir(bitRead).Name;
				buffer+=' ';
			}
		}
		Log(0, path);
		return 0;
		// See if next alloc is valid 
	}

	//Description: Get each direcotry in the direcotry
	///Pre-condition: The direcotry exist, and the buffer is of appoeriate size
	//Post-condition: Print out each direcotry.
	int _DirRead(string path){ // Helper function of Read.
	f;
		inode Inode=getInode(path);
		if(Inode.isFile==true || Inode.alloc[0]==-1 || Inode.alloc[0]==1023) // Check for is file. Save myself soem headachek
			Log(-1, path);
		return -1;
		forloop2(1, dirCount){
			bitset<dirSize> bitRead( readDirSect(WorkDisk[Inode.alloc[0]],j));
		}
		Log(0, path);
		return 0;
	}

	//Description: Delete the directory 
	///Pre-condition: The direcotry itself exist AND it's nonempty. Input of a path
	//Post-condition: Delete the direcotyr and it's corresponding entry in it's parent direcotry and inode.
	int DirUnlink(string path){ //Remove a file.
	f;
		inode delNode=getInode(path);
		size_t found = path.find_last_of("/");
		inode parent=getInode(path.substr(0,found));
		if(delNode.size==-1){ // File does not exist
			osErrMsg="E_DIR_NO_EXIST";
			Log(-1, path);
			return -1;
		}
		else if ( DirSize(path)!=0){ //Path is not empty. 
			_DirRead(path);
			/* cout << "NOT EMPTY\t" << DirSize(path) << '\t' << delNode.alloc[0] << endl; */
			osErrMsg="E_DIR_NOT_EMPTY";
			Log(-1, path);
			return -1;
		}
		else if ( delNode.alloc[0]==6){ // 6 is where the root directory is. TODO mkae this look easier to read for Stenier
			osErrMsg="E_DEL_ROOT_DIR";
			Log(-1, path);
			return -1;
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
			for(int i=0; i<10; i++){
				if(delNode.alloc[i]!=1023)
					writeDirSect(WorkDisk[delNode.alloc[i]], 0,temp); // Deleting the acutal entry.
				else
					break;
			}
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
			Log(0, path);
			return 0;
		}
	}
#endif
