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

#include <sstream>
#include <ctype.h>
#include <algorithm>
#include <assert.h>

#include "compdirctvmgr.h"

using std::map;
using std::string;
using std::set;
using std::pair;
using std::make_pair;
using std::vector;
using std::ostringstream;

extern void yyerror(char const *);
namespace Rtl
{
    CompDirctvMgr* CompDirctvMgr::compDirctvMgr = 0;

    CompDirctvMgr::CompDirctvMgr() {
    }

    CompDirctvMgr::~CompDirctvMgr() {
    }

    CompDirctvMgr*
    CompDirctvMgr::instance()
    {
        if(!CompDirctvMgr::compDirctvMgr) {
            compDirctvMgr = new CompDirctvMgr();
        }
        return compDirctvMgr;
    }    

    bool
    CompDirctvMgr::ParamDefine::operator==(const ParamDefine& rhs) const{
        return name == rhs.name;
    /*
        if(text.size() != rhs.text.size()) {
            return false;
        }
        for(int i = 0; i < text.size(); ++i) {
            if(text[i] != rhs.text[i]) {
                return false;
            }
        }
        return true;
    */    
    }
    bool
    CompDirctvMgr::ParamDefine::operator!=(const ParamDefine& rhs) const {
        return !(*this == rhs);
    }    

    bool
    CompDirctvMgr::ParamDefine::operator<(const ParamDefine& rhs) const
    {
        return compare_str()(name, rhs.name);
    /*
        string s1, s2;
        for(int i = 0; i < text.size(); ++i) {
            s1 += text[i];
        }
        for(int i = 0; i < rhs.text.size(); ++i) {
            s2 += rhs.text[i];
        }

        return compare_str()(s1, s2);
    */    
    }
    bool
    CompDirctvMgr::ParamDefine::dirty() const {
        return name.empty();
    }    

    bool
    CompDirctvMgr::ParamDefine::isArg(const string& str) const 
    {
        if(text.empty()) {
            return false;
        }

        for(int i = 0; i < text.size(); ++i) {
            if(text[i].first == str) {
                return text[i].second;
            }
        }
        return false;
    }

    int
    CompDirctvMgr::ParamDefine::getParamSize() const {
        return text.size();
    }    

    bool 
    CompDirctvMgr::isParamDefine(const string& str) const {
        return str.find_first_of('(') != string::npos;
    }

    bool 
    CompDirctvMgr::isSimpleDefine(const string& str) const {
        return !(isParamDefine(str));
    }

