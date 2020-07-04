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
private:
	string first, second;
	size_t cost;
	vector<vector<string>> steps;

public: 
	Transformation(const string &From, const string &To) {
		first = From;
		second = To;
		size_t n = first.size();
		size_t m = second.size();
		int **distance; // Tabelle, welche die Editierdistanz für Teilprobleme abspeichert. Wird benötigt, um die Sequence zu erstellen
		try {
			distance = new int*[n + 1]; // 2D Feld initialisieren.
			for(size_t i = 0; i < n + 1; i++) {
				distance[i] = new int[m + 1];
			}
		} catch(...) {
			throw "No Memory";
		}
		
		try {
			steps = vector<vector<string>>(n + 1, vector<string>(m + 1, "")); // Hier wird die Abfolge der Operationen (Sequence) gespeichert
		} catch(...) {
			throw "No Memory";
		}
		for(size_t i = 0; i <= n; i++) { // Minimale Editierdistanz nach Levensthein berechnen. Siehe: https://en.wikipedia.org/wiki/Levenshtein_distance
			for(size_t j = 0; j <= m; j++) {
				if(i == 0 && j == 0) {
					distance[0][0] = 0;
				} else if(i == 0) {
					distance[0][j] = j;
					steps[0][j] = steps[0][j - 1] + "i"; // Neben der distance wird auch die Abfolge gespeichert. i -> insert, d -> delete, e -> exchange
				} else if(j == 0 ){
					distance[i][0] = i;
					steps[i][0] = steps[i - 1][0] + "d";
				} else if(first[i - 1] == second[j - 1]) {
					distance[i][j] = distance[i - 1][j - 1];
					steps[i][j] = steps[i - 1][j - 1] + "n";
				} else {
					distance[i][j] = min(distance[i - 1][j] + 1, min(distance[i - 1][j - 1] + 1, distance[i][j - 1] + 1));
					if(distance[i - 1][j] <= distance[i - 1][j - 1] && distance[i - 1][j] <= distance[i][j - 1]) {
						 steps[i][j] = steps[i - 1][j] + "d";
					} else if(distance[i][j - 1] <= distance[i - 1][j] && distance[i][j - 1] <= distance[i - 1][j - 1]) {
						 steps[i][j] = steps[i][j - 1] + "i";
					} else {
						 steps[i][j] = steps[i - 1][j - 1] + "e";
					} 
				}
			}
		}
		cost = distance[n][m];
		for(size_t i = 0; i <= first.size(); i++) { // 2D Feld löschen
			delete[] distance[i];
		}
		delete[] distance;
	}

	string getFirst() {
		return first;
	}
	string getSecond() {
		return second;
	}
	string getSequence() {
		return steps[steps.size() - 1][steps[0].size() - 1];
	}
	size_t Cost() { 
		return cost;
	}
	
	friend ostream& operator<< (ostream &TheOstream, Transformation &me) {
		string toConvert = me.getFirst();
		string second = me.getSecond();
		string sequence = me.getSequence();

		int maxsize = 0; // Ermitteln, wie lang der String waehrend der Operationen maximal wird, um die Ausgabe ausrichten zu koennnen
		for(size_t i = 0; i < sequence.size(); i++) {
			static int size = 0;
			if(sequence[i] == 'd') { // Bei jedem delete wird der String kürzer...
				size--;
			}
			if(size > maxsize) maxsize = size;

			if(sequence[i] == 'i') { // ...bei jedem Insert länger. Nach dem Vergleich, da ein Insert an der letzten Position die Länge für die Ausgabe nicht mehr verändert
				size++;
			}
		}
		maxsize += toConvert.size();
		
		size_t n = 0, count = 0;
		for(size_t i = 0; i < sequence.size(); i++) {
			if(sequence[i] == 'n') {
				n++;
				continue;
			}
			TheOstream << "Step ";
			TheOstream.width(3); TheOstream << ++count << " is to "; 
			if(sequence[i] == 'e') {
				TheOstream << "exchange " << toConvert[n] << " at Position "; 
				TheOstream.width(3); TheOstream << n << " with " << second[n] << ": ";
				TheOstream.width(maxsize); TheOstream << toConvert << " -> ";
				toConvert[n] = second[n];
				TheOstream << toConvert << endl;
			} else if(sequence[i] == 'i') {
				TheOstream << "insert   " << second[n] << " at Position ";
				TheOstream.width(3); TheOstream << n << "       : ";
				TheOstream.width(maxsize); TheOstream << toConvert << " -> ";
				toConvert.insert(n, second, n, 1);
				TheOstream << toConvert << endl;
			} else if(sequence[i] == 'd') {
				TheOstream << "delete   " << toConvert[n] << " at Position "; TheOstream.width(3); TheOstream << n << "       : ";
				TheOstream.width(maxsize); TheOstream << toConvert << " -> ";
				toConvert.erase(n, 1);
				TheOstream << toConvert << endl;
				n--;
			} 
			n++;
		}
		return TheOstream;
	};
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
