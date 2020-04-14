#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#include "BitData.h"

struct file{
	pos Loc;
	string buffer;
	int Seek=0;

	file(pos _Loc, string _buffer){
		Loc=_Loc;
		buffer=_buffer;
	}

	string read(int _Seek, int& status){
		if(_Seek>buffer.length() || _Seek<=0)
			status=-1;
		else{
			string temp = buffer.substr(Seek, _Seek);
			Seek=_Seek;
			return temp;
		}
	}

	string write(int _Seek, int& status){
		if(_Seek>buffer.length() || _Seek<=0)
			status=-1;
		else{
			string temp = buffer.substr(Seek, _Seek);
			Seek=_Seek;
			return temp;
		}
	}


	int seek(int _Seek){
		if(_Seek>buffer.length() || _Seek<=0)
			return -1;
		Seek=_Seek;
	}

};


struct openFile{
	file OpenFile[10];
	int size=0;
	int read(int fd, string &buffer, int Seek){
		if(fd>size) // NOt in open file table
			return -1;
		else{
			int status=0;
			OpenFile[fd].read(Seek, status);
			return status;
		}
	}

	int write(int fd, string &buffer, int Seek){
		if(fd>size) // NOt in open file table
			return -1;
		else{
			int status=0;
			OpenFile[fd].write(Seek, status);
			return status;
		}
	}


};



int File_Create(string path){ 
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

			child.isFile=true;

			posInode=getFreeInode();
			posDir=getFreeDataBlock(true);
			posParDir=getFirDir( parent.alloc[0]); 



			if(posDir.Count!=-1 && posInode.Count!=-1 && posParDir.Count!=-1){ // This means there is free space for a new direcotry
				// Swap posDir and posParDir{
				string temp=path.substr(found+1);
				dir NewDir(temp, posInode.Count+(posInode.Sect-3)*35);
				if(posParDir.isPar==true){
						cout << "ERROR ERROR YOU CANT HAVE MORE THAN 24 directories/files in one!\n";
						return -6;
				}
				else{
					writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
					writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				}
				dir parDir=readDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count);
				int Alloc=(posDir.Sect);
				child.alloc[0]=Alloc; //CAUSE OF BUG
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

int File_Open(string path){
}

int File_Close(int fd){
}


int File_Read(int fd, string &buffer, int size){
}


int File_Write(int fd, string &buffer, int size){
}


int File_Unlink(string path){
}
 

#endif
