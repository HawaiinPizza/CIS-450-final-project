// Whoel Porgram decompsiostion
//    This is an outline/code of how the whole program will be. 
//    #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
// #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>/*{{{*/
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#include "Main.h"
using namespace std;
/*}}}*/

#define SectorSize 512/*{{{*/
#define SectorNum 1000
#define inodeCount 35
#define inodeOffset 6

//THe Disk
std::bitset<4096> ExtDisk[1000];
std::bitset<4096> WorkDisk[1000];





struct inode{
     bool type;
     uint size;
     uint alloc[10];
};
struct dir{
     char Name[16];
     uint long inodePlace;
};

/*}}}*/

inode readBitDataInode(string BitStream){// Type // Size // 10*10 of which bits are allociated to it.{{{{{{

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
	  Ret.type=temp.to_ulong();
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
	  bitset<1> temp(Inode.type);
	  RetStr+=temp.to_string();
     }
	  bitset<114> Ret(RetStr);
	  return Ret;
}
/*}}}*/


//Expect 20 bytes, or 16 characters of 8 bits, and 4 bits for size{{{
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



//Expect 20 bytes, or 16 characters of 8 bits, and 4 bits for size
bitset<132> writeDir(dir Dir){
	cout << endl;
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
/*}}}*/
}/*}}}*//*}}}*/

/*{{{Testing*/

void inodeReadWriteTest(){/*{{{*/
	string test= "100000000010000000001000000000100000000010000000001000000000100000000010000000001000000000100000000110000000000001";
	inode temp=readBitDataInode(test);

     bitset<114> test2=writeBitDataInode(temp);
     cout << "Input:\t" << test << endl;
     cout << "Output:\t" << test2 << endl;

     cout << "Inode size and type:\t" << temp.size << '\t' << temp.type << endl;
     for(int i=0; i<10; i++){
	  cout << temp.alloc[i] << '\t';
     }
     cout << endl;

       temp=readBitDataInode(test);
       cout << "Inode size and type:\t" << temp.size << '\t' << temp.type << endl;
       for(int i=0; i<10; i++){
       cout << temp.alloc[i] << '\t';
       }
       cout << endl;

		
     assert(test2.to_string()==test);

}/*}}}*/

void dirReadWRiteTest(){/*{{{*/
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



     
}/*}}}*/
/*}}}*/


int main(){
     dirReadWRiteTest();
     
}

// }}}
