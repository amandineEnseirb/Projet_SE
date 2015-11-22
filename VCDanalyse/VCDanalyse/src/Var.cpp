#include "Var.h"

Var::Var(Var_Types var_type, int var_size, char var_identifier, const string& var_reference)
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

Var::Var_Types Var::detect_var_type(const string& str_var_type)
{
    if(!str_var_type.compare("EVENT") || !str_var_type.compare("event") || !str_var_type.compare("Event"))
        return Var_Types::EVENT;

    if(!str_var_type.compare("INTEGER") || !str_var_type.compare("integer") || !str_var_type.compare("Integer"))
        return Var_Types::INTEGER;

    if(!str_var_type.compare("WIRE") || !str_var_type.compare("wire") || !str_var_type.compare("Wire"))
        return Var_Types::WIRE;

    cerr<<str_var_type<<" has not been recognized as a type of variable."<<endl;
    exit(EXIT_FAILURE);
}

void Var::display_var()
{
    cout<<"Var "<<Getvar_reference()<<", identified by "<<Getvar_identifier()<<" is "<<Getvar_size()<<" length and is of type "<<var_type2string(Getvar_type())<<"."<<endl;
}

void Var::display_varSample(const size_t pos, const Display_Format df, const string timestamp_unit)
{
    cout<<"Var "<<Getvar_reference()<<", id "<<Getvar_identifier()<<", the sample at pos "<<pos<<" is :"<<endl;
    cout<<'\t'<<'\t'<<"timestamp = "<<var_values[pos].timestamp<<" "<<timestamp_unit<<endl;
    cout<<'\t'<<'\t'<<"value = "<<LogicValue2string(var_values[pos].value, df)<<endl;
}

void Var::display_varSample(const int timestamp, const Display_Format df, const string timestamp_unit)
{
    display_varSample(findTimestampPos(timestamp), df, timestamp_unit);
}

string Var::LogicValue2string(const vector<Logic_Value>& _lv, const Display_Format df)
{
    string buf;

    if(_lv.size()>1)
        switch(df)
        {
            case BINARY :  buf = VCD_VALUE_BINARY; break;
            case DECIMAL : buf = VCD_VALUE_DECIMAL; break;
            case HEXADECIMAL : buf = VCD_VALUE_HEXADECIMAL; break;
            default :  buf = VCD_VALUE_BINARY; break;
        }

    switch(df)
        {
            case BINARY :
                for(size_t i = 0; _lv.size()>i; i++)
                {
                    buf = buf + LogicValue2char(_lv[i]);
                }
                break;
            case DECIMAL :
                for(size_t i = 0; _lv.size()>i; i++)
                {
                    buf = buf + LogicValue2dec(_lv[i]);
                }
                break;
            case HEXADECIMAL :
                for(size_t i = 0; _lv.size()>i; i++)
                {
                    buf = buf + LogicValue2hex(_lv[i]);
                }
                break;
            default :
                for(size_t i = 0; _lv.size()>i; i++)
                {
                    buf = buf + LogicValue2char(_lv[i]);
                }
                break;
        }

    return buf;
}

char Var::LogicValue2char(const Logic_Value lv)
{
    switch(lv)
    {
        case High : return VCD_VALUE_HIGH; break;
        case Low : return VCD_VALUE_LOW ; break;
        case X : return VCD_VALUE_UNKNOW; break;
        case Z : return VCD_VALUE_HIGHIMPEDANCE; break;
        default : return VCD_VALUE_UNKNOW; break;
    }
}

int Var::LogicValue2int(const Logic_Value lv)
{
    switch(lv)
    {
        case High : return VCD_VALUE_HIGH_BIN; break;
        case Low : return VCD_VALUE_LOW_BIN ; break;
        case X : return VCD_VALUE_UNKNOW_BIN; break;
        case Z : return VCD_VALUE_HIGHIMPEDANCE_BIN; break;
        default : return VCD_VALUE_UNKNOW_BIN; break;
    }
}

