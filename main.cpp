#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <cstdio>
#include <sys/stat.h>
#include "strinc.h"
#include "inimanager.h"

#define     TYPE_CPP        0
#define     TYPE_C          1
#define     TYPE_JAVA       2

typedef struct index
{
    int value;
    int offset;
}
index;
typedef struct indexes
{
    index projectcreate { 0 , -1};
    index projecttype {-1 , -1};
    index projectauthor { -1 , -1};
}
indexes;

int main(int argc,char **argv)
{
    /** Iterators **/
    int i , j;

    int ptype = 0;
    DIR *pdir = NULL;
    pdir = opendir("./");
    indexes m_index;
    if(pdir == NULL)
    {
        std::cout << "Error while opening directory!" << std::endl;
        exit(0);
    }
    if(argc > 1)
    {
        /**
         * Types of arguments passed:
         * -c           -           Stands for creating a project folder in the current folder
         * -t           -           Specifies the type of project it is i.e. C++ , Java , C , C# etc..
         * -a           -           Specifies the author and appends it to the readme and other auto created files
         */
        m_index.projectcreate.value = 0;
        m_index.projectauthor.value = -1;
        m_index.projecttype.value = -1;
        for(i = 1; i < argc; i++)
        {
            
            if(std::string(argv[i]) == "-c")
            {
                m_index.projectcreate.value = 1;
                m_index.projectcreate.offset = i + 1;
                continue;
            }
            else if(std::string(argv[i]) == "-t")
            {
                m_index.projecttype.value = 1;
                m_index.projecttype.offset = i + 1;
                continue;
            }
            else if(std::string(argv[i]) == "-a")
            {
                m_index.projectauthor.value = 1;
                m_index.projectauthor.offset = i + 1;
                continue;
            }
            else if(std::string(argv[i]) == "-b") //for building project
            {
                m_index.projecttype.value = -1;
                INI file("./build.ini");
                std::string commandline = "";
                if(file.getCreatedStatus())
                {
                    std::string keyvalue;
                    std::cout << "[!] Build file 'build.ini' was created , hence you need to add some key values for that:" << std::endl;
                    do
                    {
                        std::cout << "Project Type (CPP,JAVA,C):" << std::endl;
                        std::cin >> keyvalue;
                        file.SetKeyValue("PROJECT_TYPE",keyvalue);
                    }
                    while(keyvalue != "CPP" && keyvalue != "JAVA" && keyvalue != "C");
                    std::cout << "Compiler:" << std::endl;
                    std::cin >> keyvalue;
                    file.SetKeyValue("COMPILER",keyvalue);
                    std::cout << "Main Source file (main.cpp, main.java , etc..):" << std::endl;
                    std::cin >> keyvalue;
                    file.SetKeyValue("MAIN_FILE",keyvalue);
                    std::cout << "Project Output File:" << std::endl;
                    std::cin >> keyvalue;
                    file.SetKeyValue("OUTPUT_FILE",keyvalue);
                    commandline += file.ReadKeyString("COMPILER") + " ";
                    commandline += file.ReadKeyString("MAIN_FILE") + " -o ";
                    commandline += file.ReadKeyString("OUTPUT_FILE");
                }
                else
                {
                    if(file.ReadKeyString("PROJECT_TYPE") == "CPP" || file.ReadKeyString("PROJECT_TYPE") == "C")
                    {
                        commandline += file.ReadKeyString("COMPILER") + " ";
                        commandline += file.ReadKeyString("MAIN_FILE") + " -o ";
                        commandline += file.ReadKeyString("OUTPUT_FILE");
                    }
                    else if(file.ReadKeyString("PROJECT_TYPE") == "JAVA")
                    {
                        commandline = file.ReadKeyString("COMPILER") + " ";
                        commandline += file.ReadKeyString("MAIN_FILE");
                    }
                }
                int val = system(commandline.c_str());
                if(!val)
                {
                    std::cout << "Build successful!" << std::endl;
                    std::cout << "Final Executable filename: " << file.ReadKeyString("OUTPUT_FILE") << std::endl;;
                }
                else
                {
                    std::cout << "There were build errors! " << std::endl;
                }
                exit(0);
            }
            else continue;
        }

        if(!m_index.projectcreate.value)
        {
            std::cout << "No parameters passed (Pass -c <project-name>) to create a project folder" << std::endl;
            exit(21);
        }
        std::string _projectdir(argv[m_index.projectcreate.offset]);
        std::string _projectdirpath = "./" + _projectdir;

        if(mkdir(_projectdirpath.c_str(),0775) != -1)
        {
            std::cout << "[!] Created the project folder named '" << _projectdir << "' " << std::endl;
            std::cout << "[!] This project consists a autogenerated 'readme.txt' file" << std::endl;
            std::fstream readmefile;
            readmefile.open((_projectdirpath + "/readme.txt").c_str(),std::fstream::app);
            readmefile << "Project name: " << _projectdir << std::endl;
            if(m_index.projecttype.value != -1)
            {
                if(std::string(argv[m_index.projecttype.offset]) == "CPP")
                {
                    ptype = TYPE_CPP;
                    readmefile << "Type: C++" << std::endl;
                }
                else if(std::string(argv[m_index.projecttype.offset]) == "C")
                {
                    ptype = TYPE_C;
                    readmefile << "Type: C" << std::endl;
                }
                else if(std::string(argv[m_index.projecttype.offset]) == "JAVA")
                {
                    ptype = TYPE_JAVA;
                    readmefile << "Type: Java" << std::endl;
                }
            }
            else readmefile << "Type: Not specified" << std::endl;
            if(m_index.projectauthor.value != -1) { 
                readmefile << "Author: " << argv[m_index.projectauthor.offset] << std::endl;
            }
            else{ 
                readmefile << "Author: Unknown" << std::endl;
            }

            if(m_index.projecttype.value != -1)
            {
                switch(ptype)
                {
                    case TYPE_CPP:
                    {
                        readmefile << "Main file: main.cpp" << std::endl;
                        break;
                    }
                    case TYPE_C:
                    {
                        readmefile << "Main file: main.c" << std::endl;
                        break;
                    }
                    case TYPE_JAVA:
                    {
                        readmefile << "Main file: main.java" << std::endl;
                        break;
                    }
                }

            }
            else readmefile << "Main file: Unspecified" << std::endl;
            readmefile.close();  
            
            if(m_index.projecttype.value != -1)
            {
                std::fstream projectmainfile;
                switch(ptype)
                {
                    case TYPE_CPP:
                    {
                        projectmainfile.open((_projectdirpath + "/main.cpp").c_str(),std::fstream::app);
                        if(m_index.projectauthor.value != -1)
                        {
                            projectmainfile << "/**" << std::endl;
                            projectmainfile << "*\tAuthor: " << argv[m_index.projectauthor.offset] << std::endl;
                            projectmainfile << "*\tProject Name: " << argv[m_index.projectcreate.offset] << std::endl;
                            projectmainfile << "**/" << std::endl;
                            projectmainfile << std::endl;
                        }

                        projectmainfile << "#include <iostream>" << std::endl;
                        projectmainfile << std::endl;
                        projectmainfile << "int main(int argc,char **argv)" << std::endl;
                        projectmainfile << "{" << std::endl;
                        projectmainfile << "\treturn 0;" << std::endl;
                        projectmainfile << "}" << std::endl;
                        projectmainfile.close();
                        std::cout << _projectdirpath + "/build.ini" << std::endl;
                        INI file(_projectdir + "/build.ini");
                        file.SetKeyValue("PROJECT_TYPE","CPP");
                        file.SetKeyValue("COMPILER","g++");
                        file.SetKeyValue("MAIN_FILE","main.cpp");
                        file.SetKeyValue("OUTPUT_FILE","main");
                        break;
                    }
                    case TYPE_C:
                    {
                        projectmainfile.open((_projectdirpath + "/main.c").c_str(),std::fstream::app);
                        if(m_index.projectauthor.value != -1)
                        {
                            projectmainfile << "/**" << std::endl;
                            projectmainfile << "*\tAuthor: " << argv[m_index.projectauthor.offset] << std::endl;
                            projectmainfile << "*\tProject Name: " << argv[m_index.projectcreate.offset] << std::endl;
                            projectmainfile << "**/" << std::endl;
                            projectmainfile << std::endl;
                        }

                        projectmainfile << "#include <stdio.h>" << std::endl;
                        projectmainfile << std::endl;
                        projectmainfile << "int main(int argc,char **argv)" << std::endl;
                        projectmainfile << "{" << std::endl;
                        projectmainfile << "\treturn 0;" << std::endl;
                        projectmainfile << "}" << std::endl;
                        projectmainfile.close();
                        INI file(_projectdir + "/build.ini");
                        file.SetKeyValue("PROJECT_TYPE","C");
                        file.SetKeyValue("COMPILER","gcc");
                        file.SetKeyValue("MAIN_FILE","main.c");
                        file.SetKeyValue("OUTPUT_FILE","main");

                        break;
                    }
                    case TYPE_JAVA:
                    {
                        projectmainfile.open((_projectdirpath + "/" + _projectdir + ".java").c_str(),std::fstream::app);
                        if(m_index.projectauthor.value != -1)
                        {
                            projectmainfile << "/**" << std::endl;
                            projectmainfile << "*\tAuthor: " << argv[m_index.projectauthor.offset] << std::endl;
                            projectmainfile << "*\tProject Name: " << argv[m_index.projectcreate.offset] << std::endl;
                            projectmainfile << "**/" << std::endl;
                            projectmainfile << std::endl;
                        }

                        projectmainfile << "class " << _projectdir << std::endl;
                        projectmainfile << "{" << std::endl;
                        projectmainfile << "\tpublic static void main(String args[])" << std::endl;
                        projectmainfile << "\t{" << std::endl;
                        projectmainfile << "\t" << std::endl;
                        projectmainfile << "\t}" << std::endl;
                        projectmainfile << "}" << std::endl;
                        projectmainfile.close();
                        INI file(_projectdir + "/build.ini");
                        file.SetKeyValue("PROJECT_TYPE","JAVA");
                        file.SetKeyValue("COMPILER","javac");
                        file.SetKeyValue("MAIN_FILE",_projectdir + ".java");
                        file.SetKeyValue("OUTPUT_FILE","main");
                        break;
                    }
                }
            }

        }
        else
        {
            std::cout << "Could not create the directory" << _projectdirpath << std::endl;
            exit(22);
        }

    }
    else
    {

        std::cout << "Insufficient parameters provided!" << std::endl;
        exit(0);
    }
    return 0;
}