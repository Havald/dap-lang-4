#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <vector> 
#include <algorithm>
using namespace std;


class Transformation {
	
	/* You must not edit above this line */
		
	public: Transformation(const string &From, const string &To) {
		
		/* Implemet your Solution here! */
		
		;
	}
	
	
	public: size_t Cost() { 
		
		/* Implemet your Solution here! */
		
		return 0;
		
	}
	
	friend ostream& operator<< (ostream &TheOstream, Transformation &me) {
		
		/* Implemet your Solution here! */
		
		throw ("Long output not implemented yet");
		
		return TheOstream;
	};
	
	/* You must not edit below this line */
};


void usage() { throw "Usage: edd String1 String2 [ -o ]"; }

int main (int argc, char* argv[]) {
  
	bool LongOutput=false; // If true the complete edit steps are printed line by line

	try {
		if (argc<3||argc>4) usage();
	
		if (argc==4) {
			string arg = string(argv[3]);
			if (!arg.compare("-o")) LongOutput = true; else usage();
		}
	
		Transformation Try= Transformation(string(argv[1]), string(argv[2])); // Calculate edit steps
		cout << "Transformation from " << argv[1] << " to " << argv[2] << " takes " << Try.Cost() << " Operations" << endl;
		if (LongOutput) cout << endl << Try << endl;// optional long output
		
	} catch (const char *Reason) { cerr << Reason << endl; exit(1); }
   
	return 0;
}
