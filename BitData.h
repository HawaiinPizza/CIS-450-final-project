#ifndef Bit_H
#define Bit_H
/* Data and macros */
#define SectorSize 512
#define SectorBit 4096
#define SectorNum 1000
#define inodeCount 35
#define inodeOffset 6
#define inodeSize 114
#define dirSize 160
#define dirCount 25
#include <fstream>
#include "FS.h"
std::ofstream LogFile;



// Forloop, from x to y
#define forloop(x,y) \
	for(int i=x; i<y; i++) \

#define forloop2(x,y) \
	for(int j=x; j<y; j++) \

#define backloop(x,y) \
	for(int i=y; i>x; i--) \

#define backloop2(x,y) \
	for(int j=y; j>x; j--) \

#define ifelse(cond,x,y)\
	if(cond)\
	x; \
	else\
	y; \

#define foreach(x) \
	for(auto i : x)


//Description: Given a range of values and a value, see if the input is in range
///Pre-condition: a value, and the lower and upper limit
//Post-condition: a ture false statemtn, represtning if it's within bounds or not.
bool range(int val, int x, int y){
	if(val>=x && val<=y)
		return true;
	return false;
}
// Macro used to ease declearing  disk
#define Sector std::bitset<SectorSize*8> 
#define disk(x) std::bitset<SectorSize*8> x[SectorNum]
//Using
#include <iostream>
#include <algorithm>
#include <cassert>
#include <queue>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
using namespace std;
//The Disk
disk(ExtDisk); 
disk(WorkDisk);
string osErrMsg="";
string diskErrMsg="";



bool printDisk=false; //Used to print each bit in both disks

//Description: Print to the log the currrent command and it's status.
//PreCondition: The status code to print, and LogFile is opened
//PostCondition: Wirte to LogFile the current commadn
#define Log(status, string) \
	LogFile << __FUNCTION__ << ' ' << status << "\t" << string << std::endl; \
	/* if(printDisk){ \ */
	/* 	for(int i=0; i<SectorNum; i++){ \ */
	/* 		LogFile << "ExtDisk  at Sector " << i << ":\t" << ExtDisk[i] << std::endl; \ */
	/* 		LogFile << "WorkDisk at Sector " << i << ":\t" << ExtDisk[i] << std::endl; \ */
	/* 	} \ */
	/* 	LogFile << std::endl; \ */
	/* } */  

bool FS_Lock=true; // In here cause I don't know how to make it run without issues in FS.h.
// f stands for File system check. This is just a one liner, that returns -1 if any thing happens. 
#define f                         \
	do {                                       \
		if(FS_Lock ){ \
			Log(-1, "File system is locked"); \
			osErrMsg="E_FILE_BOOT"; \
			return -1; \
		} \
	} while (0) \


// Inode 

//Description: Inode struuctre, used for reading and writing inode blocks
struct inode{
	bool isFile;
	uint size;
	uint alloc[10];
	inode(){
		size=-1;
		isFile=true;
		forloop(0, 10){
			alloc[i]=-1;
		}
	}
	inode(bool _isFile, uint _size){
		isFile=_isFile;
		size=_size;
		forloop(0, 10){
			alloc[i]=-1;
		}
	}
	inode(bool _isFile, uint _size, uint alloc){
		isFile=_isFile;
		size=_size;
		this->alloc[0]=alloc;
		forloop(1, 10){
			this->alloc[i]=-1;
		}
	}
};


