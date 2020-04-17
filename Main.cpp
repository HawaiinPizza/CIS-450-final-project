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

int main(){
	FS_Boot();

	cout << "Creating directories\n";

	cout << "Creating Direcotry /I\t" << DirCreate("/I") << endl;
	cout << "Creating Direcotry /A\t" << DirCreate("/A") << endl;
	cout << "Creating Direcotry /B\t" << DirCreate("/B") << endl;
	cout << "Creating Direcotry /D\t" << DirCreate("/D") << endl;
	cout << "Creating Direcotry /C\t" << DirCreate("/C") << endl;
	cout << "Creating Direcotry /F\t" << DirCreate("/F") << endl;
	cout << "Creating Direcotry /F\t" << DirCreate("/F") << endl;
	cout << "Creating Direcotry /E\t" << DirCreate("/E") << endl;
	cout << "Creating Direcotry /G\t" << DirCreate("/G") << endl;
	cout << "Creating Direcotry /H\t" << DirCreate("/H") << endl;
	cout << "Creating Direcotry /K\t" << DirCreate("/K") << endl;
	string longname="/1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111 ";

	cout << "Creating Direcotry path with over 26 characters" << DirCreate(longname) << endl;

	cout << "Creating Direcotry /A/a\t" << DirCreate("/A/a") << endl;
	cout << "Creating Direcotry /B/b\t" << DirCreate("/B/b") << endl;
	cout << "Creating Direcotry /C/c\t" << DirCreate("/C/c") << endl;
	cout << "Creating Direcotry /C/c\t" << DirCreate("/C/c") << endl;
	cout << "Creating Direcotry /D/d\t" << DirCreate("/D/d") << endl;
	cout << "Creating Direcotry /E/e\t" << DirCreate("/E/e") << endl;
	cout << "Creating Direcotry /F/f\t" << DirCreate("/F/f") << endl;
	cout << "Creating Direcotry /G/g\t" << DirCreate("/G/g") << endl;
	cout << "Creating Direcotry /H/h\t" << DirCreate("/H/h") << endl;
	cout << "Creating Direcotry /I/i\t" << DirCreate("/I/i") << endl;
	cout << "Creating Direcotry /K/k\t" << DirCreate("/K/k") << endl;
	cout << "Creating Direcotry /P/.\t" << DirCreate("/P/.") << endl;

	cout << "Creating Directory /A/0\t" << File_Create("/A/0") << endl;
	cout << "Creating Directory /A/1\t" << File_Create("/A/1") << endl;
	cout << "Creating Directory /A/2\t" << File_Create("/A/2") << endl;
	cout << "Creating Directory /A/2\t" << File_Create("/A/2") << endl;
	cout << "Creating Directory /A/3\t" << File_Create("/A/3") << endl;
	cout << "Creating Directory /A/4\t" << File_Create("/A/4") << endl;
	cout << "Creating Directory /A/5\t" << File_Create("/A/5") << endl;
	cout << "Creating Directory /A/6\t" << File_Create("/A/6") << endl;
	cout << "Creating Directory /A/7\t" << File_Create("/A/7") << endl;
	cout << "Creating Directory /A/8\t" << File_Create("/A/8") << endl;
	cout << "Creating Directory /A/9\t" << File_Create("/A/9") << endl;
	cout << "Creating Directory /B/1\t" << File_Create("/B/1") << endl;
	cout << "Creating Directory /B/2\t" << File_Create("/B/2") << endl;

	cout << "\nReading directories\n";


	string read;
	read.resize(DirSize("/A"));
	DirRead("/A", read);
	cout << "/A Directory \t" << read << endl;

	read.resize(DirSize("/"));
	DirRead("/", read);
	cout << "/ Directory WITHOUT CLEARING THE BUFFER!! \t" << read << endl;

	read="";
	read.resize(DirSize("/"));
	DirRead("/", read);
	cout << "/ Directory WITH CLEARING THE BUFFER!! \t" << read << endl;


	cout << "File Open  A/1 " << File_Open("/A/1") << endl;
	cout << "File Open  A/2 " << File_Open("/A/2") << endl;
	cout << "File Open  A/3 " << File_Open("/A/3") << endl;
	cout << "File Open  A/4 " << File_Open("/A/4") << endl;
	cout << endl;


	string _temp;
	_temp="111001";
	cout << "Writing to fd 0 of \t" << _temp << '\t' << File_Write(0, _temp, _temp.size()) << endl;
	File_Close(0);
	File_Open("/A/1");

	string _pothole;
	_pothole.resize(_temp.size());
	cout << File_Read(0, _pothole, _pothole.size()) << endl;
	cout << "Readingfrom  fd 0 with value of  \t" << _pothole << '\t' << endl;  
	if(_pothole==_temp)
		cout << "😀";
	else
		cout << "💀";
	File_Close(0);

	forloop(0, 4096*11){
		_temp+="1";
	}
	cout << "Writing to fd 0 that has too much bits" << '\t' << File_Write(0, _temp, _temp.size()) << endl;
	File_Close(0);
	File_Open("/A/1");

	_pothole;
	_pothole.resize(_temp.size());
	cout << File_Read(0, _pothole, _pothole.size()) << endl;
	cout << "Failed at reading because it exceeds sector size " << _pothole << '\t' << endl;  
	if(_pothole==_temp)
		cout << "😀";
	else
		cout << "💀";
	File_Close(0);

	cout << "Removing File /A/0\t" << File_Unlink("/A/0") << endl;
	cout << "Removing File /A/1\t" << File_Unlink("/A/1") << endl;
	cout << "Removing File /A/2\t" << File_Unlink("/A/2") << endl;
	cout << "Removing File /A/3\t" << File_Unlink("/A/3") << endl;
	cout << "Removing File /A/4\t" << File_Unlink("/A/4") << endl;
	cout << "Removing File /A/5\t" << File_Unlink("/A/5") << endl;
	cout << "Removing File /A/6\t" << File_Unlink("/A/6") << endl;
	cout << "Removing File /A/7\t" << File_Unlink("/A/7") << endl;
	cout << "Removing File /A/8\t" << File_Unlink("/A/8") << endl;
	cout << "Removing File /A/9\t" << File_Unlink("/A/9") << endl;
	cout << "Removing File /B/1\t" << File_Unlink("/B/1") << endl;
	cout << "Removing File /B/2\t" << File_Unlink("/B/2") << endl;


	cout << "Unlinking Directory /A/a\t" << DirUnlink("/A/a") << endl;
	cout << "Unlinking Directory /B/b\t" << DirUnlink("/B/b") << endl;
	cout << "Unlinking Directory /C/c\t" << DirUnlink("/C/c") << endl;
	cout << "Unlinking Directory /D/d\t" << DirUnlink("/D/d") << endl;
	cout << "Unlinking Directory /E/e\t" << DirUnlink("/E/e") << endl;
	cout << "Unlinking Directory /F/f\t" << DirUnlink("/F/f") << endl;
	cout << "Unlinking Directory /G/g\t" << DirUnlink("/G/g") << endl;
	cout << "Unlinking Directory /H/h\t" << DirUnlink("/H/h") << endl;
	cout << "Unlinking Directory /I/i\t" << DirUnlink("/I/i") << endl;
	cout << "Unlinking Directory /K/k\t" << DirUnlink("/K/k") << endl;


	cout << "Unlinking Directory /I\t" << DirUnlink("/I") << endl;
	cout << "Unlinking Directory /A\t" << DirUnlink("/A") << endl;
	File_Close(0);
	File_Close(1);
	File_Close(2);
	File_Close(3);
	File_Close(4);
	cout << "Removing File /A/2\t" << File_Unlink("/A/2") << endl;
	cout << "Unlinking Directory /A\t" << DirUnlink("/A") << endl;
	cout << "Unlinking Directory /B\t" << DirUnlink("/B") << endl;
	cout << "Unlinking Directory /D\t" << DirUnlink("/D") << endl;
	cout << "Unlinking Directory /C\t" << DirUnlink("/C") << endl;
	cout << "Unlinking Directory /F\t" << DirUnlink("/F") << endl;
	cout << "Unlinking Directory /E\t" << DirUnlink("/E") << endl;
	cout << "Unlinking Directory /G\t" << DirUnlink("/G") << endl;
	cout << "Unlinking Directory /H\t" << DirUnlink("/H") << endl;
	cout << "Unlinking Directory /K\t" << DirUnlink("/K") << endl;

	DirCreate("/I");
	DirCreate("/A");
	DirCreate("/B");
	DirCreate("/D");
	DirCreate("/C");
	DirCreate("/F");
	DirCreate("/F");
	DirCreate("/E");
	DirCreate("/G");
	DirCreate("/H");
	DirCreate("/K");
	_temp="";
	_temp.resize(DirSize("/"));
	DirRead("/", _temp);
	cout << "Root has " << _temp << "\t to test whetehr file acccess after closign is workign\n";
	_temp = "";
	FS_Sync();
	FS_Reset();
	DirRead("/", _temp);
	cout << "Reading root after closing file system" << '\t' << _temp << endl;

	FS_Boot();


	_temp.resize(DirSize("/"));
	DirRead("/", _temp);
	cout << "Reading root after re opening file system" << '\t' << _temp << endl;



	return 0;

}
