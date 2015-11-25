#include <iostream>
#include <string>

#include "include/AnalyseVCD.h"

using namespace std;


int main()
{
    cout <<"Launch !"<<endl;

    string buf;
    string init_file_name = ".\\initialisation.ini";
    string vcd_file_name = "test_clavier.vcd";
    string output_file_name = "out.txt";

    cout<<"Enter initialisation file name : ";
    cin>>buf;
    if(buf.length()>0)
    {
        init_file_name = ".\\";
        init_file_name += buf;
        if(init_file_name.compare(init_file_name.size()-4, 4, ".ini"))
           init_file_name += ".ini";
    }

    cout<<"Enter vcd file name : ";
    cin>>buf;
    if(buf.length()>0)
    {
        vcd_file_name = buf;
        if(vcd_file_name.compare(vcd_file_name.size()-4, 4, ".vcd"))
           vcd_file_name += ".vcd";
    }

    cout<<"Enter output file name : ";
    cin>>buf;
    if(buf.length()>0)
    {
        output_file_name = buf;
        if(output_file_name.compare(output_file_name.size()-4, 4, ".txt"))
           output_file_name += ".txt";
    }


    AnalyseVCD a_vcd(vcd_file_name, init_file_name, output_file_name);

    cout<<"End of program. Enter any key to kill it.";
    cin>>buf;
    return 0;
}
