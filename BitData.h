#ifndef Bit_H
#define Bit_H
/* Data and macros *//*{{{*/
#define SectorSize 512
#define SectorBit 4096
#define SectorNum 1000
#define inodeCount 35
#define inodeOffset 6
#define inodeSize 114
#define dirSize 1160


// Macro used to ease declearing feckign disk
#define Sector std::bitset<SectorSize*8> 
#define disk(x) std::bitset<SectorSize*8> x[SectorNum]
//

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

#define backloop(x,y) \
	for(int i=y; i>x; i--) \


bool range(int val, int x, int y){
	if(val>=x && val<=y)
		return true;
	return false;
}

struct inode{
	bool isFile;
	uint size;
	uint alloc[10];
};
struct dir{
	char Name[16];
	uint long inodePlace;
};/*}}}*/
string inodeSubStr(Sector sect, int Count){ ///{{{
	if(Count<0 || Count >35){
		for(int i=0; i<9999; i++)
			cout << "ZAKI THIS IS WORNG> NOT IN BOUNDS! FIX NOW!!!!!!!!!!!!!!!!!!!!!";
		return "";

	}
	else{
		string RetStrTemp=sect.to_string();
		int x=(34-Count)*inodeSize+inodeOffset;
		int y=inodeSize;
		cout << x << '\t' << x+y << endl;

		string RetStr=RetStrTemp.substr(x,y);
		return RetStr;

	}
}

//}}}
void inodeSetStr(Sector &sect, int Count, string buffer){ ///{{{
	if(Count<0 || Count >35 || buffer.length()!=inodeSize){
		cout << "HOW?";
		for(int i=0; i<9999; i++)
			cout << "ZAKI THIS IS WORNG> NOT IN BOUNDS! FIX NOW!!!!!!!!!!!!!!!!!!!!!\n";
		return;

	}
	else{
		bitset<inodeSize> bufset(buffer);
		int x=(34-Count)*inodeSize+inodeOffset;
		int y=inodeSize;
		cout << x << '\t' << x+y << endl;
		for(int i=x; i<x+y; i++){
			sect[i]=bufset[i-x];
		}

	}
}
// indoe setter
void inodeSet(inode *Node, int pos, int val){
	Node->alloc[pos]=val+inodeOffset;
}

//}}}
inode readBitDataInode(string BitStream){// Type // Size // 10*10 of which bits are allociated to it. {{{

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


bitset<114> writeBitDataInode(inode Inode){//Retrun 114 bits

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

/*}}}*/
//Expect 20 bytes, or 16 characters of 8 bits, and 4 bits for size {{{
dir readDir(string BitStream){
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
//


//Expect 20 bytes, or 16 characters of 8 bits, and 4 bits for size 
bitset<132> writeDir(dir Dir){
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
	bitset<132> Ret(RetStr);
	return Ret;
}


///}}}
void inodeReadWriteTest(){ //{{{
	string test= "100000000010000000001000000000100000000010000000001000000000100000000010000000001000000000100000000110000000000001";
	inode temp=readBitDataInode(test);

	bitset<114> test2=writeBitDataInode(temp);
	cout << "Input:\t" << test << endl;
	cout << "Output:\t" << test2 << endl;

	cout << "Inode size and isFile:\t" << temp.size << '\t' << temp.isFile << endl;
	for(int i=0; i<10; i++){
		cout << temp.alloc[i] << '\t';
	}
	cout << endl;

	temp=readBitDataInode(test);
	cout << "Inode size and isFile:\t" << temp.size << '\t' << temp.isFile << endl;
	for(int i=0; i<10; i++){
		cout << temp.alloc[i] << '\t';
	}
	cout << endl;


	assert(test2.to_string()==test);

}

void dirReadWRiteTest(){
	string testStr="101101000001010000100100001101000100010001010100011001000111010010000100000101000010010000110100010001000101010001100100011101001000";
	dir Test1=readDir(testStr);
	cout << "Input\t";
	cout << Test1.inodePlace << '\t' << endl;
	for(int i=0; i<16; i++){
		cout << Test1.Name[i] << ' ';
	}
	cout << endl;

	cout << "Output\t";
	bitset<132> Test2=writeDir(Test1);

	Test1=readDir(Test2.to_string());
	cout << Test1.inodePlace << '\t' << endl;
	for(int i=0; i<16; i++){
		cout << Test1.Name[i] << ' ';
	}
	cout << endl;

	assert(testStr==Test2.to_string());

} //}}}
#endif
