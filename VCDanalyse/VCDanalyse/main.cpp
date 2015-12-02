#include <iostream>
#include <string>

#include "AnalyseVCD.h"

using namespace std;


int main()
{
    cout <<"Launch !"<<endl;

   /* char scanCode = 0x15;

    char uns = convertScanToAsciiCode(scanCode, false);
    char s = convertScanToAsciiCode(scanCode, true);

    cout<<"Code u = "<<uns<<endl;
    cout<<"Code s = "<<s<<endl;

   */// const string vcd_file_name = "exemple.vcd";
    const string vcd_file_name = "test_clavier.vcd";
    const string init_file_name = "init.ini";
    const string output_file_name = "out.txt";

    AnalyseVCD a_vcd(vcd_file_name, init_file_name, output_file_name);


  /*  cout<<endl;
    a_vcd.show_listeVar();
    cout<<endl;

    vector<Var> va = a_vcd.Get_listeVar();
    cout<<"??????????????"<<endl;
    size_t pos;
    for(pos = 0; pos < va.size(); pos++)
            if(va[pos].Getvar_identifier() == '%'){
                va[pos].display_var();
                break;
            }

    va[pos].display_varSample((int)2302, Var::BINARY, a_vcd.Gettimescale_unit());
    va[pos].display_varSample((int)2303, Var::BINARY, a_vcd.Gettimescale_unit());

    cout<<"??????????????"<<endl;
    cout<<endl;
   // a_vcd.display_allVar((int)2500, Var::BINARY);
    a_vcd.display_FallingEdges();
    cout<<endl;
    a_vcd.display_RisingEdges();


    cout <<endl<< "end" << endl;
  */
    return 0;
}
