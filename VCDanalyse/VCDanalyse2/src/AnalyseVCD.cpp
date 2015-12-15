#include "../include/AnalyseVCD.h"

// Unshifted characters
char unshifted[][3] = { //col 0 = ScanCode, col 1 = QWERTY layout, col 2 = AZERTY layout
{0x0d,'\t', '\t'},
{0x0e,'~', '²'},
{0x15,'q', 'a'},
{0x16,'1', '&'},
{0x1a,'z', 'w'},
{0x1b,'s', 's'},
{0x1c,'a', 'q'},
{0x1d,'w', 'z'},
{0x1e,'2', 'é'},
{0x21,'c', 'c'},
{0x22,'x', 'x'},
{0x23,'d', 'd'},
{0x24,'e', 'e'},
{0x25,'4', '\''},
{0x26,'3', '"'},
{0x29,' ', ' '},
{0x2a,'v', 'v'},
{0x2b,'f', 'f'},
{0x2c,'t', 't'},
{0x2d,'r', 'r'},
{0x2e,'5', '('},
{0x31,'n', 'n'},
{0x32,'b', 'b'},
{0x33,'h', 'h'},
{0x34,'g', 'g'},
{0x35,'y', 'y'},
{0x36,'6', '-'},
{0x39,',', ','}, //strange,not sure
{0x3a,'m', ','},
{0x3b,'j', 'j'},
{0x3c,'u', 'u'},
{0x3d,'7', 'è'},
{0x3e,'8', '_'},
{0x41,'<', ';'},
{0x42,'k', 'k'},
{0x43,'i', 'i'},
{0x44,'o', 'o'},
{0x45,'0', 'à'},
{0x46,'9', 'ç'},
{0x49,'>', ':'},
{0x4a,'?', '!'},
{0x4b,'l', 'l'},
{0x4c,':', 'm'},
{0x4d,'p', 'p'},
{0x4e,'-', ')'},
{0x52,'"', 'ù'},
{0x54,'[', '^'},
{0x55,'+', '='},
{0x5a,'\n', '\n'}, //enter key
{0x5b,'}', '$'},
{0x5d,'|', '*'},
{0x61,'<', '<'},
{0x66,8, 8}, //backspace key
{0x69,'1', '1'},
{0x6b,'4', '4'},
{0x6c,'7', '7'},
{0x70,'0', '0'},
{0x71,',', '.'},
{0x72,'2', '2'},
{0x73,'5', '5'},
{0x74,'6', '6'},
{0x75,'8', '8'},
{0x79,'+', '+'},
{0x7a,'3', '3'},
{0x7b,'-', '-'},
{0x7c,'*', '*'},
{0x7d,'9', '9'},
{0,0} //needed to stop the research
};

