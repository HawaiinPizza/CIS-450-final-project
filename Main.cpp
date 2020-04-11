// Whoel Porgram decompsiostion
//    This is an outline/code of how the whole program will be. 
//    #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
// #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#include "Main.h"
using namespace std;


#define SectorSize 512
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

inode readBitDataInode(string BitStream){// Type // Size // 10*10 of which bits are allociated to it.

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

     bitset<114> Ret;
     string RetStr; //So we can concate instead of dealing with how bits are setup in bitset
     { // alloc decleartion

	  for(int i=0; i<10; i++){ // Loop through each alloc
	       bitset<10> tempAlloc(Inode.alloc[i]);
	       for(int j=0; j<10; j++){ // Loop through each value in alloc
		    int val=9-j;
		    cout << (i*10+j) << ' ' << tempAlloc[val] << '\t';
		    Ret.set(i*10+j,tempAlloc[val]);
		    
	       }
	       cout << endl;

	  }
     }
     /*
     { // SIze decleartion
	  bitset<13> temp(Inode.size);
	  for(int i=100; i<100+13; i++){
	       Ret[i]=temp[i-100];
	       
	  }
     }

     {// Type decleartion
	  bitset<1> temp(Inode.type);
	  Ret[113]=temp[0];
     }
     */
     bitset<114> Ret;
     return Ret;
}




//Expect 20 bytes, or 16 characters of 8 bits, and 4 bits for size
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

int main(){
		string test= "100000000010000000001000000000100000000010000000001000000000100000000010000000001000000000100000000010000000000000";
		inode temp=readBitDataInode(test);

		bitset<114> test2=writeBitDataInode(temp);
		cout << "Input:\t" << test << endl;
		cout << "Output:\t" << test2 << endl;

		cout << "Inode size and type:\t" << temp.size << '\t' << temp.type << endl;
		for(int i=0; i<10; i++){
		     cout << temp.alloc[i] << '\t';
		}
		cout << endl;

		inode temp=readBitDataInode(test);
		cout << "Inode size and type:\t" << temp.size << '\t' << temp.type << endl;
		for(int i=0; i<10; i++){
		     cout << temp.alloc[i] << '\t';
		}
		cout << endl;

		
		//assert(test2.to_string()==test);

}