//Description: Given an inode, return the bitset represntation
///Pre-condition: An inode
//Post-condition: bitset represntation
bitset<114> getInodeBit(inode Inode){//Retrun 114 bits
	string RetStr; //So we can concate instead of dealing with how bits are setup in bitset
	{ // alloc decleartion
		for(int i=0; i<10; i++){ // Loop through each alloc
			bitset<10> tempAlloc(Inode.alloc[i]);
			RetStr+=tempAlloc.to_string();
		}
	}
	{ // SIze decleartion
		bitset<13> temp(Inode.size);
		RetStr+=temp.to_string();
	}
	{// Type decleartion
		bitset<1> temp(Inode.isFile);
		RetStr+=temp.to_string();
	}
	bitset<114> Ret(RetStr);
	return Ret;
}
//Description: Given a string of bits, find the inode reprsreatnion
///Pre-condition: A string of Bits
//Post-condition: an INode
inode getBitInode(string BitStream){// Type // Size // 10*10 of which bits are allociated to it. 
	inode Ret;
	{ // alloc decleartion
		for(int i=0; i<10; i++){
			bitset<10> temp(BitStream.substr(i*10, 10));
			Ret.alloc[i]=temp.to_ulong();
		}
	}
	{ // SIze decleartion
		bitset<13> temp(BitStream.substr(100,13));
		Ret.size=temp.to_ulong();
	}
	{// Type decleartion
		bitset<1> temp(BitStream.substr(113,1));
		Ret.isFile=temp.to_ulong();
	}
	return Ret;
}
//Description: Given a bitset, convert to an inode
///Pre-condition: Bitset
//Post-condition: Inode
inode getBitInode(bitset<114> bit){// Type // Size // 10*10 of which bits are allociated to it. 
	string BitStream=bit.to_string();
	inode Ret;
	{ // alloc decleartion
		for(int i=0; i<10; i++){
			bitset<10> temp(BitStream.substr(i*10, 10));
			Ret.alloc[i]=temp.to_ulong();
		}
	}
	{ // SIze decleartion
		bitset<13> temp(BitStream.substr(100,13));
		Ret.size=temp.to_ulong();
	}
	{// Type decleartion
		bitset<1> temp(BitStream.substr(113,1));
		Ret.isFile=temp.to_ulong();
	}
	return Ret;
}
	//Description: Given a sector and which part of it, return an inode repretatation in bits
	///Pre-condition: Sector and which inode part
	//Post-condition: Return a bitstream

bitset<114> readInodeSectBit(const Sector sect, int count){
	bitset<114> retBit;
	assert(range(count, 0,SectorBit/114));
	int start=count*inodeSize;
	int stop=start+inodeSize;
	forloop(start,stop) {
		retBit[i-start]=sect[i];
	}
	return retBit;
}
	//Description: Given a sector and which part of it, return an inode repretatation in inode
	///Pre-condition: Sector and which inode part
	//Post-condition: Return a inode
inode readInodeSectInode(const Sector sect, int count){
	bitset<114> part1=readInodeSectBit(sect, count);
	inode part2=getBitInode(part1.to_string());
	return part2;
}
	//Description: Given a sector and part, write to it
	///Pre-condition: Sector, count, and buffer
	//Post-condition: Write to sect[count] the value of buffer

void writeInodeSect(Sector &sect, int count, string buffer){
	assert(range(count, 0, SectorBit/114));
	int start=count*114;
	int stop=start+114;
	bitset<114> writeBit(buffer);
	forloop(start,stop) {
		sect[i]=writeBit[i-start];
	}
}
//Description: Combine getInodeBit with writeIndoeSect
	///Pre-condition: Sector[count] and indoe
	//Post-condition: Write inode to Sector[count]

void writeInodeSectInode(Sector &sect, int count, inode Write ){
	bitset<114> part1=getInodeBit(Write);
	writeInodeSect(sect, count, part1.to_string());
}
//Dir
struct dir{
	char Name[16];
	uint long  inodePlace;
	const char endOf='\0';
	dir(){
		forloop(0,15){
			Name[i]=endOf;
		}
		inodePlace=-1;
	}
	dir(string _Name, uint long _inodePlace){
		forloop(0,15){
			Name[i]=endOf;
		}
		for(int i=0; i<15 ; i++){

			if(i<_Name.length())
				Name[i]=_Name[i];

		}
		Name[15]=endOf;
		inodePlace=_inodePlace;
	}
};

//Description: Given a string of bits, return a directory
///Pre-condition: string of bits
//Post-condition: directory
//Expect dirSize bytes, or 16 characters of 8 bits, and 4 bits for size 
dir getBitDir(string BitStream){
	dir Ret;
	{ // inodePlace decleariotn
		bitset<32> inode(BitStream.substr(0,32));
		uint long ret1=inode.to_ulong();
		Ret.inodePlace=ret1;
	}
	{ // Name assigmetnation
		for(int i=0; i<16; i++){
			bitset<8> temp(BitStream.substr(32+i*8,8));
			Ret.Name[i]=(char)temp.to_ulong();
		}
	}
	return Ret;
}
//Description: Given a bitset, return a directory
///Pre-condition:bits
//Post-condition: directory
dir getBitDir(bitset<dirSize> b){
	string BitStream=b.to_string();
	dir Ret;
	{ // inodePlace decleariotn
		bitset<32> inode(BitStream.substr(0,32));
		uint long ret1=inode.to_ulong();
		Ret.inodePlace=ret1;
	}
	{ // Name assigmetnation
		for(int i=0; i<16; i++){
			bitset<8> temp(BitStream.substr(32+i*8,8));
			Ret.Name[i]=(char)temp.to_ulong();
		}
	}
	return Ret;
}

