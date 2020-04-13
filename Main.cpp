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


inode getInode(string path){

	bool isRoot=false;
	queue<string> RetStr;
	inode Ret;
	RetStr.push("/");
	{ // See if root and if not, get the next child
		{
			if(path[path.length()-1]!='/')
				path+='/';
			string tempStr="";
			for(int i=1; i<path.length(); i++){
				if(path[i]=='/'){
					RetStr.push(tempStr);
					tempStr="";
				}
				else{
					tempStr+=path[i];
				}
			}


		}
	}

	// Getting the node here.
	bool stop=false;
	int Sect=6; //There is where root dictionary will be
	while(!RetStr.empty() && !stop){
		string find=RetStr.front(); RetStr.pop();
		forloop(0, dirCount){ // This is code for finding something in root.
			auto a= readDirSect(WorkDisk[Sect], i); 
			if(a!=0){
				dir b=getBitDir(a);
				/* cout << find << ":" << b.Name << '\t'; */
				if(b.Name==find && !RetStr.empty()) { // This isn't what we're looking for. Go to antoher stage
					/* cout << "CLUE\t" ; */
					/* 			newdir.inodePlace=(posInode.sect-3)*inodeCount +  posInode.count; */
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					inode temp=readInodeSectInode(WorkDisk[_sect], _place);
					/* cout << */ 
					/* 	"inode\t" << b.inodePlace << '\t' << */
					/* 	"SECT\t" << _sect << '\t' << */ 
					/* 	"Place\t" << _place << '\t' << */
					/* 	"Alloc\t" << temp.alloc[0] << '\t' */
					/* 	<< endl; */

					;
					Sect=temp.alloc[0]+6;
					break;

				}
				else if(b.Name == find && RetStr.empty()){
					/* cout << "HUH\t" << find; */
					stop=true;
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					Ret=readInodeSectInode(WorkDisk[_sect], _place);
					return Ret;

					break;
				}
				/* else */
				/* 	cout << "STILL SEARCHING FOR HER\t"; */ 

			}

		}
	}

	// At this point we didn't find it
	return inode();
}

inode NOgetInode(string path){

	bool isRoot=false;
	queue<string> RetStr;
	inode Ret;
	RetStr.push("/");
	string testStr="";
	{ // See if root and if not, get the next child
		{
			if(path[path.length()-1]!='/')
				path+='/';
			string tempStr="";
			for(int i=1; i<path.length(); i++){
				if(path[i]=='/'){
					RetStr.push(tempStr);
					testStr+=tempStr;
					tempStr=path[i];
				}
				else{
					tempStr+=path[i];
				}
			}


		}
		cout << testStr << " THIS IS HOW IT LOOKS \n";
	}

	// Getting the node here.
	bool stop=false;
	int Sect=6; //There is where root dictionary will be
	while(!RetStr.empty() && !stop){
		string find=RetStr.front(); RetStr.pop();
		cout << find << "NEW ONE\n";
		forloop(0, dirCount){ // This is code for finding something in root.
			auto a= readDirSect(WorkDisk[Sect], i); 
			if(a!=0){
				dir b=getBitDir(a);
				//'cout << b.Name  << '\t' ;
				cout << find << ':' << b.Name << ' ' << RetStr.size() << '\t';
				if(b.Name==find && !RetStr.empty()) { // This isn't what we're looking for. Go to antoher stage
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
				else if(b.Name == find && RetStr.empty()){
					cout << "FOUDN AND GOING TO RETURN :)" << endl;
					stop=true;
					int _sect=3+b.inodePlace/35;
					int _place=b.inodePlace%35;
					Ret=readInodeSectInode(WorkDisk[_sect], _place);
					return Ret;

					break;
				}
				cout << endl;

			}

		}
	}

	// At this point we didn't find it
	return inode();
}
int DirCreate(string path){ // Haven't implmented getDirpath
	inode child=getInode(path);
	size_t found = path.find_last_of("/");
	inode parent=getInode(path.substr(0,found));
	if(parent.size==inode().size){ // Parent does not exist
		return -1;
	}
	else{
		if (child.size != inode().size){ // Child alreayd exist
			return -2; 
		}
		else if (path.length()>256 ){ // Checking if path exceeds 256 characters
			return -3;
		}
		else{ // Can make dirctioanry.
			child.isFile=false;
			child.size=1;
			child.alloc[0];

			dir Dir;
			struct pos{
				int Sect;
				int Count;
				int Alloc;
				int Dlloc;
				pos(){
					Sect=-1;
					Count=-1;
					Alloc=Sect*35+Count;
				}
				pos(int _Sect, int _Count){
					Sect=_Sect;
					Count=_Count;
					Alloc=Sect*35+Count;
					Dlloc=(Sect-6)*31+Count;

				}
			};
			pos posDir;
			pos posParDir;
			pos posInode;

			bool STOP=false;
			for(int i=0; i<3 && !STOP; i++){ // Finding the first free inode space
				forloop2(0,34){
					if(readInodeSectBit(WorkDisk[i], j)==0){
						/* int _sect=3+b.inodePlace/35; */
						/* int _place=b.inodePlace%35; */
						posInode=pos(i,j);
						STOP=true;
					}
					if(posInode.Count!=-1){ // Stop looking, since we already found one.
						break;
					}
				}
			}

			STOP=false;
			for(int i=6; i<SectorNum && !STOP; i++){ // Finding the first free dir space
				if(WorkDisk[i]==0){
					posDir=pos(i,0);
					break;
				}
			}

			for(int i=0; i<10; i++){ // Finding the space for the parent dictionary
				int _sect=6+parent.alloc[i];
				forloop2(0,dirCount){
					bitset<dirSize> bitStream( readDirSect(WorkDisk[_sect], j));
					if(bitStream==0){ // INCLUDE CHECK THAT IF THIS DIR RUNS OUT OF SPACE, YOU APPEND A NEW SPACE
						/* int _sect=3+b.inodePlace/35; */
						/* int _place=b.inodePlace%35; */
						posParDir=pos(_sect,j);
					}
					if(posParDir.Count!=-1) // Stop looking, since we already found one.
						break;
				}
				if(posParDir.Count!=-1) // Stop looking, since we already found one.
					break;
			}

			if(posDir.Count!=-1 && posInode.Count!=-1 && posParDir.Count!=-1){ // This means there is free space for a new direcotry
				string temp=path.substr(found+1);
				dir NewDir(temp, 0);
				writeDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count, NewDir );
				writeDirSectDir(WorkDisk[posDir.Sect], posDir.Count, NewDir );
				dir parDir=readDirSectDir(WorkDisk[posParDir.Sect], posParDir.Count);
				child.alloc[0]=posInode.Alloc;
				child.size=0;
				writeInodeSectInode(WorkDisk[posInode.Sect], posInode.Count, child);

				/* cout << readInodeSectBit(WorkDisk[posInode.Sect], posInode.Count); */
				/* cout << "\nToo busy printin\n"; */
				/* cout << "ParentNode\t" << parent.size << '\t' << parent.alloc[0] << endl; */
				/* cout << "Path\t" << path << '\t' << path.substr(0,found) << endl; */
				/* cout << "Dir\t" << posDir.Count << '\t' << posDir.Sect << endl; */
				/* cout << "ParDir\t" << posParDir.Count << '\t' << posParDir.Sect << endl; */
				/* cout << "ChildNode\t" << child.size << '\t' << child.alloc[0] << endl; */
				/* cout << "New Inode\t" << posInode.Sect << '\t' << posInode.Count << endl; */
				/* cout << "New Dir\t" << parDir.Name << '\t' << parDir.inodePlace  << endl; */
				/* cout << endl; */




				return 0;
			}
			else // There is no more space for a new directory
				return -4;


		}

	}
}

