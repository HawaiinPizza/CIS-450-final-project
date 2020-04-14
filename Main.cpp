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
int main(){
	/* FS_Boot(); */

	//Creating directories manually
	inode RootInode	(0,0,	6);
	/* inode AInode	(0,1, 	1); */
	/* inode BInode	(0,0,	2); */
	/* inode A1Inode	(0,0,	3); */

	dir RootDir("/", 0);
	/* dir ADir("A", 1); */
	/* dir BDir("B", 2); */
	/* dir A1Dir("1", 3); */

	writeInodeSectInode(WorkDisk[3], 0, RootInode);

	cout << "EMPTY\n";
	print();
	writeDirSectDir(WorkDisk[RootInode.alloc[0]], 0, RootDir);
	cout << readDirSectDir(WorkDisk[0],0).Name;
	cout << "ROOT\n";
	print();
	/* cout << DirCreate("/A") << endl; */
	/* cout << DirCreate("/B") << endl; */
	/* cout << DirCreate("/C") << endl; */
	/* cout << DirCreate("/D") << endl; */
	cout << "AFTER DECLEARITON\n";
	print();


/* 	cout << DirCreate("/A/a") << endl; */
/* 	cout << DirCreate("/A/b") << endl; */
/* 	cout << DirCreate("/A/c") << endl; */
/* 	cout << DirCreate("/A/d") << endl; */

/* 	cout  << DirCreate("/B/q") << endl; */
/* 	cout  << DirCreate("/B/w") << endl; */
/* 	cout  << DirCreate("/B/e") << endl; */
/* 	cout  << DirCreate("/B/r") << endl; */


/* 	cout  << DirCreate("/B/q/1") << endl; */
/* 	cout  << DirCreate("/B/q/2") << endl; */
/* 	cout  << DirCreate("/B/q/3") << endl; */
/* 	cout  << DirCreate("/B/q/4") << endl; */
/* 	cout  << DirCreate("/B/q/5") << endl; */
/* 	cout  << DirCreate("/B/q/6") << endl; */
/* 	cout  << DirCreate("/B/q/7") << endl; */
/* 	cout  << DirCreate("/B/q/8") << endl; */


/* 	cout  << "3rd level /B/q/1\t" << DirUnlink("/B/q/1") << endl; */
/* 	cout  << "3rd level /B/q/2\t" << DirUnlink("/B/q/2") << endl; */
/* 	cout  << "3rd level /B/q/3\t" << DirUnlink("/B/q/3") << endl; */
/* 	cout  << "3rd level /B/q/4\t" << DirUnlink("/B/q/4") << endl; */
/* 	cout  << "3rd level /B/q/4\t" << DirUnlink("/B/q/4") << endl; */
/* 	cout  << "3rd level /B/q/4\t" << DirUnlink("/B/q/4") << endl; */
/* 	cout  << "3rd level /B/q/4\t" << DirUnlink("/B/q/4") << endl; */

/* 	cout  << "3rd level /B/q/5\t" << DirUnlink("/B/q/5") << endl; */
/* 	cout  << "3rd level /B/q/6\t" << DirUnlink("/B/q/6") << endl; */
/* 	cout  << "3rd level /B/q/7\t" << DirUnlink("/B/q/7") << endl; */
/* 	cout  << "3rd level /B/q/8\t" << DirUnlink("/B/q/8") << endl; */

/* 	cout  << "2nd level /B/q\t" << DirUnlink("/B/q") << endl; */
/* 	cout  << "2nd level /B/w\t" << DirUnlink("/B/w") << endl; */
/* 	cout  << "2nd level /B/e\t" << DirUnlink("/B/e") << endl; */
/* 	cout  << "2nd level /B/r\t" << DirUnlink("/B/r") << endl; */

/* 	cout  << "1st level /B/r\t" <<  DirUnlink("/B/r") << endl; */

	return 0;

}
