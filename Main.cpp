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
#include "BitData.h"
#include "FS.h"
#include "Disk.h"
#include "Dir.h"
using namespace std;
#define siz 50
#define set 13
// Read and write are here for histroical purposes. They are take a size (how many inodes/directories can you fit in a sector) and set (aka how long is a inode/directory) and count (which spot in sector do you want to change" and maniplates them.
//
// THE KEY ASSUMPATION IS NOISE: there's also bits left over. their posistion is from 0->siz%set
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
int mainTest1(){
	char on='1';
	char off;
	ifelse(on=='1', off='0', off='1');
	string testStr;
	forloop(0, siz%set){ // Useless/nosie
		testStr+="1";
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
	return 0;
}
int mainTest2(){ // NOise testing
	char on='1';
	char off;
	ifelse(on=='1', off='0', off='1');
	string testStr;
	forloop(0, siz%set){ // Useless/nosie
		testStr+=on;
	}
	forloop(0,siz){
		ifelse((i/set)%2==0, testStr+=on, testStr+=off);
	}
	cout << "%:" << siz%set << '\t' <<  '\t';//  << testStr << ;;
	cout << endl;
	bitset<siz> test1(testStr);
	forloop(0, siz){
		if(range(i, 0, siz%set ))
			cout << "Noise\t";
		else
			cout << "Value\t";
		cout << i << '\t' << test1[i] << endl;
	}
	return 0;
}
	// This funciton is not expanedable yet.
	/* bool dirCreate(inode &node, string Name, int iSect, int dSect,  disk(Disk)){ // Creates an empty directory. ALSO, Disk should */
	/* 	dir Dir; */
	/* 	int posDir; */
	/* 	int posInode; */
	/* 	{ // Setting data and finding first empty partion */
	/* 		forloop(0,14){ */
	/* 			Dir.Name[i]=Name[i]; */
	/* 		} */
	/* 		Dir.Name[15]='\0'; */
	/* 		int temp=0; */
	/* 		forloop(0,35){ // Update to iteratie through the 3 inode sectors */
	/* 			if(readInodeSect(Disk[iSect],i)!=0){ */ 
	/* 				temp=i; */
	/* 				break; */
	/* 			} */
	/* 		} */
	/* 		posInode=temp; */
	/* 		Dir.inodePlace=posInode; */
	/* 	} */
	/* 	{// Create datablock dictionary */
	/* 		int temp=0; */
	/* 		forloop(0, dirSize){ // Update to iteratie through the 3 inode sectors */
	/* 			if(readDirSect(Disk[dSect],i)!=0){ */
	/* 				temp=i; */
	/* 				break; */
	/* 			} */
	/* 		} */
	/* 		posDir=temp; */
	/* 		bitset<dirSize> dirBit=writeDir(Dir); */
	/* 		cout << "\t\t" << dirBit << endl; */
	/* 		writeDirSect(Disk[posDir], posDir, dirBit); */
	/* 		node.alloc[0]=0; */ 
	/* 	} */
	/* 	{//This is creating the root folder. Later on, we will make it with DirCreate("/") */
	/* 	string inodeString=writeBitDataInode(node).to_string(); */
	/* 	bitset<SectorSize*8> inodeBit(inodeString); */
	/* 	writeInodeSect(Disk[posInode], posInode, inodeString); */
	/* 	return true; */
	/* 	} */
	/* } */

int DirCreate(string path){ // Haven't implmented getDirpath
	/* struct pos{ */
	/* 	uint count; */
	/* 	uint sect; */

	/* 	pos(int _count, int _sect){ */
	/* 		count=_count; */
	/* 		sect=_sect; */
	/* 	} */
	/* }; */

	/* // Assume root node is made. */
	
	/* pos posDir(-1,-1); */
	/* pos posInode(-1,-1); */
	/* { // Get the next empty space */
	/* 	// 0 is superblock, 1 is inode bitmap, 2 is dB bitmap, 3-6 are inode bitmap, and the rest is datablock. In case i change, this is why it's 3 and 6 */

	/* 	//Finding hte next empty inode */
	/* 	for(int i=3; i<6 && posInode.count==-1; i++){ */ 
	/* 		forloop2(0, inodeCount){ */
	/* 			if(readInodeSectBit(WorkDisk[i], j )==0){ */
	/* 				posInode=pos(j,i); */
	/* 				break; */
	/* 			} */
	/* 		} */
	/* 	} */


	/* 	//Finding hte next empty Dir */
	/* 	for(int i=inodeOffset; i<SectorNum && posDir.count==-1; i++){ */
	/* 		forloop2(0, dirCount){ */
	/* 			if(readDirSect(WorkDisk[i], j )==0){ */
	/* 				posDir=pos(j, i); */
	/* 				break; */
	/* 			} */
	/* 		} */
	/* 	} */
	/* } */

	/* //Determine if you create in root or have to go into folders. */
	/* //Create a string vector/queue,  for this. */
	/* //TODO include cehck if path is nonsensical */
	/* bool isRoot=false; */


	/* { // Find parent inode and append it. */

	/* 	cout << "ROOT?\t" << isRoot << endl; */
	/* 	if(posInode.count>=0 && posDir.count>=0){ // Create new dictionary, now we have 2 avaiable spaces */
	/* 		cout << "Sect\t" << posInode.sect << " Count\t" << posInode.count << endl; */
	/* 		cout << "Sect\t" << posDir.sect   << " Count\t" << posDir.count << endl; */

	/* 	if(isRoot){// Parent is in root. Just create dictionary and inode */
	/* 			dir newdir; */
	/* 			forloop(0, 14){ */
	/* 				newdir.Name[i]=path[i]; */
	/* 			} */
	/* 			newdir.inodePlace=(posInode.sect-3)*inodeCount +  posInode.count; */

	/* 			forloop(0,dirCount){ // Checking in the folder, to make sure it's not already tehere */
	/* 				dir Dir=readDirSectDir(WorkDisk[posDir.sect], i); */

	/* 			} */
	/* 			//inode FromRoot(0, 0, posInode.count ); */
	/* 			//writeInodeSectInode(WorkDisk[posInode.sect], posInode.count, Root); */

	/* 		} */
	/* 	} */

	/* 	else{ // There is no more space, so don't create a new inode */
	/* 		cout << "NO MORE SPACE"; */
	/* 	} */

	/* } */
	/* return 0; */
}

int getInode(string path){

	int inode=0;
	bool isRoot=false;
	vector<string> retStr;
	{ // See if root and if not, get the next child
		{
			if(path[path.length()-1]!='/')
				path+='/';
			string tempStr="";
			for(int i=1; i<path.length(); i++){
				if(path[i]=='/'){
					retStr.push_back(tempStr);
					tempStr="";
				}
				else{
					tempStr+=path[i];
				}
			}


			if(retStr.size()==1){
				isRoot=true;
			}
		}
	}

	return -1;
}

int main(){
	FS_Boot();

	//Creating directories manually
	inode RootInode(0,2,0);
	inode AInode(0,1, 1);
	inode BInode(0,8,2);
	inode A1Inode(0,9,3);

	dir RootDir("/", 0);
	dir ADir("A", 1);
	dir BDir("B", 2);
	dir A1Dir("1", 3);
	
	writeInodeSectInode(WorkDisk[3], 0, RootInode);
	writeInodeSectInode(WorkDisk[3], 1, AInode);
	writeInodeSectInode(WorkDisk[3], 2, BInode);
	writeInodeSectInode(WorkDisk[3], 3, A1Inode);

	writeDirSectDir(WorkDisk[6], 0, RootDir);
	writeDirSectDir(WorkDisk[7], 0, ADir);
	writeDirSectDir(WorkDisk[8], 0, BDir);
	writeDirSectDir(WorkDisk[9], 0, A1Dir);

	writeDirSectDir(WorkDisk[6], 1, ADir);
	writeDirSectDir(WorkDisk[6], 2, BDir);


	writeDirSectDir(WorkDisk[7], 1, A1Dir);
	writeDirSectDir(WorkDisk[7], 2, A1Dir);

	string find="/A";
	forloop(0, dirCount){ // This is code for finding something in root.
		auto a= readDirSect(WorkDisk[6], i); //Notice the 6. That is becasue it's where the root direcotry will be
		if(a!=0){
			dir b=getBitDir(a);
			if(b.Name==find) {
				cout << b.inodePlace << endl;
			}
		}
	}

	queue<string> breakUp;
	//breakUp.push("/");
	breakUp.push("A");
	breakUp.push("1");

	bool stop=false;
	int Sect=6;
	while(!breakUp.empty() && !stop){
		string find=breakUp.front(); breakUp.pop();
		forloop(0, dirCount){ // This is code for finding something in root.
			auto a= readDirSect(WorkDisk[Sect], i); 
			if(a!=0){
				dir b=getBitDir(a);
				cout << find << ":" << b.Name << '\t';
				if(b.Name==find && !breakUp.empty()) { // This isn't what we're looking for. Go to antoher stage
					cout << "CLUE\t" ;
		/* 			newdir.inodePlace=(posInode.sect-3)*inodeCount +  posInode.count; */
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					inode temp=readInodeSectInode(WorkDisk[_sect], _place);
					cout << 
						"inode\t" << b.inodePlace << '\t' <<
						"SECT\t" << _sect << '\t' << 
						"Place\t" << _place << '\t' <<
						"Alloc\t" << temp.alloc[0] << '\t'
						<< endl;

						;
					Sect=temp.alloc[0]+6;
					break;

				}
				else if(b.Name == find && breakUp.empty()){
						cout << "HUH\t" << find;
						stop=true;
						break;
				}
				else
					cout << "STILL SEARCHING FOR HER\t"; 

				cout << endl;
			}

		}
	}


	cout << endl << "DONE" << endl;


	forloop( 6, 9){
		forloop2(0,dirCount){
			auto a= readDirSect(WorkDisk[i], j);
			if(a!=0){
				dir b=getBitDir(a);
				cout << i << '\t' << j << '\t' << b.Name << endl;
			}
		}
		cout << endl;
	}




}