//Description: Given a direcotry, return a bitset represtnation
///Pre-condition: Directory
//Post-condition: bitset reprenation
//Expect dirSize bytes, or 16 characters of 8 bits, and 4 bits for size 
bitset<dirSize> getDirBit(dir Dir){
	string RetStr="";
	{ // inodePlace decleariotn
		bitset<32> inode(Dir.inodePlace);
		RetStr+=inode.to_string();
	}
	{ // Name assigmetnation
		for(int i=0; i<16; i++){
			bitset<8> temp(Dir.Name[i]);
			RetStr+=temp.to_string();
		}

	}
	bitset<dirSize> Ret(RetStr);
	return Ret;
}
//Description: Given a sector and count, read the bitset represtantion
	///Pre-condition: sect[count]
	//Post-condition: the bitset of sect[count]

bitset<dirSize> readDirSect(Sector sect, int count){
	bitset<dirSize> retBit;
	assert(range(count, 0, SectorBit/dirSize));
		int start=count*dirSize;
		int stop=start+dirSize;
		forloop(start,stop) {
			retBit[i-start]=sect[i];
		}
		return retBit;
}

//Description: Given a sector and whcih part of it, write buffer to it
	///Pre-condition: sect[count] and a bitset of buffer
	//Post-condition: write buffer to sect[count]
void	writeDirSect(Sector &sect, int count, bitset<dirSize> buffer){
	assert(range(count, 0, SectorBit/dirSize));
		int start=count*dirSize;
		int stop=start+dirSize;
		forloop(start,stop) {
			sect[i]=buffer[i-start];
		}
}
//Description: Given a sector and count, read an direcotry from it
	///Pre-condition: sect[count]
	//Post-condition:  the dir in sect[count]
dir readDirSectDir(const Sector sect, int count){
	bitset<dirSize> part1=readDirSect(sect, count);
	return getBitDir(part1.to_string());
}

	//Description: Write to a sect of count of a direcotry
	///Pre-condition:  sect[count] and a direcotry
	//Post-condition: write direcotry to sect[count]
void 	writeDirSectDir(Sector &sect, int count, dir Dir){
	bitset<dirSize>  part1=getDirBit(Dir);
	writeDirSect(sect, count, part1);
}

struct pos{
	int Sect;
	int Count;
	bool isPar=false;
	pos(){
		Sect=-1;
		Count=-1;
	}
	pos(int _Sect, int _Count){
		Sect=_Sect;
		Count=_Count;
	}

	int sectToPos(){
		return (Sect-3)*35+Count;
	}
};

//Description: get the indoe value given a path. The most important function, and if this function doesn't work, than the whole program doesn't work
	///Pre-condition: Given a string of absoulte path
	//Post-condition: Return the inode that should be tehre

inode getInode(string path){

	bool isRoot=false;
	queue<string> RetStr;
	inode Ret;
	RetStr.push("/");
	{ // See if root and if not, get the next child
		string _testStr="";
		{
			if(path[path.length()-1]!='/')
				path+='/';
			string tempStr="";
			for(int i=1; i<path.length(); i++){
				if(path[i]=='/'){
					RetStr.push(tempStr);
					tempStr="";
				}
				else{
					tempStr+=path[i];
					_testStr+=path[i];
				}
			}
		}
	}

	// Getting the node here.
	bool stop=false;
	int Sect=6; //There is where root dictionary will be
	int count=0;
	while(!RetStr.empty() && !stop && count<10){
		string find=RetStr.front(); RetStr.pop();
		forloop(0, dirCount){ // This is code for finding something in root.
			auto a= readDirSect(WorkDisk[Sect], i); 
			if(a!=0){
				dir b=getBitDir(a);

				if(b.Name==find && !RetStr.empty()) { // This isn't what we're looking for. Go to antoher stage
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					inode temp=readInodeSectInode(WorkDisk[_sect], _place);
					Sect=temp.alloc[0];
					break;

				}
				else if(b.Name == find && RetStr.empty()){
					stop=true;
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					Ret=readInodeSectInode(WorkDisk[_sect], _place);
					return Ret;

				}
				else{
					if(i==dirCount-1){
						auto parent= readDirSectDir(WorkDisk[Sect], 0); 

						pos _inode((parent.inodePlace-3)*35, parent.inodePlace%35);
						inode _temp = readInodeSectInode(WorkDisk[_inode.Sect], _inode.Count);
					}
				}

			}


		}

	}

	// At this point we didn't find it. return a null inode.
	return inode();
}


