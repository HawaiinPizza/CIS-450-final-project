#ifndef DK_H
#define DK_H

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#include "FS.h"
#include "BitData.h"



int Disk_Init(disk(DISK)){ // Setup the disk
	// Set each sector 0
	for(int i=0; i<SectorNum; i++){ 
		DISK[i]=0;
	}

	//Bitmap results The reason 1 is the first indoe and datablcok is root, and thus, it's unvaliable at first.
	DISK[0]=SBNum; //Sueprblock setting
	DISK[1]=1;
	DISK[2]=1; 

	// TODO Manually create root, so to better understand process
	inode Root; // Used later
	Root.isFile=false;
	Root.size=0;
	//inodeSet(&Root, 0, 0); // How to set inodes
	Root.alloc[0]=inodeOffset;

	dir RootDir;
	RootDir.Name[0]='/';
	RootDir.Name[1]='\0';
	RootDir.inodePlace=0;

	// Create datablock dictionary
	bitset<dirSize> dirBit=getDirBit(RootDir);
	dir test=getBitDir(dirBit.to_string());

	writeDirSect(DISK[inodeOffset], 0, dirBit);
	

	//This is creating the root folder. Later on, we will make it with DirCreate("/")

	string inodeString=getInodeBit(Root).to_string();
	bitset<SectorSize*8> inodeBit(inodeString);

	writeInodeSect(DISK[3], 0, inodeString);


	return 0;
}

// Save and load functions
int Disk_Load(){ // workDisk->extDisk
	for(int i=0; i<SectorNum; i++ )
		ExtDisk[i]=WorkDisk[i];
	return 0;
}

int Disk_Save(){ // extDisk->workDisk
	for(int i=0; i<SectorNum; i++ )
		WorkDisk[i]=ExtDisk[i];
	return 0;
}

// Read and write Functions The method signetuare is differnt, disk is not supposed to be there.
int Disk_Write( string &buffer, int sector){ // I did not check for buffer being null. IDK what to do in that scenario. 
	if(buffer.length()!=SectorBit || sector <0 || sector >= SectorNum ){ // One of these conditions happne, which are INVLAID
		diskErrMsg="E_READ_INVALID_PARM";
		return -1;
	}
	else{
		bitset<SectorBit> Write(buffer);
		WorkDisk[sector]=Write;

	}

	return 0;
}

int Disk_Read( string &buffer, int sector){ // I did not check for buffer being null. IDK what to do in that scenario. 
	if(sector <0 || sector >= SectorNum ){ // One of these conditions happne, which are INVLAID
		diskErrMsg="E_READ_INVALID_PARM";
		return -1;
	}
	else{
		bitset<SectorBit> Read(buffer);
		Read=WorkDisk[sector];
		buffer=Read.to_string();



	}

	return 0;
}



#endif