    bool
    CompDirctvMgr::addSimpleDefine(    const string& param, 
                                    const string& repTxt) {
        return defineTable.insert(make_pair(param, repTxt)).second;
    }
/*
    bool
    CompDirctvMgr::addParamDefine(    const string& param,
                                    const string& repTxt)
    {
        ParamDefine paramDefine = parseParamDefine(param, repTxt);
        if(parseParamDefine.dirty()) {
            return false;
        }
        paramDefineTable.erase(parseParamDefine);
        parseParamDefine.insert(parseParamDefine);
        return true;
    }
*/
    vector<CompDirctvMgr::ArgOrText>
    CompDirctvMgr::getText(const string& param) const
    {
        vector<ArgOrText> vec;
        string buf;
        for(int i = 0; i < param.size(); ++i)
        {
            if(param[i] == ' ' || param[i] == '    ' || param[i] == ',' ||
                param[i] == '(' || param[i] == ')') 
            {
                if(!buf.empty()) 
                {
                    vec.push_back(make_pair(buf, true));
                    buf.clear();
                }    
                continue;
            }    
            buf.append(1, param[i]);
            
        }
        if(!buf.empty()) {
            vec.push_back(make_pair(buf, true));
        }
        return vec;
/*        
        if(isSimpleDefine(param)) 
        {
            vec.push_back(make_pair(param, false));
            return vec;
        }
        int index = param.find_first_of('(');
        vec.push_back(make_pair("(", false));
        
        string tmp = param.substr(index + 1);

        while((index = tmp.find_first_of(',')) != string::npos)
        {
            string tname = tmp.substr(0, index);
            tname = tname.substr(tmp.find_first_not_of("     "));
            int tindex = tname.find_first_of("     ()");
            if(tindex != string::npos) {
                vec.push_back(make_pair(tname.substr(0, tindex), true));
            }
            else {
                vec.push_back(make_pair(tname, true));
            }    
            tmp = tmp.substr(tmp.find_first_not_of("     ", index+1));
        }
        //the last remaining
        index = tmp.find_first_of(')');
        if(index != string::npos)
        {
            tmp = tmp.substr(0, index);
            index = tmp.find_first_of("     ");
            if(index == string::npos) { 
                vec.push_back(make_pair(tmp.substr(0, index), true));
            }
            else {
                vec.push_back(make_pair(tmp, true));
            }
        }    
        return vec;
    */    
    }
    pair<string, string>
    CompDirctvMgr::parse(const string& str) const
    {
        string tstr = str;
        int index = tstr.find_first_not_of("     ");
        if(index != string::npos) {
            tstr = tstr.substr(index);
        }    

        if(this->isSimpleDefine(tstr))
        {
            index = tstr.find_first_of("     ");
            if(index == string::npos) {
                return make_pair(tstr, "");
            }
            else
            {
                string tmp = tstr.substr(0, index); 
                tstr = tstr.substr(index);
                index = tstr.find_first_not_of("     ");
                if(index == string::npos) {
                    tstr = " ";
                }
                else
                {
                    tstr = tstr.substr(index);
                    tstr = tstr.substr(0, tstr.find_first_of("     "));
                }    
                return make_pair(tmp, tstr);
            }
        }

        //complex define
        if(!this->checkForMatchingBraces(str))
        {
            yyerror("unmatched left and right braces.");
            return make_pair("", "");
        }
        tstr = tstr.substr(tstr.find_first_of('('));
        if(!this->checkForValidArgs(tstr)) 
        {
            yyerror("");
            return make_pair("", "");
        }    
        string txt = tstr.substr(1, tstr.find_first_of(')')-1);
        string rep = tstr.substr(tstr.find_first_of(')')+1);
        return make_pair(txt, rep);
    }

    bool
    CompDirctvMgr::checkForMatchingBraces(const string & str) const
    {
        int count = 0;

        for(int i = 0; i < str.size(); ++i)
        {
            if(count < 0) {
                return false;
            }
            if(str[i] == '(') {
                ++count;
            }
            else if (str[i] == ')') {
                --count;
            }    
        }
        return count == 0;
    } 

    bool
    CompDirctvMgr::checkForValidArgs(const string& str) const
    {
        //check for braces should be called before this
        string tstr = str;
        int index = tstr.find_first_of('(');
        tstr = tstr.substr(index+1, tstr.find_first_of(')')-index-1);
        vector<string> tokens;
        Tokenize(tstr, tokens, ",");

        for(int i = 0; i < tokens.size(); ++i)
        {
            string tmp = tokens[i];
            tmp = tmp.substr(tmp.find_first_not_of("     "));

            int index = tmp.find_first_of("     ");
            if(index != string::npos)
            {
                //should be trailing spaces
                tmp = tmp.substr(index);
                if((tmp.find_first_not_of("     ")) != string::npos) {
                    return false;
                }
            }
            else 
            {
                if(!isalpha(tmp[0]) && (tmp[0] != '_')) {
                    return false;
                }
                for(int j = 1; j < tmp.size(); ++j) {
                    if(!isalnum(tmp[j]) && (tmp[j] != '_')) {
                        return false;
                    }    
                }
            }
            
        }
        return true;
        
    }

