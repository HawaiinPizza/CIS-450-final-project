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
// Inode 
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
				cout << "\nZAKI THIS ISOUT OF BOUNDS: " << count << "\n";
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
				cout << "\nZAKI THIS ISOUT OF BOUNDS: " << count << "\n";
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
	
	//bit and dir converson
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
					cout << "\nDIR READ ZAKI THIS ISOUT OF BOUNDS: " << count << '\t' << dirCount << '\t' << SectorBit/dirSize << "\n";
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
					cout << "\nDIR WRITE ZAKI THIS ISOUT OF BOUNDS: " << count << "\n";
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

// getInoe and pos
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
};

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
		//cout << _testStr << "\t FULL PATH" << endl;
	}

	// Getting the node here.
	bool stop=false;
	int Sect=6; //There is where root dictionary will be
	while(!RetStr.empty() && !stop){
		string find=RetStr.front(); RetStr.pop();
		/* cout << "Gonna hit them with this\t" << find << endl; */
		forloop(0, dirCount){ // This is code for finding something in root.
			auto a= readDirSect(WorkDisk[Sect], i); 
			if(a!=0){
				dir b=getBitDir(a);
				/* cout << find << ":" << b.Name << '\t'; */

				if(b.Name==find && !RetStr.empty()) { // This isn't what we're looking for. Go to antoher stage
					/* cout << "CLUE\t" ; */
					/* 			newdir.inodePlace=(posInode.sect-3)*inodeCount +  posInode.count; */
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					inode temp=readInodeSectInode(WorkDisk[_sect], _place);
					/* cout << */ 
					/* 	"inode\t" << b.inodePlace << '\t' << */
						/* "SECT\t" << _sect << '\t' */
					/* 	"Place\t" << _place << '\t' << */
					/* 	"Alloc\t" << temp.alloc[0] << '\t' << */
					/* 	"Names\t" << b.Name << '\t' << find */
						/* << endl; */

					/* ; */
					Sect=temp.alloc[0];
					break;

				}
				else if(b.Name == find && RetStr.empty()){
					stop=true;
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					Ret=readInodeSectInode(WorkDisk[_sect], _place);
					/* cout << "Find it my boy\t" << */
					/* 	_sect << '\t' << */
					/* 	_place << '\t' << */ 
					/* 	endl; */
					return Ret;

					break;
				}
				/* else */
				/* 	cout << "STILL SEARCHING FOR HER\t"; */ 

			}

		}
	}

	// At this point we didn't find it
	return inode();
}

pos getInodePos(string path){

	pos retPos;
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
		//cout << _testStr << "\t FULL PATH" << endl;
	}

	// Getting the node here.
	bool stop=false;
	int Sect=6; //There is where root dictionary will be
	while(!RetStr.empty() && !stop){
		string find=RetStr.front(); RetStr.pop();
		/* cout << "Gonna hit them with this\t" << find << endl; */
		forloop(0, dirCount){ // This is code for finding something in root.
			auto a= readDirSect(WorkDisk[Sect], i); 
			if(a!=0){
				dir b=getBitDir(a);
				/* cout << find << ":" << b.Name << '\t'; */

				if(b.Name==find && !RetStr.empty()) { // This isn't what we're looking for. Go to antoher stage
					/* cout << "CLUE\t" ; */
					/* 			newdir.inodePlace=(posInode.sect-3)*inodeCount +  posInode.count; */
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					inode temp=readInodeSectInode(WorkDisk[_sect], _place);
					/* cout << */ 
					/* 	"inode\t" << b.inodePlace << '\t' << */
						/* "SECT\t" << _sect << '\t' */
					/* 	"Place\t" << _place << '\t' << */
					/* 	"Alloc\t" << temp.alloc[0] << '\t' << */
					/* 	"Names\t" << b.Name << '\t' << find */
						/* << endl; */

					/* ; */
					Sect=temp.alloc[0];
					break;

				}
				else if(b.Name == find && RetStr.empty()){
					stop=true;
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					pos Ret=pos(_sect, _place);
					return Ret;
					/* cout << "Find it my boy\t" << */
					/* 	_sect << '\t' << */
					/* 	_place << '\t' << */ 
					/* 	endl; */


					break;
				}
				/* else */
				/* 	cout << "STILL SEARCHING FOR HER\t"; */ 

			}

		}
	}

	// At this point we didn't find it
	return pos();
}

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
				/* cout << readInodeSectInode(WorkDisk[i], j).size << endl; */
			}
			if(posInode.Count!=-1){ // Stop looking, since we already found one.
				break;
			}
		}
	}
	return posInode;
}


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

//int _sect=6+parent.alloc[i];
pos getFirDir(int _sect){ // _sect is the sector where the directory si in.
	pos posParDir;
	for(int i=0; i<9; i++){ // Finding the space for the parent dictionary
		forloop2(0,dirCount){
			bitset<dirSize> bitStream( readDirSect(WorkDisk[_sect], j));
			if(bitStream==0){ // INCLUDE CHECK THAT IF THIS DIR RUNS OUT OF SPACE, YOU APPEND A NEW SPACE
				/* int _sect=3+b.inodePlace/35; */
				/* int _place=b.inodePlace%35; */
				/* cout << _sect << '\t' << j << "\t Values to node\n"; */
				posParDir=pos(_sect, j );
			}
			if(posParDir.Count!=-1) // Stop looking, since we already found one.
				break;
		}

		if(posParDir.Count!=-1) // Stop looking, since we already found one.
			break;
		else{
			
			// Get parent dicitoanry (the first directory in the direcotry node)'s indoe
			uint _pos= readDirSectDir(WorkDisk[_sect], 0).inodePlace;
			pos _posInode(3+_pos/35, _pos%35);
			inode temp= readInodeSectInode(WorkDisk[_posInode.Sect], _posInode.Count);


			if(temp.alloc[i+1]==1023) { // Check the next one, and if it's not real, make a new locatoin for it.
				bitset<dirSize> bitStream( readDirSect(WorkDisk[_sect], 0)); // This is where parent/name of direcotyr should be
				posParDir=getFreeDataBlock(false);
				// make new direcotry from it and write it.
				writeDirSect(WorkDisk[posParDir.Sect], 0, bitStream);
				temp.alloc[i+1]=posParDir.Sect;

				writeInodeSectInode(WorkDisk[_posInode.Sect], _posInode.Count, temp);
				cout << "Writeshit\t\t" << readInodeSectInode(WorkDisk[_posInode.Sect], _posInode.Count).alloc[0] << " indoe\t" << readInodeSectInode(WorkDisk[_posInode.Sect], _posInode.Count).alloc[1] ;
				cout << _pos << '\t'  << posParDir.Sect << '\t' << posParDir.Count << '\t' << _posInode.Sect << '\t' << _posInode.Count <<  endl;
				posParDir.isPar=true;
				i=10;
				


			}
			else{
				_sect=temp.alloc[i+1];
			}
		}
	}
	return posParDir;
}
#endif

