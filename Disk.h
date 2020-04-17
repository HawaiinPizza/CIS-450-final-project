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


//Description: Setup the disk to have root and rest all zeros
	///Pre-condition: Input of a disk
	//Post-condition: Create a disk.
int Disk_Init(disk(DISK)){ // Setup the disk
	f;
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


	Log(0, "");
	return 0;
}
//Description: Save the working disk onto the extenral disk
	///Pre-condition:
	//Post-condition: Working disk si saved.

// Save and load functions
int Disk_Load(){ // workDisk->extDisk
	f;
	for(int i=0; i<SectorNum; i++ )
		ExtDisk[i]=WorkDisk[i];
	Log(0, "");
	return 0;
}

//Description: Load the external disk onto ram/working disk
	///Pre-condition: 
	//Post-condition: Working disk has external disk value
int Disk_Save(){ // extDisk->workDisk
	f;
	for(int i=0; i<SectorNum; i++ )
		WorkDisk[i]=ExtDisk[i];
	Log(0, "");
	return 0;
}

//Description: Write the disk given the buffer
	///Pre-condition: The buffer is of correct size and sector is within range
	//Post-condition: WRite the buffer onto the disk
// Read and write Functions The method signetuare is differnt, disk is not supposed to be there.
int Disk_Write( string &buffer, int sector){ // I did not check for buffer being null. IDK what to do in that scenario. 
	f;
	if(buffer.length()!=SectorBit || sector <0 || sector >= SectorNum ){ // One of these conditions happne, which are INVLAID
		diskErrMsg="E_WRITE_INVALID_PARM";
		string _temp=buffer;
		_temp+=" ";
		_temp+=to_string(sector);
		Log(-1, _temp);
		return -1;
	}
	else{
		bitset<SectorBit> Write(buffer);
		WorkDisk[sector]=Write;

	}

	string _temp=buffer;
	_temp+=" ";
	_temp+=to_string(sector);
	Log(0, _temp);
	return 0;
}
//Description: Read the value of a sector
///Pre-condition: The buffer is of correct size and sector is in range.
//Post-condition: Set teh buffer to correct value
int Disk_Read( string &buffer, int sector){ // I did not check for buffer being null. IDK what to do in that scenario. 
	f;
	if(sector <0 || sector >= SectorNum ){ // One of these conditions happne, which are INVLAID
		diskErrMsg="E_READ_INVALID_PARM";
		string _temp=buffer;
		_temp+=" ";
		_temp+=to_string(sector);
		Log(-1, _temp);
		return -1;
	}
	else{
		bitset<SectorBit> Read(buffer);
		Read=WorkDisk[sector];
		buffer=Read.to_string();



	}

	string _temp=buffer;
	_temp+=" ";
	_temp+=to_string(sector);
	Log(0, _temp);
	return 0;
}



#endif
