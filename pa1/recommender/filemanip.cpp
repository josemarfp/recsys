#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int file2Vector(string &fileName)
{
    ifstream refFile(fileName);
    vector <string> wordsDB_mem;
        string currentLine = "";
	    int lines = 0;
	    wordsDB_mem.reserve(10000000);              //!! stage 3

	    while(getline(refFile, currentLine)) {
	        wordsDB_mem.push_back(currentLine);     //!! stage 2
	    }    
        
    return lines;
}

int main()
{
    file2Vector("./dataset/ratings.csv");
}
