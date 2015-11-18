#include "AnalyseVCD.h"

AnalyseVCD::AnalyseVCD(const string VCD_file_name, const string init_file_name, const string output_file_name)
{
    read_init_file(init_file_name);
    analyse_VCD_file(VCD_file_name, output_file_name);
}

AnalyseVCD::~AnalyseVCD()
{
    //dtor
}

int AnalyseVCD::Get_Var(size_t pos, Var& _var)
{
    if(_listeVar.size() > pos)
    {
        _var = _listeVar[pos];
        return 1;
    }

    return 0;
}

int AnalyseVCD::Add_Var(Var& _var)
{
    if(_listeVar.size() < _listeVar.max_size()) {
        _listeVar.push_back(_var);
        return 1;
    }
    return 0;
}

int AnalyseVCD::Add_Var(const string& str_var_type, int var_size, char var_identifier, const string& var_reference)
{
    if(_listeVar.size() < _listeVar.max_size()) {
        Var _var(str_var_type, var_size, var_identifier, var_reference);
        _listeVar.push_back(_var);
        return 1;
    }
    return 0;
}

int AnalyseVCD::Add_Var(Var_Type var_type, int var_size, char var_identifier, const string& var_reference)
{
    if(_listeVar.size() < _listeVar.max_size()) {
        Var _var(var_type, var_size, var_identifier, var_reference);
        _listeVar.push_back(_var);
        return 1;
    }
    return 0;
}

void AnalyseVCD::read_init_file(const string& init_file_name)
{

}

void AnalyseVCD::analyse_VCD_file(const string& VCD_file_name, const string& output_file_name)
{
    ifstream vcd_file(VCD_file_name);
    if(!vcd_file.is_open())
    {
        cerr<<"Warning ! AnalyseVCD::analyse_VCD_file ; File not found : "<< VCD_file_name<<endl;
        exit(EXIT_FAILURE);
    }

    ofstream output_file(output_file_name);
    if(!output_file.is_open())
    {
        cerr<<"Warning ! AnalyseVCD::analyse_VCD_file ; File can't be written : "<< output_file_name<<endl;
        vcd_file.close();
        exit(EXIT_FAILURE);
    }

    if(!get_VCD_header(vcd_file))
    {
        cerr<<"Warning ! AnalyseVCD::analyse_VCD_file ; No header "<<VCD_ENDDEFINITION_SYMBOLE<<" detected in file : "<< VCD_file_name<<endl;
        vcd_file.close();
        output_file.close();
        exit(EXIT_FAILURE);
    }



    vcd_file.close();
    output_file.close();
}

int AnalyseVCD::get_VCD_header(ifstream& _vcd_file)
{
    string buf;

    while(!_vcd_file.eof())
    {
        _vcd_file >> buf;
        if(!buf.compare(VCD_VAR_SYMBOLE))
        {
            string var_type;
            _vcd_file >> var_type;

            size_t var_size;
            _vcd_file >> buf;
            // var_size = stoi(buf, NULL, VCD_NUMBER_BASE);
            var_size = atoi(buf.c_str());

            char var_identifier;
            _vcd_file >> buf;
            var_identifier = buf.c_str()[0];

            string var_reference;
            _vcd_file >> var_reference;

            Add_Var(var_type, var_size, var_identifier, var_reference);
        }
        else if(!buf.compare(VCD_ENDDEFINITION_SYMBOLE))
        {
            Setenddefinition(true);
        }
        else if(!buf.compare(VCD_TIMESCALE_SYMBOLE))
        {
            float ts;
            _vcd_file >> buf;

            size_t pos;

            pos = buf.find(VCD_ps);
            if(pos != buf.npos)
            {
                string sub = buf.substr(0,pos-1);
                ts = atoi(sub.c_str())*VCD_ps_value;
            }
            else
            {
                pos = buf.find(VCD_ns);
                if(pos != buf.npos)
                {
                    string sub = buf.substr(0,pos-1);
                    ts = atoi(sub.c_str())*VCD_ns_value;
                }
                else
                {
                    pos = buf.find(VCD_us);
                    if(pos != buf.npos)
                    {
                        string sub = buf.substr(0,pos-1);
                        ts = atoi(sub.c_str())*VCD_us_value;
                    }
                    else
                    {
                        pos = buf.find(VCD_ms);
                        if(pos != buf.npos)
                        {
                            string sub = buf.substr(0,pos-1);
                            ts = atoi(sub.c_str())*VCD_ms_value;
                        }
                        else
                        {
                            pos = buf.find(VCD_s);
                            if(pos != buf.npos)
                            {
                                string sub = buf.substr(0,pos-1);
                                ts = atoi(sub.c_str())*VCD_s_value;
                            }
                            else
                            {
                                cerr<<"No timestamp power found"<<endl;
                            }
                        }
                    }
                }
            }

            ts = atoi(buf.c_str());
            Settimescale(ts);
        }

        return 1;
    }

    return 0;
}
