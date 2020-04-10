// Whoel Porgram decompsiostion
//    This is an outline/code of how the whole program will be. 
//    #+HEADER: :noweb yes :tangle Main.cpp   :colnames no :comments org
// #+HEADER: :includes "<iostream> <cmath> <vector> <climits> <bitset>"

#include <iostream>
#include <cmath>
#include <vector>
#include <climits>
#include <bitset>


int SectorSize = 512;
int SectorNum = 1000;
int inodeCount = 21;





#include <iostream>
#include <cmath>
using namespace std;

#define SIZE 512
#define NUM 1000
     using namespace std;

    class inode{
	    private: 
		bool isFile;
		const uint SizeMax = SIZE*8;
		const uint BlocksMax = NUM;
		uint Size=0;
		bitset<NUM> allBlocks;
	

		double log2(uint num){return log10(num)/log10(2);}



	    public:
		inode(){
		    isFile=true;
		    allBlocks=5;
		}

		void writeInode(){
		    //Assume Sector size is 512 bytes, with the sector number being 1000.
		    {
		    int Size=1 + (int)ceil(log(SizeMax)) + (int)ceil(log2(BlocksMax));
		    bool Bitmap[Size];
		    cout << Size << std::endl <<'\t' << 
			 1 <<'\t' <<
			 ceil(log2((SizeMax))) <<'\t' <<
			 ceil(log2((BlocksMax))) <<std::endl; 		
			 }


		    //Get bit representation of Size
		    //Since the maxium of inodes is 512*10 datablocks, so the maxium size is 4096 is 12 bits.
		    {
		    bitset<SIZE> temp(Size);
		    cout << temp.to_ulong() << endl;
		    }
		    
		    



		    //Get bit representation of Blocks
		    {
		    cout << allBlocks.to_ulong() << endl;
		    }




		}
		bool setSectorSize(uint Size){
		    if(Size>=SizeMax)
			    return false;
		    this->Size=Size;
		    return true;

		}
		bool setBlocks(uint Blocks[], int size){
		    return true;

		}
		void printMe(){ std::cout <<
			isFile <<'\t' <<
			SizeMax <<'\t' <<
			BlocksMax << '\t' <<
			Size << '\t' <<
			std::endl;
		}
    };






int main(){
   inode temp;
   temp.writeInode();
    return 0;
}
