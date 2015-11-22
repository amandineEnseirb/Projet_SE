#ifndef WRITE_FILE_H
#define WRITE_FILE_H

#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

class Write_file
{
private:
    string filename;
    ifstream file;
public:
    Write_file(const string& s);
    ~Write_file();


    void WriteByte(char* buff);
};

#endif // WRITE_FILE_H
