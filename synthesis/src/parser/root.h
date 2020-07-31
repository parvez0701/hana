/* 
Copyright (C) 2009-2010 Parvez Ahmad
Written by Parvez Ahmad <parvez_ahmad@yahoo.co.uk>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef _ROOT_H_
#define _ROOT_H_
/**********************************************************************
 * Root class is to represent an RTL input design. This is a simple
 * structure and contains the enviorenment under which the tool was
 * invoked. This includes all command line options aswell.
 * At this moment it contains all file list only.
 * *******************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "file.h"
#include "utils.h"
using std::vector;
using std::string;
using std::cout;
using std::cerr;
typedef void (*FUNC)(const std::string&);
namespace Rtl
{
    class File;
    class Base;
    class Module;
    class Root
    {
        public:
            //This constuctor throws Error when error
            //occurs
            Root(int argc, char**argv);
            Root();
            virtual ~Root();
            int        parse();
            int        parse(int argc, char** argv);
            const    vector<File*>&    getFiles() const{
                return files;
            }
            const std::string& getLogFile() const{
                return logfname;
            }

            const std::string& getDecFile() const {
                return decfname;
            }
            const    File* getFile(const string& name){
                const int size = files.size();
                for(int i = 0; i < size; ++i) {
                    if(files[i]->getName() == name){
                        return files[i];
                    }
                }
                return 0;
            } 
            void decompile() const;
            //Always deep copy
            virtual Root* copy() const{ return 0;}
            Base* addObject(const std::string& n,
                const Base* b, const bool r);
            void removeObject(const std::string& n){
                symbolTable.erase(n);
            }

            Base* getObject(const std::string& n) const;

            const std::vector<File*>& getLibFiles() const{
                return libfiles;
            }
            bool isTopModule(const std::string&) const;
            const std::vector<Module*>& getTopModules() const {
                return topmodules;
            }
            void populateTopModules();
            const std::vector<std::string>& getInclDirs() const {
                return incldirs;
            }
            const std::string& getVersion() const {
                return version;
            }    
            
        private:
            Root(const Root&);
            const Root& operator=(const Root&) const;
            //Functions
            void printHelp() const;
            void printVersion() const;
            int checkAndAddDecFile(int, int, char**);
            int checkAndAddLogFile(int, int, char**);
            int checkAndAddLibFile(int, int, char**);
            int checkAndAddInclDir(int, int, char**);
            //data
            vector<File*>    files;
            vector<File*> libfiles;
            std::string logfname;
            std::string decfname;
            std::ofstream logfile;
            std::vector<std::string> incldirs;
            std::map<std::string, Base*, compare_str> symbolTable;
            std::vector<Module*> topmodules;
            const std::string version;
    };
}
#endif
