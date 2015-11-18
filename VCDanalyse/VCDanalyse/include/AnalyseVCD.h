#ifndef ANALYSEVCD_H
#define ANALYSEVCD_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Var.h"
#include "VCD_definitions.h"

using namespace std;

class AnalyseVCD
{
    public:
        AnalyseVCD(){};
        AnalyseVCD(const string VCD_file_name, const string init_file_name, const string output_file_name);

        virtual ~AnalyseVCD();

        void Settimescale(float val) { timescale = val; }
        float Gettimescale() { return timescale; }

        vector<Var> Get_listeVar() { return _listeVar; }
        void Add_listeVar(vector<Var> val) { _listeVar = val; }

        int Get_Var(size_t pos, Var& _var);
        int Add_Var(Var& _var);
        int Add_Var(const string& str_var_type, int var_size, char var_identifier, const string& var_reference);
        int Add_Var(Var_Type var_type, int var_size, char var_identifier, const string& var_reference);

        bool Getenddefinition() { return enddefinition; }
        void Setenddefinition(bool val) { enddefinition = val; }

        void read_init_file(const string& init_file_name);
        int get_VCD_header(ifstream& _vcd_file);
        void analyse_VCD_file(const string& VCD_file_name, const string& output_file_name);

    protected:
    private:
        vector<Var> _listeVar;
        bool enddefinition = false;
        float timescale = 0.0;

};

#endif // ANALYSEVCD_H