int main(){
	FS_Boot();

	//Creating directories manually
	inode RootInode	(0,10,	0);
	inode AInode	(0,20, 	1);
	inode BInode	(0,40,	2);
	inode A1Inode	(0,80,	3);

	dir RootDir("/", 0);
	dir ADir("A", 1);
	dir BDir("B", 2);
	dir A1Dir("1", 3);

	writeInodeSectInode(WorkDisk[3], 0, RootInode);
	writeInodeSectInode(WorkDisk[3], 1, AInode);
	writeInodeSectInode(WorkDisk[3], 2, BInode);
	writeInodeSectInode(WorkDisk[3], 3, A1Inode);

	writeDirSectDir(WorkDisk[6+RootInode.alloc[0]], 0, RootDir);
	writeDirSectDir(WorkDisk[6+BInode.alloc[0]], 0, BDir);
	writeDirSectDir(WorkDisk[6+A1Inode.alloc[0]], 0, A1Dir);
	writeDirSectDir(WorkDisk[6+AInode.alloc[0]], 0, ADir);

	writeDirSectDir(WorkDisk[6], 1, ADir);
	writeDirSectDir(WorkDisk[6], 2, BDir);


	writeDirSectDir(WorkDisk[7], 1, A1Dir);


	inode test1=getInode("/");
	inode test2=getInode("/A");
	inode test3=getInode("/A/1");
	inode test4=getInode("/A/D");
	cout << test1.size << '\t' << test1.alloc[0] << endl;
	cout << test2.size << '\t' << test2.alloc[0] << endl;
	cout << test3.size << '\t' << test3.alloc[0] << endl;
	cout << test4.size << '\t' << test4.alloc[0] << endl;
	cout << endl;


	cout << "DIR -1\t" << DirCreate("/A/B/C/D")  << endl;
	cout << "DIR -2 \t" << DirCreate("/A/B")  << endl;
	cout << "DIR -2  \t" << DirCreate("/A/")  << endl;
	cout << "DIR 0\t" << DirCreate("/A/7") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/7") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/7/J") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/7/IO") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/7/ZXJCI") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/9") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/2") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/q") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/h") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/h") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/hojaid") << endl ;
	cout << "DIR -2\t" << DirCreate("/A/h9j1id") << endl ;


	forloop(0, 32){
		bitset<132> stream=readDirSect(WorkDisk[7], i);
		if(stream!=0){
			dir Dir=getBitDir(stream);
			cout << Dir.Name << '\t' << Dir.inodePlace << endl;
		}
	}






}
