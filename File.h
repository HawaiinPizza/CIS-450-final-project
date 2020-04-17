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

	file(pos _Loc[10],  string _name){
		forloop(0, 10){
			Loc[i]=_Loc[i];
			buffer[i]=WorkDisk[_Loc[i].Sect];
		}
		name=_name;
	}

	file(){
		forloop(0, 10){
			buffer[i]=0;
			Loc[i]=pos(-1,-1);
		}
		name="?";
		Seek=-1;
	}

	string read(int _Seek, int& status){
		if(_Seek>(512-Seek) || _Seek<=0){
			status=-1;
			return "";
		}
		else{
			string temp="";
			forloop(Seek, _Seek){
				pos Pos;
				Pos.Sect=i/4096;
				Pos.Count=i%4096;
				bitset<1> Str(buffer[Pos.Sect][Pos.Count]);
				temp+=Str.to_string();
			}
			Seek=_Seek;
			return temp;
		}
	}

	string write(int _Seek, int& status, string& BUFFER){
		if(_Seek>(512-Seek) || _Seek<=0){
			status=-1;
			return "";
		}
		else{

			forloop(Seek, _Seek){
				pos Pos;
				Pos.Sect=i/4096;
				Pos.Count=i%4096;
				bitset<1> Str(BUFFER[i]);
				buffer[Pos.Sect][Pos.Count]=Str[0];
				WorkDisk[Loc[Pos.Sect].Sect][Pos.Count]=Str[0];
			}
			Seek=_Seek;
			return BUFFER;
		}
	}


	int seek(int _Seek){
		if(_Seek>(512-Seek) || _Seek<=0){
			return -1;
		}
		Seek=_Seek;
		return 0;
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
			buffer=OpenFile[fd].read(_Seek, status);
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
		OpenFile[fd].Seek=0;
		isValid[fd]=false;
		size--;
	}


	int seek(int fd, int offset){
		if(isValid[fd]){
			if(OpenFile[fd].seek(offset))
				return fd;
			else
				return -1;
		}
		else
			return -1;
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

//Description: Create a file to the path
///Pre-condition: The parent exist and there is nothing that sahres the same name
//Post-condition: Create a file to the path, with ti's corresponding inode.

int File_Create(string path){ 
	f;
	inode child=getInode(path);
	size_t found = path.find_last_of("/");
	inode parent=getInode(path.substr(0,found));
	if(parent.size==inode().size){ // Parent does not exist
		osErrMsg="E_FILE_CREATE";
		Log(-1, path); 
		Log(0, path); 
		return 0;
	}
	else{
		if (child.size != inode().size){ // Child alreayd exist
			osErrMsg="E_FILE_CREATE";
			Log(-1, path); 
			Log(0, path); 
			return 0; 
		}
		else if (path.length()>256 ){ // Checking if path exceeds 256 characters
			osErrMsg="E_FILE_CREATE";
			Log(-1, path); 
			Log(0, path); 
			return 0;
		}
		else if( WorkDisk[1].to_ulong()>=100){ // There are 100 or over files/direcotires already
			osErrMsg="E_FILE_CREATE";
			Log(-1, path); 
			Log(0, path); 
			return 0;
		}
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
					osErrMsg="E_FILE_CREATE";
					Log(-1, path); 
					Log(0, path); 
					return 0;
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




				return 0;
			}
			else{ // There is no more space for a new directory
				osErrMsg="E_FILE_CREATE";
				Log(-1, path); 
				Log(0, path); 
				return 0;
			}


			}

		}
	}
	//Description: Set the file to the opnefie table. 
	///Pre-condition: The file exist and the openfiletable does not exceed size
	//Post-condition: Opne file 
	int File_Open(string path){
	f;
		inode fileNode=getInode(path);
		if(fileNode.alloc[0]==1023 || fileNode.alloc[0]==-1 || fileNode.isFile==false){
			osErrMsg="E_NO_SUCH_FILE";
			Log(-1, path); 
			Log(0, path); 
			return 0;
		}
		else{
			if( openFileTable.isFileOpened(path) ){ // FIle is already opened

				osErrMsg="E_ALREADY_OPENED";
				Log(-1, path); 
				Log(0, path); 
				return 0;
			}
			else if ( openFileTable.size>=10){
				osErrMsg="E_FILE_CREATE";
				return -3;
			}
			else{
				// Setup file
				pos Temp[10];
				forloop(0,10){
					Temp[i].Sect=fileNode.alloc[i];
					Temp[i].Count=0;
				}

				int _alloc[10];
				file File(Temp, path);
				int _temp=openFileTable.addFileOpen(File);
				if(_temp)
					return _temp;
				else{
					osErrMsg="E_FILE_OPENED";
					Log(-1, path); 
					Log(0, path); 
					return 0;
				}



			}



		}


	}

	//Description: Close the file, or just remove the file from the openfiletable
	///Pre-condition: The fd is valid in teh openfieltable
	//Post-condition: Remove file from openfiletable
	int File_Close(int fd){
	f;
		if(! openFileTable.isValid[fd]){ // Fd not here.
			osErrMsg="E_CLOSE_BAD_FD";
			string buffer=to_string(fd);
			Log(-1, buffer);
			return -1;
		}
		else{
			openFileTable.rmFileOpen(fd);
		}
		string buffer=to_string(fd);
		Log(0, buffer);
		return 0;
	}

	//Description: Read a file from teh openfile table. 
	///Pre-condition: The buffer is equal to size and fd exist
	//Post-condition: The buffer gets modfied by the file by size
	int File_Read(int fd, string &buffer, int size){
	f;
		int i=openFileTable.read(fd, buffer, size);
		if(i==0){
			string buffer=to_string(fd);
			buffer +=" "+buffer + " ";
			buffer+=to_string(size);
			Log(i, buffer);
			return i;
		}
		else{
			osErrMsg="E_CLOSE_BAD_FD";
			string buffer=to_string(fd);
			buffer +=" "+buffer + " ";
			buffer+=to_string(size);
			Log(i, buffer);
			return -1;
		}
	}

	//Description: write a file from teh openfile table. 
	///Pre-condition: The buffer is equal to size and fd exist
	//Post-condition: The file gets modfied by the buffer by size

	int File_Write(int fd, string &buffer, int size){
	f;
		int i=openFileTable.write(fd, buffer, size);
		if(i==0){

			string buffer=to_string(fd);
			buffer +=" "+buffer + " ";
			buffer+=to_string(size);
			Log(size, buffer);
			return size;
		}
		else if (i==2){
			osErrMsg="E_FILE_TOO_BIG";
			string buffer=to_string(fd);
			buffer +=" "+buffer + " ";
			buffer+=to_string(size);
			Log(-1, buffer);
			return -1;
		}
		else{
			string buffer=to_string(fd);
			buffer +=" "+buffer + " ";
			buffer+=to_string(size);
			Log(-1, buffer);
			osErrMsg="E_CLOSE_BAD_FD";
			return -1;
		}
	}


	//Description: Delete the file 
	///Pre-condition: The file itself exist AND it's nonempty. Input of a path
	//Post-condition: Delete the file and it's corresponding entry in it's parent file and inode.
	int File_Unlink(string path){
	f;
		inode delNode=getInode(path);
		size_t found = path.find_last_of("/");
		inode parent=getInode(path.substr(0,found));
		if(delNode.size==-1){ // File does not exist
			osErrMsg="E_NO_SUCH_FILE";
			Log(-1, path);
			return -1;
		}
		else if ( openFileTable.isFileOpened(path)  ){ //Path is not empty. 
			osErrMsg="E_FILE_OPENED";
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
		Log(0, path);
		return 0;
	}

	//Description: Move the file by seek. Not really used, since openfiletable already has it's own seek function.
	///Pre-condition: offset is within range, and fd exist
	//Post-condition: move the file by the offset.
	int File_Seek(int fd, int offset){
	f;
		//Not used, but it works Stener. Because it just invokes openfiletable seek.
		string _temp=to_string(fd);
		_temp+="\t";
		_temp+=offset;
		int status=openFileTable.seek(fd, offset);
		Log(status, _temp);
		return status;
	}

#endif
