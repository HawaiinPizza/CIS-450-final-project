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
#include "FS.h"
using namespace std;
/*}}}*/




int main(){

	cout << FS_Reset() << endl;
	cout << FS_Check() << endl;
	cout << FS_Boot() << endl;
	cout << FS_Reset() << endl;
	cout << FS_Check() << endl;

}
