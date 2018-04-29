#ifndef __STRINC_H_INCLUDED__
#define __STRINC_H_INCLUDED__

#include <string>

class strfunc
{
    public:
        static std::string* strsplit(std::string& str,int &total,const char delimiter);
};
std::string* strfunc::strsplit(std::string& str,int &total,const char delimiter)
{
    std::string *splits;
    int _total = 0;
    int i, j,last = 0;
    for(i = 0; i < str.length(); i++)
    {
        if(str[i] == delimiter)
        {
            _total++;
            continue;
        }
    }
    _total++;
    total = _total;
    splits = new std::string[_total];
    j = 0;
    for(i = 0; i < str.length(); i++)
    {
        if(str[i] == delimiter)
        {
            splits[j] = str.substr(last,last + i); 
            j++;
            last = i+1;
            continue;
        }
    }
    splits[j] = str.substr(last,str.length()); 
    return splits;

}

#endif