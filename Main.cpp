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


int main(){
	FS_Boot();

	//Creating directories manually
	inode RootInode	(0,10,	0);
	inode AInode	(0,20, 	1);
	inode BInode	(0,40,	2);
	inode A1Inode	(0,80,	3);

	dir RootDir("/", 0);
	dir ADir("A", 1);
	dir BDir("B", 2);
	dir A1Dir("1", 3);

	writeInodeSectInode(WorkDisk[3], 0, RootInode);
	writeInodeSectInode(WorkDisk[3], 1, AInode);
	writeInodeSectInode(WorkDisk[3], 2, BInode);
	writeInodeSectInode(WorkDisk[3], 3, A1Inode);

	writeDirSectDir(WorkDisk[6+RootInode.alloc[0]], 0, RootDir);
	writeDirSectDir(WorkDisk[6+BInode.alloc[0]], 0, BDir);
	writeDirSectDir(WorkDisk[6+A1Inode.alloc[0]], 0, A1Dir);
	writeDirSectDir(WorkDisk[6+AInode.alloc[0]], 0, ADir);

	writeDirSectDir(WorkDisk[6], 1, ADir);
	writeDirSectDir(WorkDisk[6], 2, BDir);


	writeDirSectDir(WorkDisk[7], 1, A1Dir);


	inode test1=getInode("/");
	inode test2=getInode("/A");
	inode test3=getInode("/A/1");
	inode test4=getInode("/A/D");
	cout << test1.size << '\t' << test1.alloc[0] << endl;
	cout << test2.size << '\t' << test2.alloc[0] << endl;
	cout << test3.size << '\t' << test3.alloc[0] << endl;
	cout << test4.size << '\t' << test4.alloc[0] << endl;
	cout << endl;


	cout << "DIR -1\t" << DirCreate("/A/B/C/D")  << endl;
	cout << "DIR -2 \t" << DirCreate("/A/B")  << endl;
	cout << "DIR -2  \t" << DirCreate("/A/")  << endl;
	cout << "SIZE\t" << DirSize("/A") << endl;
	cout << "DIR 0\t" << DirCreate("/A/7") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/7") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/7/J") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/7/IO") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/7/ZXJCI") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/9") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/2") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/q") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/h") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/h") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/hojaid") << endl ;
	cout << "DIR 0\t" << DirCreate("/A/h9j1id") << endl ;
	cout << "SIZE\t" << DirSize("/A") << endl;


	forloop(0, 32){
		bitset<132> stream=readDirSect(WorkDisk[7], i);
		if(stream!=0){
			dir Dir=getBitDir(stream);
			cout << Dir.Name << '\t' << Dir.inodePlace << endl;
		}
	}






}
