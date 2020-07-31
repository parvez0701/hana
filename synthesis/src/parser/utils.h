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

#ifndef _UTILS_H_
#define _UTILS_H_
#include <vector>
#include <string>
#include <set>

#include <string.h>
#include "defines.h"
#include "ixname.h"

using std::string;
using std::set;
namespace Rtl
{
    extern const unsigned int KILO;
    extern const unsigned int MEGA;
    extern const unsigned int GIGA;
    class Range;
    class Module;
    class Base;
    class Scope;
    typedef struct{
        int line;
        string file;
        string msg;
    }Error;
    /******************************************************************
     * Functor to compare two strings.
     * ***************************************************************/
//    struct compare_str{
//        bool operator()(const string& s1, const string& s2) const{
//            return strcmp(s1.c_str(), s2.c_str()) < 0;
//        }
//    };

    /******************************************************************
     * Functor to compare two design objects which have a the function
     * getName()
     * ***************************************************************/
    template <typename T1, typename T2>
    struct compare_name{
        bool operator()(const T1* t1, const T2* t2){
            struct compare_str cmp;
            return cmp(t1->getName(), t2->getName());
        }
    };

    template <typename T>
    struct DELETE: public std::unary_function<T, bool>{
         bool operator()(T* t)
		 {
            delete t;
			return true;
        }
    };
        
    template<typename T>
    class equal_name{
        string name;
        public:
            explicit equal_name(const string& _name): name(_name){
            }
            bool operator()(const T* t) const{
                return t->getName() == name;
            }
    };

    std::string GetDirString(const Dir);
    std::string GetNetTypeString(const NetType);
    /*
    void PrintMsg(const std::string&);
    void PrintMsg(const std::string&, const string&, const string&);
    void PrintMsg(const std::string&, const int);
    void PrintMsg(const std::string&, const int, const string&);
    void PrintMsg(const std::string&, const int, const string&, const int,
        const string&, const int, const string&);
    void PrintMsg(const string&, const int, const string&,
        const string&, const string&, const int, const string&,
         const int);
    void PrintMsg(const string&, const int, const string&,
        const string&, const string&);
    void PrintMsg(const string&, const int, const string&,
        const string&);
    */    
    long Evaluate(const char base, std::vector<char> bits);
    std::string GetGateName(const GateType);
    std::string GetDriveStrName(const DrStr);
    std::string GetOpString(const OpType);

    template<typename T>
    int GetNumElems(const std::vector<T*>& vec)
    {
        set<string, compare_str> doneObjs;
        const int size = vec.size();
        int num = 0;
        for(int i = 0; i < size; ++i)
        {
            T* t = vec[i];
            IxName ixName = t->getIxName();
            if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                continue;
            }
            doneObjs.insert(ixName.getName());
            ++num;
        }
        return num;
    }

    template<typename T>
    std::vector<std::string> GetNames(const std::vector<T*>& vec)
    {
        set<string, compare_str> doneObjs;
        const int size = vec.size();
        std::vector<std::string> retvec;
        for(int i = 0; i < size; ++i)
        {
            T* t = vec[i];
            IxName ixName = t->getIxName();
            if(doneObjs.find(ixName.getName()) != doneObjs.end()){
                continue;
            }
            doneObjs.insert(ixName.getName());
            retvec.push_back(ixName.getName());
        }
        return retvec;
    }



    int ParseDimension(const std::vector<Expr*>&, 
        const Expr*, const bool ispartsel=false);
    int ParseRange(const Range*, const Expr*);

    std::string GetPolOpString(PolOp);

    const Module* GetModule(const Base*);
    const Base* GetObject(const std::string&, const Scope*);
    Path GetObjectPath(const Base*);
    Expr* ReplaceSubExpression(Expr*, const std::string&, const Expr*);

    template<class T>  void CopyVector(
    const std::vector<T>& src, std::vector<T>& des)
    {    
        for(unsigned int i = 0; i < src.size(); ++i) {
            des.push_back(dynamic_cast<T>(src[i]->copy()));
        }    
    }    

    void Tokenize(    const std::string&, 
                    std::vector<std::string>&, 
                    const std::string&);


//Generally used when error message is not of much significance.
#define throw_error Error error = {__LINE__, __FILE__, "Error occured"};\
    throw error
#define catch_error catch(Error& e){}
        
}
#endif
