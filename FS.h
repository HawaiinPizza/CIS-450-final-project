#ifndef FS_H
#define FS_H

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>
#include <cassert>
#define MagicNum 18
#define SBNum 18
#include "Disk.h"
#include "BitData.h"






//Description: Boot up the filesystem
///Pre-condition: If the external disk exist, than check the magic number
//Post-condition: If magic number is valid OR file is empty, boot up disk

// FS_Boot() ExtDisk->Workign Disk or throw error if magic num differnt
// Removes external disk, and set up external disk.
int FS_Boot(){
	FS_Lock=false;
	//Assume external disk is not created if ALL VALUES are 0
	for(int i=0; i<SectorNum; i++) {
		// If eveyr value in external disk equals 0, than external disk is not made
		if(ExtDisk[i]!=0){ //Now external disk is conformed to be made.
			if(ExtDisk[0]==MagicNum){ // Workign as intended. Ext->Work
				for(i=0; i<SectorNum; i++){
					WorkDisk[i]=ExtDisk[i];
				}
				Log(0, "");
return 0;
			}
			else { // ERRORR. 
				osErrMsg="E_FILE_Boot";
				Log(-1, "");
return -1;
			}
		}
	}
	// External is not made. As such, set the sueprblock to a number.
	ExtDisk[0]=SBNum;
	Disk_Init(ExtDisk);
	Disk_Save();
	Log(0, "");
return 0;
} 


//Description: Save the current file system to disk
///Pre-condition: There is no lock
//Post-condition: Save the file system

int FS_Sync(){ // Saves workign disk to extenral disk.
	f;
	for(int i=0; i<SectorNum; i++)
		ExtDisk[i]=WorkDisk[i];
	Log(0, "");
return 0;
}



//Description: Sets a lock on the file system
///Pre-condition: file system isn't already locked.
//Post-condition: Locks the system.
int FS_Reset(){ // 
	if(FS_Lock)
		osErrMsg="E_FILE_RESET";
	f;	
	FS_Lock=true;
	Log(0, "");
	return 0;
}




#endif
