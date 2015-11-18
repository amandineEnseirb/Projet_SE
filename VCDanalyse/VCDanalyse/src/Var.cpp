#include "Var.h"

Var::Var(Var_Type var_type, int var_size, char var_identifier, const string& var_reference)
{
    Setvar_identifier(var_identifier);
    Setvar_reference(var_reference);
    Setvar_size(var_size);
    Setvar_type(var_type);
}

Var::Var(const string& str_var_type, int var_size, char var_identifier, const string& var_reference)
{
    Setvar_identifier(var_identifier);
    Setvar_reference(var_reference);
    Setvar_size(var_size);
    Setvar_type(detect_var_type(str_var_type));
}

Var::~Var()
{
    //dtor
}

Var_Type Var::detect_var_type(const string& str_var_type)
{
    if(!str_var_type.compare("EVENT") || !str_var_type.compare("event") || !str_var_type.compare("Event"))
        return Var_Type::EVENT;

    if(!str_var_type.compare("INTEGER") || !str_var_type.compare("integer") || !str_var_type.compare("Integer"))
        return Var_Type::INTEGER;

    if(!str_var_type.compare("WIRE") || !str_var_type.compare("wire") || !str_var_type.compare("Wire"))
        return Var_Type::WIRE;

    cerr<<str_var_type<<" has not been recognized as a type of variable."<<endl;
    exit(EXIT_FAILURE);
}
