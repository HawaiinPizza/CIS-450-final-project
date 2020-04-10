// Whoel Porgram decompsiostion
//    This is an outline/code of how the whole program will be. 
//    #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
// #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>


int SectorSize = 512;
int SectorNum = 1000;
int inodeCount = 21;





#include <iostream>
#include <cmath>
using namespace std;

int main(){
   inode temp;
   temp.writeInode();
    return 0;
}

// inode 
//     - writeBitStream() :: Write teh type, size and allociation, by reversing the blwo opeariton

//     - readBitStream() :: read the type, size and allcioation by folowing the following processess

//     - There are 4 indoes within a inode sector. The makeup totals to 114 bits.
//       - 1 bit  :: for which type of inode this is.
//       - 13 bits :: (or 1.625 bytes) for representing the size of datablocks
//       - 100 bits ::  10 seqeunces of 10 bits for reprsenting the location. note that all 1s mean that this is not allociated
	
//     - This results of 106 of useless data, and 3990 of useful data. Since there are 35 inodes in a sector, we split it up into an array, with each piece being a substr of 114 bits.

//     - The function below is a method ofreading it. Note it doesn't return anything. Maybe i'll try to do that thing where i have an inlnie function and do it there.

//     - Anotehr note: there'll be 35 inodes withn a sector, so the spliting of that by 114 is left to futrue zak.

//     - Writing it to bitstream is simple. if need be write a funciton for it.

//       #+NAME: bitstreamtoInode
//       #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
//      #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>






int main() {
#include<iostream>
#include<bitset>
using namespace std;
// Note in babel mode this will be incorrect

void readBitDataInode(){// Type // Size // 10*10 of which bits are allociated to it.

    // This little test is used to demonstrate values used to finding where to substring
    /*
    string test= "11111NNNNN22222NNNNN33333NNNNN44444NNNNN55555NNNNN66666NNNNN77777NNNNN88888NNNNN99999NNNNN00000NNNNNSSSSSSSSSSSSST";
    cout << test.substr(0,100) << endl; //Which are allcoiated
    cout << test.substr(100,13) << endl; //Size
    cout << test.substr(113,1) << endl; //Type
    for(int i=0; i<10; i++){ 
	cout << test.substr(i*10,10) << endl;  // used to show how to splti the function
    }
    */


    string test= "001001010000000000000000000000000000000000000000000000000000000000000000000000000000000010000000001100000001111111";
	uint alloc[10];
	for(int i=0; i<10; i++){
		bitset<10> temp(test.substr(i*10, 10));
		cout << temp << '\t' << temp.to_ulong() << endl;
		alloc[i]=temp.to_ulong();
	}
	uint size;
	bitset<13> temp2(test.substr(100,13));
	size=temp2.to_ulong();

	bool type;
	bitset<1> temp3(test.substr(113,1));
	type=temp2.to_ulong();

	cout << size << '\t' << type << endl;
}
return 0;
}

// datablock
//     - Datablocks are disgshiustehd by two types: file and directory
//     - the type of the datablock is denoted by teh inode, not the directory.
//     - For directory, tehre is a 20 bytes/160 bits, which are
//       + 16 bytes/128 bits :: file name. 15 characters PLUS 1 for end of string, so it's mroe of 15 characters
//       + 4 byte/32 bits :: inode that shows which file/driectory this is.
//     - This means that dictionaries cna have 25 files in a a sector, but 250 files/directories overall.
//     - This doesn't have the case, of half a directoriy's infroamtion being in one datablcok, and the other half being in another datablock. THat isn't consdiered.
//      #+NAME: bitstreamtoInode
//      #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org 
//      #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>






using namespace std;
void readDir(string TestString){
    bitset<64> inode(TestString.substr(0,4));
    cout << inode.to_ulong() << endl;
    char temp[10];
    for(int i=0; i<8; i++){
	    bitset<16> temp(TestString.substr(4+i*16,16));
	    cout << (char)temp.to_ulong() << endl;

    }

}

int main(){
/*
	string temp1="iiii11111111111111112222222222222222333333333333333344444444444444445555555555555555666666666666666677777777777777778888888888888888";
	for(int i=0; i<8; i++){
	    cout << temp1.substr(4+i*16,16) << endl;
	}
*/
	/*readDir("111110000000000000001000000000000000100000000000000010000000000000001000000000000000100000000000000010000000000000001000000000000000");*/
	readDir("111100000000010000010000000001000010000000000100001100000000010001000000000001000101000000000100011000000000010001110000000001001000");
}

// Sector/Root Inode      
//     - A sector is a collection of a superblock, bitmaps for in use indoes and datablocks, a sqeunce of indoes, and a sequence of datablocks.  However, this information HAS TO BE CONVERETD to that. Otehrwise, a sector is just an array of bitsets of 4096 bits.
//     - However, the sector converts it's concats to usuable datasturcutres. After each file/directory operation, it saves the stuff to workign directory. Than, working directory saves it stuff to external disk when FS_SYNC() is made.
//     - The disks are just a bitset array of 4096 bits, with 1000 elements in each.
//     - The root inode is the indoe that represtns nothing. This is a special variable, as to not have to find out what it is on disk tediously.
      
//    #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
// #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>






int main() {
std::bitset<4096> ExtDisk[1000];
std::bitset<4096> WorkDisk[1000];
return 0;
}