// Shifted characters
char shifted[][3] = { //col 0 = ScanCode, col 1 = QWERTY layout, col 2 = AZERTY layout
{0x0d, '\t', '\t'},
{0x0e,'\'', '²'},
{0x15,'Q', 'A'},
{0x16,'!', '1'},
{0x1a,'Z', 'W'},
{0x1b,'S', 'S'},
{0x1c,'A', 'Q'},
{0x1d,'W', 'Z'},
{0x1e,'@', '2'},
{0x21,'C', 'C'},
{0x22,'X', 'X'},
{0x23,'D', 'D'},
{0x24,'E', 'E'},
{0x25,'$', '4'},
{0x26,'#', '3'},
{0x29,' ', ' '},
{0x2a,'V', 'V'},
{0x2b,'F', 'F'},
{0x2c,'T', 'T'},
{0x2d,'R', 'R'},
{0x2e,'%', '5'},
{0x31,'N', 'N'},
{0x32,'B', 'B'},
{0x33,'H', 'H'},
{0x34,'G', 'G'},
{0x35,'Y', 'Y'},
{0x36,'^', '6'},
{0x39,'L', 'L'},
{0x3a,'M', '?'},
{0x3b,'J', 'J'},
{0x3c,'U', 'U'},
{0x3d,'&', '7'},
{0x3e,'*', '8'},
{0x41,'<', '.'},
{0x42,'K', 'K'},
{0x43,'I', 'I'},
{0x44,'O', 'O'},
{0x45,')', '0'},
{0x46,'(', '9'},
{0x49,'>', '/'},
{0x4a,'/', '§'},
{0x4b,'L', 'L'},
{0x4c,';', 'M'},
{0x4d,'P', 'P'},
{0x4e,'_', '°'},
{0x52,'\'', 'µ'},
{0x54,'{', '¨'},
{0x55,'=', '+'},
{0x5a,13, 13}, //enter key
{0x5b,']', '£'},
{0x5d,'\\', 'µ'},
{0x61,'>', '>'},
{0x66,8, 8}, //backspace key
{0x69,'1', '1'},
{0x6b,'4', '4'},
{0x6c,'7', '7'},
{0x70,'0', '0'},
{0x71,',', '.'},
{0x72,'2', '2'},
{0x73,'5', '5'},
{0x74,'6', '6'},
{0x75,'8', '8'},
{0x79,'+', '+'},
{0x7a,'3', '3'},
{0x7b,'-', '-'},
{0x7c,'*', '*'},
{0x7d,'9', '9'},
{0,0} //needed to stop research
};

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
	char buf[256];
    int sizeString = GetPrivateProfileString("clock", "id", "", buf, 8, init_file_name.c_str());
    if(sizeString == 0)
    {
        cerr<<"No clock found in init file : "<<init_file_name<<endl;
        cout<<"End of program. Enter any key to kill it.";
        cin>>buf;
        exit(EXIT_FAILURE);
    }
    clock_id = buf[0];

    for(int i = 0; i<7; i++)
    {
        string buf_ind = "id";
        char ind = '0' + i;
        buf_ind.append(1, ind);
        sizeString = GetPrivateProfileString("data", buf_ind.c_str(), "", buf, 8, init_file_name.c_str());
        cout<<buf_ind<<" "<<sizeString<<endl;
        if(sizeString > 0)
            data_ids.push_back(buf[0]);
    }

   // sizeString = GetPrivateProfileString("data", buf_ind.c_str(), "", buf, 256, init_file_name.c_str());
   // vcd_file_name = buf;

    /*clock_id = '$';
    data_ids.push_back('%');
    data_ids.push_back('&');
    */
}