//Description: Anotehr crucerial helper function. This deals with finding the first indoe taht's avaiable to be used
//NOTE TO STEINER AND OTHERS: There are 35 inodes within a sector, and there are 3 sectors. That means there's 105 indoes avaiable. So you don't have to test for indoes exceeding indoe count.
	///Pre-condition: There is a working disk
	//Post-condition: Returns the first avaialbe inode

pos getFreeInode(){
	pos posInode;
	bool STOP=false;
	for(int i=3; i<6 && !STOP; i++){ // Finding the first free inode space
		forloop2(0,34){
			if(readInodeSectBit(WorkDisk[i], j)==0){
				posInode=pos(i,j);
				STOP=true;
			}
			else{
			}
			if(posInode.Count!=-1){ // Stop looking, since we already found one.
				break;
			}
		}
	}
	return posInode;
}


//Description: This funciton is kinda uselss, since direcotries only ahve 24 files/directories within them. Otherwise, find the first datablock that's freee to be sued
//Arguemtns were going to be used to distushin, but now it's not.
	///Pre-condition: There is a working disk
	//Post-condition: Returns the first avaialbe datalbock
void getFreeDataBlock(int temp, pos* Temp){
	forloop(0, 10){
		Temp[i]=pos();
	}
	int Count=0;
	vector<int> alreadyIn;
	forloop(3, 6){
		forloop2(0, inodeCount){
			inode temp=readInodeSectInode(WorkDisk[i],j);
			for(int k=0; k<10; k++){
				if(temp.alloc[k]!=0)
					alreadyIn.push_back(temp.alloc[k]);
			}
		}
	}

	for(int i=6; i<SectorNum && Count!=10; i++){ // Finding the first free dir space
		if(WorkDisk[i]==0){
			if(find(alreadyIn.begin(), alreadyIn.end(), i) != alreadyIn.end()){
			}
			else{
				Temp[Count]=pos(i,0);
				Count++;
			}
		}
	}

}

//Description: Get the first datablock that's free
	///Pre-condition: There is a working disk
	//Post-condition: First datablock taht's free, or if getNext is false, the second one that's free.

pos getFreeDataBlock(bool getNext){
	pos posDir;
	for(int i=6; i<SectorNum; i++){ // Finding the first free dir space
		if(WorkDisk[i]==0 && getNext){
			posDir=pos(i,0);
			break;
		}
		else if (WorkDisk[i]==0 && !getNext)
			getNext=true;
	}
	return posDir;
}

//Description: Get the first direcotry taht is free.
	///Pre-condition: WorkDisk[_sect] has a direcotry
	//Post-condition: Returns the first direcotry that is free in it.
pos getFirDir(int _sect){ // _sect is the sector where the directory si in.
	pos posParDir;
	forloop2(0,dirCount){
		bitset<dirSize> bitStream( readDirSect(WorkDisk[_sect], j));
		if(bitStream==0){ // INCLUDE CHECK THAT IF THIS DIR RUNS OUT OF SPACE, YOU APPEND A NEW SPACE
			posParDir=pos(_sect, j );
		}
		if(posParDir.Count!=-1) // Stop looking, since we already found one.
			break;
	}


	if(posParDir.Count==-1){
		posParDir.isPar=true;
	}
	return posParDir;
}




