#ifndef ANALYSEVCD_H
#define ANALYSEVCD_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Var.h"
#include "VCD_definitions.h"
#include "ScanCode_definitions.h"

using namespace std;

enum BitStartStopType{B1_High, B2_High, B1_Low, B2_Low};

class AnalyseVCD
{
    public:
        AnalyseVCD(){};
        AnalyseVCD(const string VCD_file_name, const string init_file_name, const string output_file_name);

        virtual ~AnalyseVCD();

        void Settimescale(int val) { timescale = val; }
        int Gettimescale() { return timescale; }
        void Settimescale_unit(const string& val) { timescale_unit = val; }
        string Gettimescale_unit() { return timescale_unit; }

        vector<Var> Get_listeVar() { return _listeVar; }
        void Add_listeVar(vector<Var> val) { _listeVar = val; }
        size_t Get_listeVar_size(){return _listeVar.size();}

        int Get_Var(size_t pos, Var& _var);
        bool FindVarByIdentifier(const char id, size_t& pos);

        int Add_Var(Var& _var);
        int Add_Var(const string& str_var_type, const size_t var_size, const char var_identifier, const string& var_reference);
        int Add_Var(const Var::Var_Types var_type, const size_t var_size, const char var_identifier, const string& var_reference);
        bool AddSampleToVar(const string& str, const int timestamp); //find the variable identifier and value in str and add the sample in the matching variable with the specified timestamp

        bool Getenddefinition() { return enddefinition; }
        void Setenddefinition(bool val) { enddefinition = val; }

        void read_init_file(const string& init_file_name);
        int get_VCD_header(ifstream& _vcd_file);
        int get_var_values(ifstream& _vcd_file);
        void read_VCD_file(const string& VCD_file_name);
        void analyse_VCD_data(const string& output_file_name);

        void show_listeVar();
        void display_allVar(const size_t pos, const Var::Display_Format df = Var::BINARY);
        void display_allVar(const int timestamp, const Var::Display_Format df = Var::BINARY);
        void display_RisingEdges();
        void display_FallingEdges();

        bool getDataOnClock(vector<vector<Var::Sample>>& data_stream, const bool onRisingEdge, vector<size_t>& pos);
        bool getDataInCommunicationProtocole(vector<vector<Var::Sample>>& data_on_clock_stream, BitStartStopType bit_start_type, size_t data_length, BitStartStopType bit_stop_type, vector<vector<char>>& data_from_protocole);

        char convertScanToAsciiCode(char ScanCode, bool isShift);
        void getAsciiFromScanCode(vector<vector<char>>& data_from_protocole, vector<vector<char>>& data_ascii);
        bool isShiftScanCode(unsigned char scanCode);

    protected:
    private:
        vector<Var> _listeVar;
        bool enddefinition = false;
        int timescale = 0;
        string timescale_unit = "s";
        char clock_id;
        vector<char> data_ids;
};

#endif // ANALYSEVCD_H
