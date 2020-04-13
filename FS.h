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

// Global data
bool FS_Lock=true;







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
				return 0;
			}
			else { // ERRORR. 
				osErrMsg="E_FILE_Boot";
				return -1;
			}
		}
	}
	// External is not made. As such, set the sueprblock to a number.
	ExtDisk[0]=SBNum;
	Disk_Init();
	Disk_Save();
	return 0;
} 


int FS_Sync(){ // Saves workign disk to extenral disk.
	f;
	for(int i=0; i<SectorNum; i++)
		ExtDisk[i]=WorkDisk[i];
	return 0;
}



int FS_Reset(){ // 
	f;	
	FS_Lock=true;
	return 0;
}


string FS_Check(){
	string temp="";
	if(FS_Lock)
		temp="Lock "+osErrMsg;
	else
		temp="No Lock"+osErrMsg;;
	return temp;
}


#endif
