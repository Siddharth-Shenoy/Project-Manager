#ifndef __INIMANAGER_H_INCLUDED__
#define __INIMANAGER_H_INCLUDED__

#include <string>
#include <fstream>
#include <vector>
#include "strinc.h"

class INIKeyData
{
    public:
        std::string keyname;
        std::string keyvalue;
};
class INI
{
    private:
        std::string mfilepath;
        std::fstream m_file;
        std::vector<INIKeyData> m_keydata;
        void LoadKeyData();
        bool FindKey(const std::string& key,int &ind);
        void WriteChanges();
        bool fileCreated;
    public:
        INI(const std::string &filepath);
        void SetKeyValue(const std::string &key,const std::string& value);
        
        void SetKeyInt(const std::string &key,int value);
        void SetKeyLong(const std::string &key,long value);
        void SetKeyFloat(const std::string &key,float value);
        void SetKeyDouble(const std::string &key,double value);
        bool getCreatedStatus();
        float ReadKeyFloat(const std::string &key);
        double ReadKeyDouble(const std::string &key);
        long ReadKeyLong(const std::string &key);
        int ReadKeyInt(const std::string &key);
        const char* ReadKeyCharArray(const std::string &key);
        std::string ReadKeyString(const std::string &key);
        ~INI();
};

void INI::LoadKeyData()
{
    std::string fileline;
    std::string *splits;
    int totalsplits;
    m_keydata.empty();
    m_file.seekg(0);
    m_file.seekp(0);
    while(!m_file.eof())
    {
        getline(m_file,fileline);
        splits = strfunc::strsplit(fileline,totalsplits,'=');
        if(totalsplits > 1) {
            INIKeyData newdata;
            newdata.keyname = splits[0];
            newdata.keyvalue = splits[1];
            m_keydata.push_back(newdata);
        }
    }
}
bool INI::FindKey(const std::string& key,int &ind)
{
    bool found = false;
    for(int i = 0; i < m_keydata.size(); i++)
    {
        if(m_keydata[i].keyname == key)
        {
            found = true;
            ind = i;
            break;
        }
        else 
        {
            ind = -1;
            found = false;
            continue;
        }
    }
    return found;
}
void INI::WriteChanges()
{
    m_file.close();
    m_file.open(mfilepath.c_str(),std::fstream::out | std::fstream::trunc);
    for(int i = 0; i < m_keydata.size(); i++) 
    {
        m_file << m_keydata[i].keyname << "=" << m_keydata[i].keyvalue << std::endl;
    }
    m_file.close();
    m_file.open(mfilepath,std::fstream::out | std::fstream::in);
}
bool INI::getCreatedStatus()
{
    return fileCreated;
}
INI::INI(const std::string &filepath)
{
    fileCreated = false;
    mfilepath = filepath;
    m_file.open(filepath,std::fstream::out | std::fstream::in);
    if(m_file.good())
    {
        LoadKeyData();
    }
    else
    {
        m_file.close();
        m_file.open(filepath,std::fstream::app);
        m_file.close();
        m_file.open(filepath,std::fstream::out | std::fstream::in);
        fileCreated = true;
    }
}
std::string INI::ReadKeyString(const std::string &key)
{
    int index;
    if(FindKey(key,index))
    {
        return m_keydata[index].keyvalue;
    }
    else
    {
        std::cerr << "Could not find the key named '" << key << "'" << std::endl;
    }
    return std::string("");
}
float INI::ReadKeyFloat(const std::string &key)
{
    int index;
    if(FindKey(key,index))
    {
        return std::atof(m_keydata[index].keyvalue.c_str());
    }
    else
    {
        std::cerr << "Could not find the key named '" << key << "'" << std::endl;
    }
    return 0.00000f;
}
double INI::ReadKeyDouble(const std::string &key)
{
    int index;
    if(FindKey(key,index))
    {
        return (double)std::atof(m_keydata[index].keyvalue.c_str());
    }
    else
    {
        std::cerr << "Could not find the key named '" << key << "'" << std::endl;
    }
    return 0.00000;
}
long INI::ReadKeyLong(const std::string &key)
{
    int index;
    if(FindKey(key,index))
    {
        return std::atol(m_keydata[index].keyvalue.c_str());
    }
    else
    {
        std::cerr << "Could not find the key named '" << key << "'" << std::endl;
    }
    return 0L;
}
int INI::ReadKeyInt(const std::string &key)
{
    int index;
    if(FindKey(key,index))
    {
        return std::atoi(m_keydata[index].keyvalue.c_str());
    }
    else
    {
        std::cerr << "Could not find the key named '" << key << "'" << std::endl;
    }
    return 0;
}
const char* INI::ReadKeyCharArray(const std::string &key)
{
    int index;
    if(FindKey(key,index))
    {
        return m_keydata[index].keyvalue.c_str();
    }
    else
    {
        std::cerr << "Could not find the key named '" << key << "'" << std::endl;
    }
    return "";
}
void INI::SetKeyInt(const std::string &key,int value)
{
    SetKeyValue(key,std::to_string(value));
}
void INI::SetKeyLong(const std::string &key,long value)
{
    SetKeyValue(key,std::to_string(value));
}
void INI::SetKeyFloat(const std::string &key,float value)
{
    SetKeyValue(key,std::to_string(value));
}
void INI::SetKeyDouble(const std::string &key,double value)
{
    SetKeyValue(key,std::to_string(value));
}
void INI::SetKeyValue(const std::string &key,const std::string& value)
{
    int index;
    if(FindKey(key,index))
    {
        m_keydata[index].keyvalue = value;
    }
    else
    {
        INIKeyData newkey;
        newkey.keyname = key;
        newkey.keyvalue = value;
        m_keydata.push_back(newkey);
    }
    WriteChanges();
}
INI::~INI()
{
    m_file.close();
}
#endif