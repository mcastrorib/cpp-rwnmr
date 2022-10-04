// include C++ standard libraries
#include <iostream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <string>

#include "ArgsParser.h"

ArgsParser::ArgsParser(int argc, char **argv)
{
    vector<string> commands();
    vector<string> paths();

    // initialize essentials
    (*this).addCommand("rwnmr");
    (*this).addPath("default");
    (*this).addCommand("uct");
    (*this).addPath("default");

    // read args
    if(argc > 1)
    {
        int arg_idx = 1;
        while(arg_idx < argc)
        {
            string argument(argv[arg_idx]);

            if(argument == "-rwconfig") 
            {
                
                if((arg_idx + 1) < argc)
                {
                    string newPath = argv[arg_idx+1];
                    (*this).setPath(newPath, 0);
                }                
            }
            else if(argument == "-uctconfig")
            {                
                if((arg_idx + 1) < argc)
                {
                    string newPath = argv[arg_idx+1];
                    (*this).setPath(newPath, 1);
                }                
            }
            else if(argument == "cpmg")
            {
                (*this).addCommand("cpmg");

                if((arg_idx + 1) < argc)
                {
                    string addFlag = argv[arg_idx + 1];
                    if(addFlag == "-config" and (arg_idx + 2) < argc)
                    {
                        string newPath = argv[arg_idx + 2];
                        (*this).addPath(newPath);
                    } else
                    {
                        (*this).addPath("default");
                    }
                } else
                {
                    (*this).addPath("default");
                }                
            }
            else if(argument == "pfgse")
            {
                (*this).addCommand("pfgse");

                if((arg_idx + 1) < argc)
                {
                    string addFlag = argv[arg_idx + 1];
                    if(addFlag == "-config" and (arg_idx + 2) < argc)
                    {
                        string new_path = argv[arg_idx + 2];
                        (*this).addPath(new_path);
                    } else
                    {
                        (*this).addPath("default");
                    }  
                } else
                {
                    (*this).addPath("default");
                }  
            }
            else if(argument == "ga")
            {
                (*this).addCommand("ga");

                if((arg_idx + 1) < argc)
                {
                    string addFlag = argv[arg_idx + 1];
                    if(addFlag == "-config" and (arg_idx + 2) < argc)
                    {
                        string newPath = argv[arg_idx + 2];
                        (*this).addPath(newPath);
                    } else
                    {
                        (*this).addPath("default");
                    }
                } else
                {
                    (*this).addPath("default");
                }  
            }
            else if(argument == "multitau")
            {
                (*this).addCommand("multitau");
                (*this).addCommand("mtoff");

                if((arg_idx + 1) < argc)
                {
                    string addFlag = argv[arg_idx + 1];
                    if(addFlag == "-config" and (arg_idx + 3) < argc)
                    {

                        string multitauPath = argv[arg_idx + 2];
                        string cpmgPath = argv[arg_idx + 3];
                        (*this).addPath(multitauPath);
                        (*this).addPath(cpmgPath);

                    } else if(addFlag == "-mtconfig" and (arg_idx + 2) < argc)
                    {
                        string multitauPath = argv[arg_idx + 2];
                        string cpmgPath = "default";
                        (*this).addPath(multitauPath);
                        (*this).addPath(cpmgPath);

                    }else if(addFlag == "-cpmgconfig" and (arg_idx + 2) < argc)
                    {
                        string multitauPath = "default";
                        string cpmgPath = argv[arg_idx + 2];
                        (*this).addPath(multitauPath);
                        (*this).addPath(cpmgPath);
                        
                    } else
                    {
                        (*this).addPath("default");
                        (*this).addPath("default");
                    }
                } else
                {
                    (*this).addPath("default");
                    (*this).addPath("default");
                }  
            }

            // increment argument
            arg_idx++;
        }
    }

}

ArgsParser::ArgsParser(const ArgsParser &_otherParser)
{
    this->commands = _otherParser.commands;
    this->paths = _otherParser.paths;    
}