    bool
    CompDirctvMgr::process(const string& str)
    {
        pair<string, string> define = this->parse(str);

        if(define.first.empty()) {
            return false;
        }    

        if(this->isSimpleDefine(str))
        {
            this->removeSimpleDefine(define.first);
            this->addSimpleDefine(define.first, define.second);
            return true;
        }
        else if(this->isParamDefine(str))
        {
           // yyerror("`define with parameters is not supported yet.");
            //return false;
            vector<string> argsStr;
            vector<string> repStr;
            vector<string> tmpStr;
            Tokenize(define.first, argsStr, ",     ()");
            Tokenize(define.second, tmpStr, "     ");
            //ostringstream tmp;
            for(int i = 0; i < tmpStr.size(); ++i)
            {
                //tmp.flush();
                string buf;
                for(int j = 0; j < tmpStr[i].size(); ++j) { 
                    if((tmpStr[i][j] != '(') && 
                        (tmpStr[i][j] != ')')) {
                        buf.append(1, tmpStr[i][j]);
                    }
                    else
                    {
                        if(!buf.empty()) {
                            repStr.push_back(buf);
                        }    
                        buf.clear();    
                        buf.append(1, tmpStr[i][j]);
                        repStr.push_back(buf);
                        buf.clear();
                    }    
                }
                if(!buf.empty()) {
                    repStr.push_back(buf);
                }    
                
            }
            ParamDefine paramDefine;
            string tstr = str;
            tstr = tstr.substr(tstr.find_first_not_of("     "));
            tstr = tstr.substr(0, tstr.find_first_of("     ("));
            paramDefine.name = tstr;
            for(int i = 0; i < argsStr.size(); ++i) {
                paramDefine.text.push_back(make_pair(argsStr[i], true));
            }    

            for(int i = 0; i < repStr.size(); ++i) {
                paramDefine.repText.push_back(
                    make_pair(repStr[i], paramDefine.isArg(repStr[i])));
            }
            paramDefineTable.erase(paramDefine);
            paramDefineTable.insert(paramDefine);
        }    
        return true;

    }

    bool
    CompDirctvMgr::addDefine(const string& args, const string& rep) 
    {
        defineTable.erase(args);
        defineTable.insert(make_pair(args, rep));
        return true;
    }    

    string
    CompDirctvMgr::getRepText(const string& str)
    {
        if(str.empty()) {
            return "";
        }    
        assert(!currName.empty());
        string name = currName;

        //if(isSimpleDefine(str))
        DefineTable::const_iterator iter = defineTable.find(name);
        if(iter != defineTable.end()) 
        {
            string repStr = iter->second + string(" ") + str;
            std::reverse(repStr.begin(), repStr.end());
            return repStr;
        }    

        ParamDefine define;

        define.name = name;

        ParamDefineTable::const_iterator paramDefine = paramDefineTable.find(define);
        if(paramDefine == paramDefineTable.end()) {
            return "";
        }

        vector<ArgOrText> vec = this->getText(str);
        return this->getReplacementText(vec, *paramDefine);
    }

    string
    CompDirctvMgr::getReplacementText(    const std::vector<ArgOrText>& args,
                                        const ParamDefine& param) const
    {
        if(param.getParamSize() != args.size()) {
            return "";
        }
        
        vector<ArgOrText> repText = param.repText;
        for(int i = 0; i < args.size(); ++i) 
        {
            for(int j = 0; j < repText.size(); ++j) 
            {
                if(repText[j].second)
                {
                    if(repText[j].first == param.text[i].first) {
                        repText[j].first = args[i].first;
                    }    
                }    
            }
        }
        string str;
        for(int i = 0; i < repText.size(); ++i) {
            str += repText[i].first;
        }
        std::reverse(str.begin(), str.end());
        return str;
#if 0
        bool done = false;
        int i = 0;
        int j = 0;
        string str;
        while(!done)
        {
            str += " "; //add a space
            if(param.repText[j].second) {
                str += args[i++].first;
            }
            else {
                str += param.repText[j].first;
            }
            ++j;
            if((i == args.size()) || (j == param.repText.size())) {
                done = true;
            }    
        }
        return str;
#endif        
    }
    bool
    CompDirctvMgr::removeDefine(const string& def)
    {
        if(removeSimpleDefine(def)) {
            return true;
        }
        return removeParamDefine(def);
    }    
            

    bool
    CompDirctvMgr::removeSimpleDefine(const string& def)
    {
        if(defineTable.find(def) == defineTable.end()) {
            return false;
        }

        defineTable.erase(def);
        return true;
    }    

    bool
    CompDirctvMgr::removeParamDefine(const string& str)
    {
        ParamDefine define;
        define.name = str;
        if(paramDefineTable.find(define) == paramDefineTable.end()) {
            return false;
        }
        paramDefineTable.erase(define);
        return true;
    }    

    void
    CompDirctvMgr::setCurrName(const string& name) {
        currName = name;
    }    
       
}
