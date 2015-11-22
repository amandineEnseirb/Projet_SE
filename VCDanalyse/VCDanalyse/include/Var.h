#ifndef VAR_H
#define VAR_H

#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "VCD_definitions.h"

#define SIZE_BYTE   8

using namespace std;


class Var
{
    public:
    enum Var_Types{EVENT, INTEGER, PARAMETER, REAL, REG, SUPPLY0, SUPPLY1, TIME, TRI, TRIAND, TRIOR, TRIREG, TRI0, TRI1, WAND, WIRE, WOR};
    enum Logic_Value{X, Z, High, Low};
    enum Display_Format{BINARY, DECIMAL, HEXADECIMAL};

    struct Sample{  vector<Logic_Value> value;
                    int timestamp;
                    };

    public:
        Var(){};
        Var(Var_Types var_type, int var_size, char var_identifier, const string& var_reference);
        Var(const string& str_var_type, int var_size, char var_identifier, const string& var_reference);
        virtual ~Var();

        char Getvar_identifier() { return var_identifier; }
        void Setvar_identifier(char val) { var_identifier = val; }

        size_t Getvar_size() { return var_size; }
        void Setvar_size(size_t val) { var_size = val; }

        string Getvar_reference() { return var_reference; }
        void Setvar_reference(const string& val) { var_reference = val; }

        Var_Types Getvar_type(){ return var_type;}
        void Setvar_type(Var_Types val) { var_type = val; }
        Var_Types detect_var_type(const string& str_var_type);

        void display_var(); //display the variable information
        void display_varSample(const size_t pos, const Display_Format df, const string timestamp_unit = VCD_s);
        void display_varSample(const int timestamp, const Display_Format df, const string timestamp_unit = VCD_s);

        string var_type2string(const Var_Types _vt);

        string LogicValue2string(const vector<Logic_Value>& _lv, const Display_Format df);
        char LogicValue2char(const Logic_Value lv);
        string LogicValue2dec(const Logic_Value lv);
        string LogicValue2hex(const Logic_Value lv);
        int LogicValue2int(const Logic_Value lv);

        int LogicByte2int(const Logic_Value lv8[SIZE_BYTE]);


        void AddSample(const Sample& _sample){var_values.push_back(_sample);}
        void AddSample(const string& str_val, const int timestamp);
        int GetSample(const size_t pos, Sample& _sample);
        int GetSample(const int timestamp, Sample& _sample){ return GetSample(findTimestampPos(timestamp), _sample);}
        Sample GetSample(const size_t pos){Sample s; (var_values.size() > pos) ? (s=var_values[pos]) : (s=var_values[var_values.size()-1]); return s;}
        size_t GetNumberOfSamples(){return var_values.size();}

        size_t findTimestampPos(const int _timestamp);
        void findRisingEdge(vector<vector<int>>& vect_time); //retourne un vecteur qui, pour chaque bit, contient un vecteur de timestamps où se trouvent les rising edge effectifs
        void findFallingEdge(vector<vector<int>>& vect_time);//retourne un vecteur qui, pour chaque bit, contient un vecteur de timestamps où se trouvent les falling edge effectifs

        void convertBinary2LogicValue(const string& str_val, vector<Logic_Value>& value);
        bool isLogicalValue(const char c, Logic_Value& _lv);

    protected:
    private:
        char var_identifier;
        size_t var_size;
        string var_reference;
        Var_Types var_type;
        vector<Sample> var_values;
};

#endif // VAR_H