void AnalyseVCD::read_VCD_file(const string& VCD_file_name)
{
    ifstream vcd_file(VCD_file_name);
    if(!vcd_file.is_open())
    {
        cerr<<"Warning ! AnalyseVCD::read_VCD_file ; File not found : "<< VCD_file_name<<endl;
        cout<<"End of program. Enter any key to kill it.";
        string buf;
        cin>>buf;
        exit(EXIT_FAILURE);
    }

    if(!get_VCD_header(vcd_file))
    {
        cerr<<"Warning ! AnalyseVCD::read_VCD_file ; No header """<<VCD_ENDDEFINITION_SYMBOLE<<""" detected in file : "<< VCD_file_name<<endl;
        vcd_file.close();
        cout<<"End of program. Enter any key to kill it.";
        string buf;
        cin>>buf;
        exit(EXIT_FAILURE);
    }

    if(!get_var_values(vcd_file))
    {
        cerr<<"Warning ! AnalyseVCD::read_VCD_file ; No """<<VCD_DUMPVARS_SYMBOLE<<""" or no """<<VCD_END_SYMBOLE<<""" symbole detected in file : "<< VCD_file_name<<endl;
        vcd_file.close();
        cout<<"End of program. Enter any key to kill it.";
        string buf;
        cin>>buf;
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
        cerr<<"Warning ! AnalyseVCD::analyse_VCD_data - File can't be written : "<< output_file_name<<endl;
        cout<<"End of program. Enter any key to kill it.";
        string buf;
        cin>>buf;
        exit(EXIT_FAILURE);
    }

    vector<vector<Var::Sample>> data_on_clock_stream; //data[0] = clock and data[i] are the data variables
    vector<size_t> var_pos;
    if(!getDataOnClock(data_on_clock_stream, false, var_pos))
    {
        cout<<"End of program. Enter any key to kill it.";
        string buf;
        cin>>buf;
        exit(EXIT_FAILURE);
    }

    vector<vector<char>> data_from_protocole;
    getDataInCommunicationProtocole(data_on_clock_stream, B1_Low, 8, 1, B1_High, data_from_protocole);

    vector<vector<char>> data_ascii;
    getAsciiFromScanCode(data_from_protocole, data_ascii, true, AZERTY);

    size_t pos;
    string buf;

    //write data in output file
    FindVarByIdentifier(clock_id, pos);

    buf.append("Clock is ");
    buf.append(_listeVar[pos].Getvar_reference());
    buf.append("\n\r");
    output_file.write(buf.c_str(), buf.size());

    for(size_t i=0; i<data_ascii.size(); i++)
    {
        FindVarByIdentifier(data_ids[i], pos);
        buf.clear();
        buf.append("Var ");
        buf.append(_listeVar[pos].Getvar_reference());
        buf.append(" flow is :\n\r");
        output_file.write(buf.c_str(), buf.size());
        cout<<buf;
        for(size_t j=0; j<data_ascii[i].size(); j++){
            output_file.put(data_ascii[i][j]);
            cout<<data_ascii[i][j];
        }

        if((i+1) <data_ascii.size())
            output_file.write("\n\n\n", 3);
            cout<<endl<<endl<<endl;
    }

    cout<<"VCD data analyzed succesfully !"<<endl;
    output_file.close();
}

int AnalyseVCD::get_VCD_header(ifstream& _vcd_file)
{
    string buf;

    while(!_vcd_file.eof())
    {
        // reading file and compare it with symboles to decrypt it
        _vcd_file >> buf;

        if(!buf.compare(VCD_VAR_SYMBOLE))
        { // new variable definition
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
        { // end of the header detected
            Setenddefinition(true);
            return 1;
        }
        else if(!buf.compare(VCD_TIMESCALE_SYMBOLE))
        { // timscale definition
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

    return 0;
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

bool AnalyseVCD::getDataOnClock(vector<vector<Var::Sample>>& data_stream, const bool onRisingEdge, vector<size_t>& pos)
{
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

bool AnalyseVCD::getDataInCommunicationProtocole(vector<vector<Var::Sample>>& data_on_clock_stream, BitStartStopType bit_start_type, size_t data_length, size_t parity_length, BitStartStopType bit_stop_type, vector<vector<char>>& data_from_protocole)
{
    Var::Logic_Value datum = Var::Low;
    bool isbitStartDone = false, isbitParityDone = false, isbitStopDone = false, areDataDone = false;
    size_t data_size_counter = 0;
    char buffer = 0;

    data_from_protocole.resize(data_on_clock_stream.size()-1);

    for(size_t id_stream = 1; id_stream<data_on_clock_stream.size(); id_stream++) //id_stream = 0 -> clock -> don't care here...
    {
        vector<Var::Sample>& var_data_stream = data_on_clock_stream[id_stream];

        for(size_t id_sample=0; id_sample<var_data_stream.size(); id_sample++)
        {
            if(isbitStopDone)
            {
                isbitStartDone = false;
                areDataDone = false;
                isbitStopDone = false;
                isbitParityDone = false;
                data_size_counter = 0;
                buffer = 0;
            }

            datum = var_data_stream[id_sample].value[0]; //several bit length var is not supported

            if(isbitStartDone == false)
            {
                switch(bit_start_type)
                {
                    case B1_High :
                        if(datum == Var::High)
                            isbitStartDone = true;
                        break;
                    case B2_High :
                        cerr<<"AnalyseVCD::getDataInCommunicationProtocole : 2 bits of start are not supported for now !"<<endl;
                        exit(EXIT_FAILURE);
                        break;
                    case B1_Low :
                        if(datum == Var::Low)
                            isbitStartDone = true;
                        break;
                    case B2_Low :
                        cerr<<"AnalyseVCD::getDataInCommunicationProtocole : 2 bits of start are not supported for now !"<<endl;
                        exit(EXIT_FAILURE);
                        break;
                }
                continue;
            }

            if(areDataDone == false)
            {
                buffer |= (_listeVar[0].LogicValue2int(var_data_stream[id_sample].value[0])<<(data_length - 1 - data_size_counter));
                // we use _listeVar to have access to the Var::method, we don't cara about which Var is used.
                // "data_length - 1 - data_size_counter" and not just "data_size_counter" because first bit is considered as LSB

                data_size_counter ++;

                if(data_size_counter == data_length)
                    areDataDone = true;

                continue;
            }

            if(isbitParityDone == false)
            {
                isbitParityDone = true;
                continue;
            }

            if(isbitStopDone == false)
            {
                switch(bit_stop_type)
                {
                    case B1_High :
                        if(datum == Var::High)
                            isbitStopDone = true;
                        break;
                    case B2_High :
                        cerr<<"AnalyseVCD::getDataInCommunicationProtocole : 2 bits of stop are not supported for now !"<<endl;
                        exit(EXIT_FAILURE);
                        break;
                    case B1_Low :
                        if(datum == Var::Low)
                            isbitStopDone = true;
                        break;
                    case B2_Low :
                        cerr<<"AnalyseVCD::getDataInCommunicationProtocole : 2 bits of stop are not supported for now !"<<endl;
                        exit(EXIT_FAILURE);
                        break;
                }

                data_from_protocole[id_stream-1].push_back(buffer);
                continue;
            }
        }
    }
    return true;
}

char AnalyseVCD::convertScanToAsciiCode(char ScanCode, bool isShift, KEYBOARD_LAYOUT key_lay, bool isVerNum)
{
    size_t i = 0;

    while(unshifted[i][0] != 0 && unshifted[i][0] != ScanCode)
    {
        i++;
    }

    if(!isVerNum && 0x69 <= i && i <= 0x7D)
        return 0;

    if(isShift)
        return shifted[i][key_lay];
    else
        return unshifted[i][key_lay];
}

void AnalyseVCD::getAsciiFromScanCode(vector<vector<char>>& data_from_protocole, vector<vector<char>>& data_ascii, bool isInverted, KEYBOARD_LAYOUT key_lay)
{
    data_ascii.resize(data_from_protocole.size());
    bool isShift = false, isReleased = false, isCapitalLetter = false, isVerNum = false;
    char buf;

    for(size_t id_stream = 0; id_stream < data_from_protocole.size(); id_stream++)
    {
        vector<char>& stream = data_from_protocole[id_stream];

        for(size_t id_data = 0; id_data < stream.size(); id_data++)
        {
            if(isInverted)
                buf = inverseByte(stream[id_data]);
            else
                buf = stream[id_data];

            if(isShift == false && isShiftScanCode(buf))
            {
                isShift = true;
                continue;
            }
            if(isReleased == false && isKeyReleased(buf))
            {
                isReleased = true;
                continue;
            }
            if(isReleased)
            {
                if(isShift == true && isShiftScanCode(buf))
                    isShift = false;

                isReleased = false;
                continue;
            }
            if(isReleased == false && isCapsScanCode(buf))
            {
                isCapitalLetter = !isCapitalLetter;
                continue;
            }
            if(isReleased == false && isVerNumScanCode(buf))
            {
                isVerNum = !isVerNum;
                continue;
            }

            data_ascii[id_stream].push_back(convertScanToAsciiCode(buf, (isShift || isCapitalLetter), key_lay, isVerNum));
        }
    }
}

bool AnalyseVCD::isShiftScanCode(unsigned char scanCode)
{
    if(scanCode == 0x12 || scanCode == 0x59)
        return true;
    return false;
}

bool AnalyseVCD::isKeyReleased(unsigned char scanCode)
{
    if(scanCode == 240)
        return true;
    return false;
}

char AnalyseVCD::inverseByte(const char byteIn)
{
    char byteOut = 0;

    for(int i = 0; i<8; i++)
    {
        if((byteIn>>i)&0x01)
        {
            byteOut |= 1 << (7-i);
        }
    }
    return byteOut;
}

bool AnalyseVCD::isCapsScanCode(unsigned char scanCode)
{
    if(scanCode == 0x58)
        return true;
    return false;
}

bool AnalyseVCD::isVerNumScanCode(unsigned char scanCode)
{
    if(scanCode == 0x77)
        return true;
    return false;
}

