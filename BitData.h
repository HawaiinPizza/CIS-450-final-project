#ifndef Bit_H
#define Bit_H
/* Data and macros */
#define SectorSize 512
#define SectorBit 4096
#define SectorNum 1000
#define inodeCount 35
#define inodeOffset 6
#define inodeSize 114
#define dirSize 132
#define dirCount 31
// f stands for File system check. This is just a one liner, that returns -1 if any thing happens. 
#define f                         \
	do {                                       \
		if(FS_Lock ){ \
			osErrMsg="E_FILE_BOOT"; \
			return -1; \
		} \
	} while (0) \
// For loop macro
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
	
	
bool range(int val, int x, int y){
	if(val>=x && val<=y)
		return true;
	return false;
}
// Macro used to ease declearing feckign disk
#define Sector std::bitset<SectorSize*8> 
#define disk(x) std::bitset<SectorSize*8> x[SectorNum]
//Using
#include <iostream>
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
// Inode 
	struct inode{
		bool isFile;
		uint size;
		uint alloc[10];
		inode(){
		}
		inode(bool _isFile, uint _size){
			isFile=_isFile;
			size=_size;
		}
		inode(bool _isFile, uint _size, uint alloc){
			isFile=_isFile;
			size=_size;
			this->alloc[0]=alloc;
		}
	};
	void inodeSet(inode *Node, int pos, int val){
		Node->alloc[pos]=val+inodeOffset;
	}
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
	bitset<114> readInodeSectBit(const Sector sect, int count){
			bitset<114> retBit;
			if(range(count, 0, SectorBit/114)){
				int start=count*inodeSize;
				int stop=start+inodeSize;
				forloop(start,stop) {
					retBit[i-start]=sect[i];
				}
				return retBit;
			}
			else{
				forloop(0,999)
					cout << "ZAKI THIS ISOUT OF BOUNDS";
				return retBit;
			}
	}
	inode readInodeSectInode(const Sector sect, int count){
		bitset<114> part1=readInodeSectBit(sect, count);
		inode part2=getBitInode(part1.to_string());
		return part2;
	}
	void writeInodeSect(Sector &sect, int count, string buffer){
			if(range(count, 0, SectorBit/114)){
				int start=count*114;
				int stop=start+114;
				bitset<114> writeBit(buffer);
				forloop(start,stop) {
					sect[i]=writeBit[i-start];
				}
			}
			else{
				forloop(0,999)
					cout << "ZAKI THIS ISOUT OF BOUNDS";
				return;
				}
	}
	void writeInodeSectInode(Sector &sect, int count, inode Write ){
		bitset<114> part1=getInodeBit(Write);
		writeInodeSect(sect, count, part1.to_string());
	}
//Dir
	struct dir{
		char Name[16];
		uint long inodePlace;
		char endOf='\0';
	};
	
	//bit and dir converson
		//Expect dirSize bytes, or 16 characters of 8 bits, and 4 bits for size 
		dir getBitDir(string BitStream){
			dir Ret;
			{ // inodePlace decleariotn
				bitset<4> inode(BitStream.substr(0,4));
				uint long ret1=inode.to_ulong();
				Ret.inodePlace=ret1;
			}
			{ // Name assigmetnation
				for(int i=0; i<16; i++){
					bitset<8> temp(BitStream.substr(4+i*8,8));
					Ret.Name[i]=(char)temp.to_ulong();
				}
			}
			return Ret;
		}
		//Expect dirSize bytes, or 16 characters of 8 bits, and 4 bits for size 
		bitset<dirSize> getDirBit(dir Dir){
			string RetStr="";
			{ // inodePlace decleariotn
				bitset<4> inode(Dir.inodePlace);
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
	// DictSect 
		bitset<dirSize> readDirSect(Sector sect, int count){
				bitset<dirSize> retBit;
				if(range(count, 0, SectorBit/dirSize)){
					int start=count*dirSize;
					int stop=start+dirSize;
					forloop(start,stop) {
						retBit[i-start]=sect[i];
					}
					return retBit;
				}
				else{
					forloop(0,999)
						cout << "ZAKI THIS ISOUT OF BOUNDS";
					return retBit;
				}
		}
		void 		writeDirSect(Sector &sect, int count, bitset<dirSize> buffer){
				if(range(count, 0, SectorBit/dirSize)){
					int start=count*dirSize;
					int stop=start+dirSize;
					forloop(start,stop) {
						sect[i]=buffer[i-start];
					}
				}
				else{
					forloop(0,999)
						cout << "ZAKI THIS ISOUT OF BOUNDS";
					return;
					}
		}

		dir 		readDirSectDir(const Sector sect, int count){
			bitset<dirSize> part1=readDirSect(sect, count);
			return getBitDir(part1.to_string());
		}

		void 		writeDirSectDir(Sector &sect, int count, dir Dir){
			bitset<dirSize>  part1=getDirBit(Dir);
			writeDirSect(sect, count, part1);
		}
	/* void writeInodeSectBit(Sector &sect, int count, inode Write ){ */
	/* 	bitset<114> part1=getInodeBit(Write); */
	/* 	writeInodeSect(sect, count, part1.to_string()); */
	/* } */
#endif

