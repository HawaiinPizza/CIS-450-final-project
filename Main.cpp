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
#include "BitData.h"
#include "FS.h"
#include "Disk.h"
#include "Dir.h"
using namespace std;


#define siz 50
#define set 13

// Read and write are here for histroical purposes. They are take a size (how many inodes/directories can you fit in a sector) and set (aka how long is a inode/directory) and count (which spot in sector do you want to change" and maniplates them.
//
// THE KEY ASSUMPATION IS NOISE: there's also bits left over. their posistion is from 0->siz%set
	void read(bitset<siz> test, int count){
		if(range(count, 0, siz/set)){
			int start=count*set;
			int stop=start+set;
			bitset<set> retBit;
			forloop(start,stop) {
				retBit[i-start]=test[i];
			}
			cout << start <<":" << (stop-1) << '\t' << retBit << endl;
		}
		else{
		}
	}


	void write(bitset<siz> &test, int count, string buffer){
		if(range(count, 0, siz/set)){
			int start=count*set;
			int stop=start+set;
			bitset<set> writeBit(buffer);
			forloop(start,stop) {
				test[i]=writeBit[i-start];
			}
		}
		else{


		}
	}

int mainTest1(){
	char on='1';
	char off;
	ifelse(on=='1', off='0', off='1');
	string testStr;
	forloop(0, siz%set){ // Useless/nosie
		testStr+="1";
	}
	forloop(0,siz){
		ifelse((i/set)%2==0, testStr+=on, testStr+=off);
	}
	cout << "%:" << siz%set << '\t' <<  '\t';//  << testStr << ;;
	cout << endl;
	bitset<siz> test1(testStr);
	forloop(0,siz/set){
		read(test1, i);
	}

	cout << endl;
	cout << on << '\t' << off << endl;
	forloop(0,siz/set){

		if(i%2==0){
			string z="";
			forloop(0, set){
				z+=on;
			}
			write(test1, i,z);
		}
		else{
			string z="";
			forloop(0, set){
				z+=off;
			}
			write(test1, i, z);
		}
	}

	forloop(0,siz/set){
		read(test1, i);
	}
	return 0;
}

int mainTest2(){ // NOise testing
	char on='1';
	char off;
	ifelse(on=='1', off='0', off='1');
	string testStr;
	forloop(0, siz%set){ // Useless/nosie
		testStr+=on;
	}
	forloop(0,siz){
		ifelse((i/set)%2==0, testStr+=on, testStr+=off);
	}
	cout << "%:" << siz%set << '\t' <<  '\t';//  << testStr << ;;
	cout << endl;
	bitset<siz> test1(testStr);
	forloop(0, siz){
		if(range(i, 0, siz%set ))
			cout << "Noise\t";
		else
			cout << "Value\t";
		cout << i << '\t' << test1[i] << endl;
	}
	return 0;
}


// This funciton is not expanedable yet.
bool dirCreate(inode &node, string Name, int Sect, disk(Disk)){ // Creates an empty directory. ALSO, Disk should

	int posDir;
	int posInode;
	dir Dir;
	{ // Settign the posDir and posInode Values
		forloop(0, 35){ // Finding first empty inode TODO make sure to expand this, so we check 3 sectors, not just the first 1.
			if(readInodeSect(Disk[0], i)!=0){
				cout << i << '\t' << readInodeSect(Disk[0], i) << endl;
			}
		}
	}

	/* { // Setting Dir Values */
	/* 	forloop(0,14){ */
	/* 		Dir.Name[i]=Name[i]; */
	/* 	} */
	/* 	Dir.Name[15]='\0'; */
	/* 	Dir.inodePlace=posDir; */
	/* } */

	/* // Create datablock dictionary */
	/* bitset<dirSize> dirBit=writeDir(Dir); */
	/* writeDirSect(sect, posDir, dirBit); */
	
	/* node.alloc[0]=posDir; */ 
	

	/* //This is creating the root folder. Later on, we will make it with DirCreate("/") */
	/* string inodeString=writeBitDataInode(node).to_string(); */
	/* bitset<SectorSize*8> inodeBit(inodeString); */

	/* writeInodeSect(sect, posInode, inodeString); */
	return true;
}

int main(){
	bitset<SectorBit> TestDrive[SectorNum];
	/* disk(TestDrive2); */
	/* uint *Arr=new uint [10]; */
	/* inode Root(false, 1, Arr); */
	/* dirCreate(Root, "/", 0, TestDrive); */
	cout << "Arr does not have = new int[10]\t" << readInodeSect(TestDrive[0], 1) << endl;
	/* cout << readInodeSect(TestDrive2[0], 1) << endl; */
	int *Arr=new int[10];
	cout << "Arr does  have = new int[10]\t" << readInodeSect(TestDrive[0], 1) << endl;






	/* getDirPath testing
		getDirPath("/");
		getDirPath("/A");
		getDirPath("/A/");
		getDirPath("/A/B/");
		getDirPath("/A/B/C");
		getDirPath("/A/B/D");
		getDirPath("/A/B/E");
		getDirPath("/A/B/F");
		getDirPath("/A/B/G");
		getDirPath("/A/B/H");

		getDirPath("/A/B/C/1");
		getDirPath("/A/B/C/2");
		getDirPath("/A/B/C/3");
		getDirPath("/A/B/C/4");
		getDirPath("/A/B/C/5");

		getDirPath("/A/B/D/1");
		getDirPath("/A/B/D/2");
		getDirPath("/A/B/D/3");
		getDirPath("/A/B/D/4");
		getDirPath("/A/B/D/5");

		getDirPath("/A/B/E/1");
		getDirPath("/A/B/E/2");
		getDirPath("/A/B/E/3");
		getDirPath("/A/B/E/4");
		getDirPath("/A/B/E/5");

		getDirPath("/A/B/F/1");
		getDirPath("/A/B/F/2");
		getDirPath("/A/B/F/3");
		getDirPath("/A/B/F/4");
		getDirPath("/A/B/F/5");
		*/


}
