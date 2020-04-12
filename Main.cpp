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



void read(bitset<20> test, int count){
	if(range(count, 0, 5)){
		int start=count*3;
		int stop=start+3;
		bitset<3> retBit;
		forloop(start,stop) {
			retBit[i-start]=test[i];
		}
		cout << start <<":" << (stop-1) << '\t' << retBit << endl;
	}
	else{
	}
}


void write(bitset<20> &test, int count, string buffer){
	if(range(count, 0, 5)){
		int start=count*3;
		int stop=start+3;
		bitset<3> writeBit(buffer);
		forloop(start,stop) {
			test[i]=writeBit[i-start];
		}
	}
	else{
	}
}

int main(){
//	FS_Boot();
	int size=20;
	int set=3;
	bitset<20> test1("11000111000111000111");
	forloop(0,7){
		read(test1, i);
	}

	cout << endl;
	forloop(0,7){
		if(i%2==0)
			write(test1, i, "000");
		else
			write(test1, i, "111");
	}

	forloop(0,7){
		read(test1, i);
	}



}
