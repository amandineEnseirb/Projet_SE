#include "AnalyseVCD.h"

AnalyseVCD::AnalyseVCD(const string VCD_file_name, const string init_file_name, const string output_file_name)
{
    read_init_file(init_file_name);
    read_VCD_file(VCD_file_name);
    analyse_VCD_data(output_file_name);
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

int AnalyseVCD::Add_Var(const string& str_var_type, const size_t var_size, const char var_identifier, const string& var_reference)
{
    if(_listeVar.size() < _listeVar.max_size()) {
        Var _var(str_var_type, var_size, var_identifier, var_reference);
        _listeVar.push_back(_var);
        return 1;
    }
    return 0;
}

int AnalyseVCD::Add_Var(const Var::Var_Types var_type, const size_t var_size, const char var_identifier, const string& var_reference)
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
    /* initialise les clock_names et data_names pour pouvoir analyser les signaux*/
        clock_id = '$';

        data_ids.push_back('%');
        data_ids.push_back('&');

}

void AnalyseVCD::read_VCD_file(const string& VCD_file_name)
{
    ifstream vcd_file(VCD_file_name);
    if(!vcd_file.is_open())
    {
        cerr<<"Warning ! AnalyseVCD::read_VCD_file ; File not found : "<< VCD_file_name<<endl;
        exit(EXIT_FAILURE);
    }

    if(!get_VCD_header(vcd_file))
    {
        cerr<<"Warning ! AnalyseVCD::read_VCD_file ; No header """<<VCD_ENDDEFINITION_SYMBOLE<<""" detected in file : "<< VCD_file_name<<endl;
        vcd_file.close();
        exit(EXIT_FAILURE);
    }

    if(!get_var_values(vcd_file))
    {
        cerr<<"Warning ! AnalyseVCD::read_VCD_file ; No """<<VCD_DUMPVARS_SYMBOLE<<""" or no """<<VCD_END_SYMBOLE<<""" symbole detected in file : "<< VCD_file_name<<endl;
        vcd_file.close();
        exit(EXIT_FAILURE);
    }
    vcd_file.close();
    cout<<VCD_file_name<<" read succesfully !"<<endl;
}

void AnalyseVCD::analyse_VCD_data(const string& output_file_name)
{
    fstream output_file(output_file_name, ios_base::out);
    if(!output_file.is_open())
    {
        cerr<<"Warning ! AnalyseVCD::analyse_VCD_data ; File can't be written : "<< output_file_name<<endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<Var::Sample>> data_stream; //data[0] = clock and data[i] are the data variables
    getDataOnClock(data_stream, false);

    cout<<"Clock is "<<clock_id<<endl;
    for(size_t i=0; i<data_stream[0].size(); i++)
    {
        cout<<"Timestamp = "<<data_stream[0][i].timestamp<<":"<<endl;
        for(size_t j=0; j<data_ids.size(); j++)
        {
            cout<<'\t'<<"Var "<<data_ids[j]<<" = "<< _listeVar[0].LogicValue2char(data_stream[j+1][i].value[0])<<endl;
        }
    }
    Var::Logic_Value lv8[SIZE_BYTE], lv8_[SIZE_BYTE];
    int byte;

    for(size_t i=0; i<SIZE_BYTE; i++)
    {
        lv8[SIZE_BYTE-1-i] = data_stream[1][i].value[0];
        cout<<"lv1 = "<<_listeVar[0].LogicValue2int(lv8[SIZE_BYTE-1-i])<<endl;
    }

    byte = _listeVar[0].LogicByte2int(lv8);
    cout<<endl<<"car 1 :"<<(char)byte<<endl;

    output_file.put((char)byte);
    for(size_t i=0; i<SIZE_BYTE; i++)
    {
        lv8_[SIZE_BYTE-1-i] = data_stream[2][i].value[0];
        cout<<"lv2 = "<<_listeVar[2].LogicValue2int(lv8_[SIZE_BYTE-1-i])<<endl;
    }

    byte = _listeVar[0].LogicByte2int(lv8_);
    cout<<endl<<"car 2 :"<<(char)byte<<endl;

    output_file.put((char)byte);

    cout<<"VCD data analyzed succesfully !"<<endl;
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
            return 1;
        }
        else if(!buf.compare(VCD_TIMESCALE_SYMBOLE))
        {
            int ts;
            string ts_unit;
            _vcd_file >> buf;

            size_t pos;

            pos = buf.find(VCD_ps);
            if(pos != buf.npos)
            {
                string sub = buf.substr(0,pos);
                ts = atoi(sub.c_str());
                ts_unit = VCD_ps;
            }
            else
            {
                pos = buf.find(VCD_ns);
                if(pos != buf.npos)
                {
                    string sub = buf.substr(0,pos);
                    ts = atoi(sub.c_str());
                    ts_unit = VCD_ns;
                }
                else
                {
                    pos = buf.find(VCD_us);
                    if(pos != buf.npos)
                    {
                        string sub = buf.substr(0,pos);
                        ts = atoi(sub.c_str());
                        ts_unit = VCD_us;
                    }
                    else
                    {
                        pos = buf.find(VCD_ms);
                        if(pos != buf.npos)
                        {
                            string sub = buf.substr(0,pos);
                            ts = atoi(sub.c_str());
                            ts_unit = VCD_ms;
                        }
                        else
                        {
                            pos = buf.find(VCD_s);
                            if(pos != buf.npos)
                            {
                                string sub = buf.substr(0,pos);
                                ts = atoi(sub.c_str());
                                ts_unit = VCD_s;
                            }
                            else
                            {
                                ts = 0;
                                ts_unit = VCD_s;
                                cerr<<"Warning : No timestamp unit found."<<endl;
                            }
                        }
                    }
                }
            }
            Settimescale(ts);
            Settimescale_unit(ts_unit);
        }
    }

    return 1;
}

void AnalyseVCD::show_listeVar()
{
    size_t length = Get_listeVar_size();
    size_t pos = 0;

    Var _var;

    for(;pos<length; pos++)
    {
        Get_Var(pos, _var);
        _var.display_var();
    }

    cout<<"Timescale is of "<<Gettimescale()<<" "<<Gettimescale_unit()<<"."<<endl;
}

int AnalyseVCD::get_var_values(ifstream& _vcd_file)
{
    string buf;
    int ts;

    //find dumpvar to initialize variables
    while(1)
    {
        if(_vcd_file.eof())
        {
            return 0;
        }

        getline(_vcd_file, buf);

        if(!buf.compare(VCD_DUMPVARS_SYMBOLE))
        {
            break;
        }
    }

    //DUMPVARS analyse
    ts = -1;
    while(1)
    {
        if(_vcd_file.eof())
        {
            return 0;
        }

        getline(_vcd_file, buf);

        if(!buf.compare(VCD_END_SYMBOLE))
        {
            break;
        }

        AddSampleToVar(buf, ts);
    }

    // get all timestamps
    while(1)
    {
        if(_vcd_file.eof())
        {
            break;
        }
        getline(_vcd_file, buf);
        if(buf.length()==0)
            continue;

        char fr = (buf.c_str())[0];

        if(fr == VCD_TIMESTAMP_SYMBOLE)
        {
            string sub = buf.substr(sizeof(VCD_TIMESTAMP_SYMBOLE), buf.length()-1);
            int _ts = atoi(sub.c_str());
            if(_ts <= ts)
            {
                cerr<<"Warning : new timestamp is "<<_ts<<" while previous is "<<ts<<"."<<endl;
                cerr<<"This can disturb VCD data analyze."<<endl;
            }
            ts = _ts;
            continue;
        }
        AddSampleToVar(buf, ts);
    }
    return 1;
}

void AnalyseVCD::display_allVar(const size_t pos, const Var::Display_Format df)
{
    for(size_t i = 0; i < Get_listeVar_size(); i++)
    {
        _listeVar[i].display_varSample(pos, df, timescale_unit);
    }
}

void AnalyseVCD::display_allVar(const int timestamp, const Var::Display_Format df)
{
    for(size_t i = 0; i < Get_listeVar_size(); i++)
    {
        _listeVar[i].display_varSample(timestamp, df, timescale_unit);
    }
}

bool AnalyseVCD::FindVarByIdentifier(const char id, size_t& pos)
{
    for(pos = 0; pos < Get_listeVar_size(); pos++)
            if(_listeVar[pos].Getvar_identifier() == id)
                return true;
    return false;
}

bool AnalyseVCD::AddSampleToVar(const string& str, const int timestamp)
{
        string buf = str;
        size_t pos;

        if(FindVarByIdentifier(buf.back(), pos))
        {
            buf.pop_back();
            _listeVar[pos].AddSample(buf, timestamp);
            return true;
        }

        cerr<<"Unknown variable identifier : "<<buf.back()<<endl;
        return false;
}

void AnalyseVCD::display_RisingEdges()
{
    for(size_t i = 0; _listeVar.size()>i; i++)
    {
        vector<vector<int>> vect_time;
        _listeVar[i].findRisingEdge(vect_time);

        cout<<"For var "<<_listeVar[i].Getvar_identifier()<<" there are rising edges at timestamps:"<<endl;
        for(size_t j = 0; j<vect_time.size(); j++)
        {
            cout<<'\t'<<"bit "<<j<<" :";
            for(size_t k = 0; k<vect_time[j].size(); k++)
            {
                cout<<" "<<vect_time[j][k];
            }
            cout<<endl;
        }
    }
}

void AnalyseVCD::display_FallingEdges()
{
    for(size_t i = 0; _listeVar.size()>i; i++)
    {
        vector<vector<int>> vect_time;
        _listeVar[i].findFallingEdge(vect_time);

        cout<<"For var "<<_listeVar[i].Getvar_identifier()<<" there are falling edges at timestamps:"<<endl;
        for(size_t j = 0; j<vect_time.size(); j++)
        {
            cout<<'\t'<<"bit "<<j<<" :";
            for(size_t k = 0; k<vect_time[j].size(); k++)
            {
                cout<<" "<<vect_time[j][k];
            }
            cout<<endl;
        }
    }
}

bool AnalyseVCD::getDataOnClock(vector<vector<Var::Sample>>& data_stream, const bool onRisingEdge)
{
    vector<size_t> pos;
    pos.resize(data_ids.size()+1);//size = clock + each data variable

    if(!FindVarByIdentifier(clock_id, pos[0]))
    {
        cerr<<"Clock Identifier has not been recognized : "<<clock_id<<endl;
        return false;
    }

    for(size_t i = 0; i<data_ids.size(); i++)
    {
        if(!FindVarByIdentifier(data_ids[i], pos[i+1]))
        {
            cerr<<"Var Identifier has not been recognized : "<<data_ids[i]<<endl;
            return false;
        }
    }

    data_stream.resize(data_ids.size()+1); //size = clock + each data variable

    vector<vector<int>> vect_time;
    if(onRisingEdge)
       _listeVar[pos[0]].findRisingEdge(vect_time);
    else
       _listeVar[pos[0]].findFallingEdge(vect_time);

    Var::Sample samp;
    for(size_t i = 0; i<vect_time[0].size(); i++)
    {
        for(size_t j = 0; j<pos.size(); j++)
        {
            _listeVar[pos[j]].GetSample(vect_time[0][i], samp);
            data_stream[j].push_back(samp);
        }
    }
    return true;
}

