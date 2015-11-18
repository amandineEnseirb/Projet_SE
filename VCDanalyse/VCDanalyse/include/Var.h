#ifndef VAR_H
#define VAR_H

#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

enum Var_Type{EVENT, INTEGER, PARAMETER, REAL, REG, SUPPLY0, SUPPLY1, TIME, TRI, TRIAND, TRIOR, TRIREG, TRI0, TRI1, WAND, WIRE, WOR};

class Var
{
    public:
        Var(){};
        Var(Var_Type var_type, int var_size, char var_identifier, const string& var_reference);
        Var(const string& str_var_type, int var_size, char var_identifier, const string& var_reference);
        virtual ~Var();
        char Getvar_identifier() { return var_identifier; }
        void Setvar_identifier(char val) { var_identifier = val; }
        int Getvar_size() { return var_size; }
        void Setvar_size(int val) { var_size = val; }
        string Getvar_reference() { return var_reference; }
        void Setvar_reference(const string& val) { var_reference = val; }
        Var_Type Getvar_type() { return var_type; }
        void Setvar_type(Var_Type val) { var_type = val; }
    protected:
    private:
        char var_identifier;
        int var_size;
        string var_reference;
        Var_Type var_type;
        Var_Type detect_var_type(const string& str_var_type);
};

#endif // VAR_H
