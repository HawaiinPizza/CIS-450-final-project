// Whoel Porgram decompsiostion
//    This is an outline/code of how the whole program will be. 
//    #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
// #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"
#include <iostream>
#include <cmath>
#include <queue>
#include <climits>
#include <bitset>
#include <cassert>
#include "Dir.h"
#include "Dir.h"
#include "FS.h"
#include "Disk.h"
#include "Dir.h"
#include "File.h"

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


	void print(){
		forloop2(6, SectorNum){
			forloop(0, dirCount){
				auto a=readDirSect(WorkDisk[j], i);
				if(a!=0){
					cout << j << '\t' << i << '\t';
					dir Dir=readDirSectDir(WorkDisk[j], i); 
					cout << Dir.Name;
					cout << endl;
				}
			}
		}

	}

	void inodePrint(){
		forloop(3, 6){
			forloop2(0, inodeCount){
				auto a=readInodeSectBit(WorkDisk[i], j);
				if(a!=0){
					cout << i << '\t' << j << '\t';
					inode node=getBitInode(a);
					cout << node.alloc[0] << '\t' << node.alloc[1];
					cout << endl;
				}

			}
		}
	}


int main(){
	/* FS_Boot(); */

	//Creating directories manually
	inode RootInode	(0,0,	6);
	/* inode AInode	(0,1, 	1); */
	/* inode BInode	(0,0,	2); */
	/* inode A1Inode	(0,0,	3); */

	dir RootDir("/", 0);
	/* dir RootDir("/", 0); */
	/* dir ADir("A", 1); */
	/* dir BDir("B", 2); */
	/* dir A1Dir("1", 3); */

	writeInodeSectInode(WorkDisk[3], 0, RootInode);

	writeDirSectDir(WorkDisk[RootInode.alloc[0]], 0, RootDir);

	/* cout  << "Try to dlete root/\t" <<  DirUnlink("/") << endl; */

	DirCreate("/A") ;
	DirCreate("/B") ;
	DirCreate("/C") ;
	DirCreate("/D") ;
	DirCreate("/E") ;
	DirCreate("/F") ;
	DirCreate("/G") ;
	DirCreate("/H") ;
	DirCreate("/I") ;
	DirCreate("/K") ;


	File_Create("/A/1");
	File_Create("/A/10");
	File_Create("/A/2");
	File_Create("/A/3");
	File_Create("/A/4");
	File_Create("/A/5");
	File_Create("/A/6");
	File_Create("/A/7");
	File_Create("/A/8");
	File_Create("/A/9");
	File_Create("/B/1");
	File_Create("/B/2");

	cout << File_Open("/A/1") << endl;
	cout << File_Open("/A/2") << endl;
	cout << File_Open("/A/3") << endl;
	cout << File_Open("/A/4") << endl;


	forloop(0,10){
		if(openFileTable.isValid[i])
			cout << openFileTable.OpenFile[i].name << endl;
	}

	cout << File_Close(0) << endl;

	forloop(0,10){
		if(openFileTable.isValid[i])
			cout << openFileTable.OpenFile[i].name << endl;
	}
	cout << "SIZE\t" << openFileTable.size << endl;

	string _temp="1111";
	cout << File_Write(2, _temp, _temp.size()) << endl;
	string _pothole;
	_pothole.resize(4096);
	cout << File_Read(2, _pothole, _pothole.size()) << endl;

	cout << _pothole << endl;


	return 0;

}