string Var::LogicValue2dec(const Logic_Value lv)
{
    /**********************************/
    return "?";
}

string Var::LogicValue2hex(const Logic_Value lv)
{
   /**********************************/
   return "?";
}

string Var::var_type2string(const Var_Types _vt)
{
    string _vt_str;

    switch(_vt)
    {
        case EVENT: _vt_str = "EVENT"; break;
        case INTEGER : _vt_str = "INTEGER"; break;
        case WIRE : _vt_str = "WIRE"; break;
        default : _vt_str = "OTHER"; break;
    }

    return _vt_str;
}

int Var::GetSample(const size_t pos, Sample& _sample)
{
    if(var_values.size() > pos)
    {
        _sample = var_values[pos];
        return 1;
    }

    return 0;
}

size_t Var::findTimestampPos(const int _timestamp)
{
    for(size_t i = 1; i<var_values.size(); i++)
    {
        if(var_values[i-1].timestamp <= _timestamp && var_values[i].timestamp > _timestamp)
            return i-1;
    }
    return var_values.size()-1;
}

void Var::findRisingEdge(vector<vector<int>>& vect_time)
{
    vect_time.resize(var_size);

    for(size_t k = 0; k<var_size; k++)
    {
        for(size_t i = 1; i<var_values.size(); i++)
        {
            if(var_values[i-1].value[k] == Low && var_values[i].value[k] == High )
                vect_time[k].push_back(var_values[i].timestamp);
        }
    }
}

void Var::findFallingEdge(vector<vector<int>>& vect_time)
{
    vect_time.resize(var_size);

    for(size_t k = 0; k<var_size; k++)
    {
        for(size_t i = 1; i<var_values.size(); i++)
        {
            if(var_values[i-1].value[k] == High && var_values[i].value[k] == Low )
                vect_time[k].push_back(var_values[i].timestamp);
        }
    }
}

void Var::AddSample(const string& str_val, const int timestamp)
{
    Sample s;
    s.timestamp = timestamp;
    vector<Logic_Value> value;

    char c = str_val.front();

    switch(c)
    {
        case VCD_VALUE_BINARY : convertBinary2LogicValue(str_val, s.value); break;
        case VCD_VALUE_DECIMAL : cerr<<"Decimal value detected."<<endl; break;
        default : convertBinary2LogicValue(str_val, s.value); break;
    }

    AddSample(s);
}

void Var::convertBinary2LogicValue(const string& str_val, vector<Logic_Value>& value)
{
    string buf = str_val;
    char c;
    size_t pos = var_size;
    Logic_Value lv;

    value.resize(var_size);
    while(pos>0)
    {
        if(buf.length() > 0)
        {
            c = buf.back();
            buf.pop_back();
            if(!isLogicalValue(c, lv))
            {
                continue;
            }
        }
        else
        {
            lv = Low;
        }
        value[pos-1] = lv;
        pos--;
    }
}

bool Var::isLogicalValue(const char c, Logic_Value& _lv)
{
    switch(c)
    {
        case VCD_VALUE_HIGH : _lv = High; return true; break;
        case VCD_VALUE_LOW : _lv = Low; return true; break;
        case VCD_VALUE_UNKNOW : _lv = X; return true; break;
        case VCD_VALUE_HIGHIMPEDANCE : _lv = Z; return true; break;
        default : _lv = X; return false; break;
    }
}

int Var::LogicByte2int(const Logic_Value lv8[SIZE_BYTE])
{
    int byte = 0;

    for(size_t pos = 0; pos<SIZE_BYTE; pos++)
    {
        cout<<"lv = "<<LogicValue2int(lv8[pos])<<endl;
        byte |= (LogicValue2int(lv8[pos])<<pos);
        cout<<"byte = "<<byte<<endl;
    }
    return byte;
}

