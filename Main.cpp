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
using namespace std;


#define siz 10000
#define set 219

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

int main(){
	char on='1';
	char off;
	ifelse(on=='1', off='0', off='1');
	string testStr;
	forloop(0, siz%set){
		testStr+="0";
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

}
//	FS_Boot();
