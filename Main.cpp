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


	DirCreate("/A/a") ;
	DirCreate("/A/b") ;
	DirCreate("/A/c") ;
	DirCreate("/A/d") ;

	DirCreate("/B/q") ;
	DirCreate("/B/w") ;
	DirCreate("/B/e") ;
	DirCreate("/B/r") ;


	DirCreate("/B/q/a") ;
	DirCreate("/B/q/b") ;
	DirCreate("/B/q/c") ;
	DirCreate("/B/q/d") ;
	DirCreate("/B/q/e") ;
	DirCreate("/B/q/f") ;
	DirCreate("/B/q/g") ;
	DirCreate("/B/q/h") ;
	DirCreate("/B/q/i") ;
	DirCreate("/B/q/j") ;
	DirCreate("/B/q/k") ;
	DirCreate("/B/q/l") ;
	DirCreate("/B/q/m") ;
	DirCreate("/B/q/n") ;
	DirCreate("/B/q/o") ;
	DirCreate("/B/q/l") ;
	DirCreate("/B/q/p") ;
	DirCreate("/B/q/q") ;
	DirCreate("/B/q/r") ;
	DirCreate("/B/q/s") ;
	DirCreate("/B/q/t") ;
	DirCreate("/B/q/u") ;
	DirCreate("/B/q/v") ;
	DirCreate("/B/q/w") ;
	DirCreate("/B/q/x") ;
	DirCreate("/B/q/y") ;
	DirCreate("/B/q/a1") ;
	DirCreate("/B/q/b1") ;
	DirCreate("/B/q/c1") ;
	DirCreate("/B/q/d1") ;
	DirCreate("/B/q/e1") ;
	DirCreate("/B/q/f1") ;
	DirCreate("/B/q/g1") ;



	_DirRead("/");
	_DirRead("/A");
	_DirRead("/B");
	_DirRead("/B/q");

	inode temp=getInode("/B/q");
	cout << endl;
	cout << temp.alloc[1] << endl;
	print();

	/* cout  << "3rd level /B/q/1\t" << DirUnlink("/B/q/1") << endl; */
	/* cout  << "3rd level /B/q/2\t" << DirUnlink("/B/q/2") << endl; */
	/* cout  << "3rd level /B/q/3\t" << DirUnlink("/B/q/3") << endl; */
	/* cout  << "3rd level /B/q/4\t" << DirUnlink("/B/q/4") << endl; */

	/* cout  << "3rd level /B/q/5\t" << DirUnlink("/B/q/5") << endl; */
	/* cout  << "3rd level /B/q/6\t" << DirUnlink("/B/q/6") << endl; */
	/* cout  << "3rd level /B/q/7\t" << DirUnlink("/B/q/7") << endl; */
	/* cout  << "3rd level /B/q/8\t" << DirUnlink("/B/q/8") << endl; */

	/* cout  << "2nd level /B/q\t" << DirUnlink("/B/q") << endl; */
	/* cout  << "2nd level /B/w\t" << DirUnlink("/B/w") << endl; */
	/* cout  << "2nd level /B/e\t" << DirUnlink("/B/e") << endl; */
	/* cout  << "2nd level /B/r\t" << DirUnlink("/B/r") << endl; */

	/* cout  << "1st level /B\t" <<  DirUnlink("/B") << endl; */

	/* cout  << "Try to dlete root/\t" <<  DirUnlink("/") << endl; */
	/* print(); */

	return 0;

}
