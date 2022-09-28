#ifndef ARGS_PARSER_H_
#define ARGS_PARSER_H_

// include C++ standard libraries
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ArgsParser
{
private:
    vector<string> commands;
    vector<string> paths;

public:       
    // default constructors
    ArgsParser(){};
    ArgsParser(int argc, char **argv);
    
    //copy constructors
    ArgsParser(const ArgsParser &_otherParser);

    // default destructor
    virtual ~ArgsParser(){}

    void setCommands(vector<string> c){ this->commands = c; }
    void setPaths(vector<string> p){ this->paths = p; }
    
    void setCommand(string c, int idx){ 
        if(idx < this->commands.size()) this->commands[idx] = c; 
        else cout << "incompatible args size." << endl;
    }
    void setPath(string p, int idx){ 
        if(idx < this->paths.size()) this->paths[idx] = p; 
        else cout << "incompatible args size." << endl;
    }

    void addCommand(string c){ this->commands.push_back(c); }
    void addPath(string p){ this->paths.push_back(p); }

    vector<string> getCommands(){ return this->commands; }
    vector<string> getPaths(){ return this->paths; }
    string getCommand(uint idx) {return this->commands[idx];}
    string getPath(uint idx) {return this->paths[idx];}

    void print()
    {
        for(int i = 0; i < this->commands.size(); i++)
        {
            cout << "(" << i << ") " 
            << "C: " << this->commands[i]
            << " P: " << this->paths[i] << endl; 
        }
    }
      
};

#endif