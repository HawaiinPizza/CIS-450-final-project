#ifndef DK_H
#define DK_H
/*{{{*/
#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#include "FS.h"
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
	Disk[3]=inodeBit;
	
	return 0;

}

#endif
