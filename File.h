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
#include "Disk.h"

struct file{
	pos Loc[10];
	bitset<4096> buffer[10];
	string name;
	int Seek=0;
	bool isValid[10];

	file(pos _Loc[10],  bitset<4096> _buffer[10], string _name){
		isValid[0]=true;
		forloop(0, 10){
			Loc[i]=_Loc[i];
			if(_buffer[i]!=0){
				buffer[i]=_buffer[i];
				isValid[i]=true;
			}
			else{
				buffer[i]=0;
				isValid[i]=false;
			}
		}
		name=_name;
	}

	file(){
		forloop(0, 10){
			buffer[i]=0;
			isValid[i]=false;
			Loc[i]=pos(-1,-1);
		}
		name="?";
		Seek=-1;
	}

	int size(){
		int count=0;
		forloop(0, 10){
			if(isValid[i]){
				count++;
			}
		}
		return count;

	}
	string read(int _Seek, int& status){
		if(_Seek>size()*4096 || _Seek<=0){
			status=-1;
			return "";
		}
		else{
			string temp="";
			for(int i=0; i<size(); i++){ 
				//forloop2(Seek, _Seek){
				forloop2(Seek, _Seek){
					cout << i << ':' << j << '\t' << buffer[i][j] << '\n' ;
					temp += buffer[i][j];
				}
				cout << endl;
			}
			Seek=_Seek;
			return temp;
		}
	}

	string write(int _Seek, int& status, string& _buffer){
		if(_Seek>size()*4096 || _Seek<=0){
			status=-1;
			return "";
		}
		else{
			// Lmao TODO make sure this writes to both openFile AND file locaiotn

			forloop(0, size()){
				forloop2(Seek, _Seek){
					cout << "GETFUCK\t" << i << ':' << j << '\t' << buffer[i][j] << '\n' ;
					buffer[i][j]=_buffer[i*4096+j];
					WorkDisk[Loc[i].Sect][j]=_buffer[i*4096+j];
				}
			}
			Seek=_Seek;
			return _buffer;
		}
	}


	int seek(int _Seek){
		if(_Seek>size()*4096 || _Seek<=0)
			return -1;
		Seek=_Seek;
	}

};


struct openFile{
	file OpenFile[10];
	bool isValid[10];
	int size=0;

	openFile(){
		forloop(0,10){
			OpenFile[i]=file();
			isValid[i]=false;
		}
	}

	int read(int fd, string &buffer, int _Seek){

		if(!isValid[fd]) // NOt in open file table
			return -1;
		else{
			int status=0;
			OpenFile[fd].read(_Seek, status);
			return status;
		}
	}

	int write(int fd, string &buffer, int _Seek){
		if(!isValid[fd]) // NOt in open file table
			return -1;
		else{
			int status=0;
			OpenFile[fd].write(_Seek, status, buffer);
			return status;
		}
	}

	int addFileOpen(file File){
		OpenFile[size]=File;
		forloop(0,10){
			if(!isValid[i]){
				isValid[i]=true;
				size++;
				return i;
				break;
			}
		}
		return -1;
	}

	void rmFileOpen(int fd){
		cout << "RM\t" << fd << endl;
		OpenFile[fd].Seek=0;
		isValid[fd]=false;
		size--;
	}



	bool isFileOpened(string name){
		forloop(0, size){
			if(isValid[i] && OpenFile[i].name==name) // Make sure name is all of it, not just the basename
				return true;
		}
		return false;
	}


};


openFile openFileTable;

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
			child.size=0;

			dir Dir;
			pos posParDir;
			pos posInode;

			child.isFile=true;

			posInode=getFreeInode();
			pos posFile[10];
			getFreeDataBlock(0, posFile);
			posParDir=getFirDir( parent.alloc[0]); 

			forloop(0,10){
				if ( posFile[i].Count==-1){
					return -3;
				}

			}


			if(posFile[0].Count!=-1 && posInode.Count!=-1 && posParDir.Count!=-1){ // This means there is free space for a new direcotry
				// Swap posDir and posParDir{
				string temp=path.substr(found+1);
				dir NewDir(temp, posInode.Count+(posInode.Sect-3)*35);
				writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
				dir parDir=readDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count);
				forloop(0,10){
					int Alloc=(posFile[i].Sect);
					child.alloc[i]=Alloc; 
				}
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
		inode fileNode=getInode(path);
		if(fileNode.alloc[0]==1023 || fileNode.alloc[0]==-1 || fileNode.isFile==false){
			return -1;
		}
		else{
			if( openFileTable.isFileOpened(path) ) // FIle is already opened
				return -2;
			else if ( openFileTable.size>=10)
				return -3;
			else{
				// Setup file
				pos Temp[10];
				forloop(0,10){
					Temp[i].Sect=fileNode.alloc[i];
					Temp[i].Count=0;
				}
				bitset<4096> newBuffer[10];
				for(int i=0; i<10;  i++){
					newBuffer[i]=WorkDisk[Temp[i].Sect];
				}
				file File(Temp, newBuffer, path);
				openFileTable.addFileOpen(File);

				return 0;
				/* file _File(Temp, newBuffer, path); */
				/* int temp=openFileTable.addFileOpen(_File); */
				/* return temp; */


			}



		}

	}

	int File_Close(int fd){
		if(openFileTable.size<=fd) // Fd not here.
			return -1;
		else{
			openFileTable.rmFileOpen(fd);
		}
		return 0;
	}


	int File_Read(int fd, string &buffer, int size){
		return openFileTable.read(fd, buffer, size);
	}


	int File_Write(int fd, string &buffer, int size){
		return openFileTable.write(fd, buffer, size);
	}


	int File_Unlink(string path){
		return 0;
	}


#endif
