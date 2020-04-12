#include <iostream>/*{{{*/
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
/*}}}*/


int Disk_Init(disk(Disk) ){ // Setup the disk
	// Set each sector 0
	for(int i=0; i<SectorNum; i++){ 
		Disk[i]=0;
	}
	
	//Bitmap results The reason 1 is the first indoe and datablcok is root, and thus, it's unvaliable at first.
	Disk[0]=SBNum; //Sueprblock setting
	Disk[1]=1;
	Disk[2]=1; 

	// TODO Manually create root, so to better understand process
	inode Root; // Used later
	Root.isFile=false;
	Root.size=SectorSize;
	inodeSet(&Root, 0, 0); // How to set inodes

	//This is creating the root folder. Later on, we will make it with DirCreate("/")
	string inodeString=writeBitDataInode(Root).to_string();
	bitset<SectorSize*8> inodeBit(inodeString);
	string send="";
	for(int i=0; i<35; i++){
		send+=writeBitDataInode(Root).to_string();
	}
	bitset<4096> hard(send);
	Disk[3]=hard;

	for(int i=0; i<35; i++){
		assert(inodeString==inodeSubStr(Disk[3],i));
	}
	cout << "SUCCESS";



	
	return 0;

}