/* // datablcok */
/* struct datablock{ */
/* 	dir Dir[10]; */
/* 	inode Inode; */
/* 	int count=1; */
/* 	datablock(){ */
/* 	} */
/* 	datablock(inode _Inode, dir _Dir){ */
/* 		Dir[0].inodePlace=_Dir.inodePlace; */
/* 		forloop(0, 15){ */
/* 			Dir[0].Name[i]=_Dir.Name[i]; */
/* 		} */
/* 		Inode=_Inode; */
/* 	} */

/* 	void pushDir(dir _Dir, pos _NewSect){ */
/* 		if(count<10){ */
/* 			Dir[count].inodePlace=_Dir.inodePlace; */
/* 			forloop(0, 15){ */
/* 				Dir[count].Name[i]=_Dir.Name[i]; */
/* 			} */
/* 			Inode.alloc[count] = _NewSect.sectToPos(); */
/* 			count++; */
/* 		} */
/* 	} */


/* pos getInodePos(string path){ */

/* 	pos retPos; */
/* 	bool isRoot=false; */
/* 	queue<string> RetStr; */
/* 	inode Ret; */
/* 	RetStr.push("/"); */
/* 	{ // See if root and if not, get the next child */
/* 		string _testStr=""; */
/* 		{ */
/* 			if(path[path.length()-1]!='/') */
/* 				path+='/'; */
/* 			string tempStr=""; */
/* 			for(int i=1; i<path.length(); i++){ */
/* 				if(path[i]=='/'){ */
/* 					RetStr.push(tempStr); */
/* 					tempStr=""; */
/* 				} */
/* 				else{ */
/* 					tempStr+=path[i]; */
/* 					_testStr+=path[i]; */
/* 				} */
/* 			} */
/* 		} */
/* 		//cout << _testStr << "\t FULL PATH" << endl; */
/* 	} */

/* 	// Getting the node here. */
/* 	bool stop=false; */
/* 	int Sect=6; //There is where root dictionary will be */
/* 	while(!RetStr.empty() && !stop){ */
/* 		string find=RetStr.front(); RetStr.pop(); */
/* 		/1* cout << "Gonna hit them with this\t" << find << endl; *1/ */
/* 		forloop(0, dirCount){ // This is code for finding something in root. */
/* 			auto a= readDirSect(WorkDisk[Sect], i); */ 
/* 			if(a!=0){ */
/* 				dir b=getBitDir(a); */
/* 				/1* cout << find << ":" << b.Name << '\t'; *1/ */

/* 				if(b.Name==find && !RetStr.empty()) { // This isn't what we're looking for. Go to antoher stage */
/* 					/1* cout << "CLUE\t" ; *1/ */
/* 					/1* 			newdir.inodePlace=(posInode.sect-3)*inodeCount +  posInode.count; *1/ */
/* 					int _sect=3+b.inodePlace/35; */
/* 					int _place=b.inodePlace%35; */
/* 					inode temp=readInodeSectInode(WorkDisk[_sect], _place); */
/* 					/1* cout << *1/ */ 
/* 					/1* 	"inode\t" << b.inodePlace << '\t' << *1/ */
/* 					/1* "SECT\t" << _sect << '\t' *1/ */
/* 					/1* 	"Place\t" << _place << '\t' << *1/ */
/* 					/1* 	"Alloc\t" << temp.alloc[0] << '\t' << *1/ */
/* 					/1* 	"Names\t" << b.Name << '\t' << find *1/ */
/* 					/1* << endl; *1/ */

/* 					/1* ; *1/ */
/* 					Sect=temp.alloc[0]; */
/* 					break; */

/* 				} */
/* 				else if(b.Name == find && RetStr.empty()){ */
/* 					stop=true; */
/* 					int _sect=3+b.inodePlace/35; */
/* 					int _place=b.inodePlace%35; */
/* 					pos Ret=pos(_sect, _place); */
/* 					return Ret; */
/* 					/1* cout << "Find it my boy\t" << *1/ */
/* 					/1* 	_sect << '\t' << *1/ */
/* 					/1* 	_place << '\t' << *1/ */ 
/* 					/1* 	endl; *1/ */


/* 					break; */
/* 				} */

/* 				/1* else *1/ */
/* 				/1* 	cout << "STILL SEARCHING FOR HER\t"; *1/ */ 

/* 			} */
/* 			else{ */
/* 			} */

/* 		} */
/* 	} */

/* 	// At this point we didn't find it */
/* 	return pos(); */
/* } */


#endif

