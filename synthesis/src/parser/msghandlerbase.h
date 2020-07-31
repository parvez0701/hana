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

#ifndef _MSGHANDLERBASE_H_
#define _MSGHANDLERBASE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <cassert>
#include <cstdarg>
#include <sstream>

namespace Rtl
{
    struct MsgInfo {
        typedef enum { IMM, DEF, DRO} MsgFlag;
        typedef enum { NOTYPE, INFO, WARN, ERROR } MsgType;
        MsgInfo(const std::string& fo,
            const int no,
            const MsgFlag f,
            MsgType t):
            format(fo), msgno(no), flag(f), type(t){
        }
        std::string format;
        int msgno;
        MsgFlag flag;
        MsgType type;
    };


    template< class Handler>
    class MsgHandlerBase
    {
        public:
            static Handler* instance();
            virtual ~MsgHandlerBase();

            typedef std::map<int, const MsgInfo*> MsgTable;
            typedef std::vector<std::pair<int, std::string> > MsgLog;

            int setLogFile(const std::string&, bool=true);
            std::ofstream* getLogFile();
            bool print(int, ...);
            //flush out log messages
            void print();
            void print(const std::string&);

            void disable(int msgno);
            void close();
            int getMsgCount(MsgInfo::MsgType) const;
        protected:
            MsgHandlerBase();
            bool addMsgInfo(const MsgInfo *);

        private:
            //creation/deletion related stuff
            static Handler* msghandler;
            MsgHandlerBase(const MsgHandlerBase&);
            MsgHandlerBase& operator=(const MsgHandlerBase&);

            //functions
            void flush();
            MsgInfo::MsgType getMsgType(int) const;

            //data
            std::ofstream *logFile;
            MsgTable msgTable;
            MsgLog defMsgLog;
            MsgLog msgLog;
    };


    template < class Handler >
    Handler* MsgHandlerBase<Handler>::msghandler = 0;

    template < class Handler >
    MsgHandlerBase<Handler>::MsgHandlerBase(): logFile(0) {
    }

    template < class Handler >
    MsgHandlerBase<Handler>::~MsgHandlerBase() 
    {
        close();
        msghandler = 0;
    }

    template < class Handler > Handler * 
    MsgHandlerBase<Handler>::instance()
    {
        if(!msghandler) {
            msghandler = new Handler();
        }
        return msghandler;
    }

    
    template < class Handler > int
    MsgHandlerBase<Handler>::setLogFile(const std::string& name, bool trunc)
    {
        if(logFile){
            //Can not change the output file in the same run
            return 1;
        }
        logFile = new std::ofstream();
        logFile->open(name.c_str(), trunc ? std::ios_base::trunc : std::ios_base::app);
        if(logFile->bad())
        {
            delete logFile;
            logFile = 0;
            return 1;
        }
        return 0;
    }

    template < class Handler > std::ofstream*
    MsgHandlerBase<Handler>::getLogFile() {
        return logFile;
    }

    template < class Handler > bool
    MsgHandlerBase<Handler>::print(int msgno, ...)
    {
        if(msgno < 0 ){
            return false;
        }
        if(msgTable[msgno]->flag == MsgInfo::DRO) {
            //drop this message
            return true;
        }
        const char *p = msgTable[msgno]->format.c_str();
        va_list ap;
        va_start(ap, msgno);
        std::ostringstream str;
        while(*p != '\0')
        {
            if(*p == '%')
            {
                ++p;
                if(!p)
                {
                    str << *p;
                    //check
                    return false;
                }

                switch (*p)
                {
                    case 'd':
                        str << va_arg(ap, int);
                        ++p;
                        continue;

                    case 's':
                        str << va_arg(ap, char*);
                        ++p;
                        continue;

                    case 'f':
                        str << va_arg(ap, double);
                        ++p;
                        continue;
                    default:
                        assert(0);
                }
            }
            str << *p;
            ++p;
        }
        msgLog.push_back(make_pair(msgno, str.str()));
        if(msgTable[msgno]->flag == MsgInfo::DEF)
        {
            //defer printing of message
            defMsgLog.push_back(make_pair(msgno, str.str()));
            return true;
        }
        this->print(str.str());
        return true;
    }

    template < class Handler > void
    MsgHandlerBase<Handler>::print(const std::string& str){
        if(logFile && logFile->good()){
            (*logFile) << str;
        }
        else{
            std::cout << str;
        }
    }
    template < class Handler > void
    MsgHandlerBase<Handler>::flush()
    {
        for(int i = 0; i < defMsgLog.size(); ++i)
        {
            if(msgTable[defMsgLog[i].first]->flag != MsgInfo::DRO){
                this->print(defMsgLog[i].second);
            }
        }
    }

    template < class Handler > void
    MsgHandlerBase<Handler>::print() {
        flush();
    }    
    
    template < class Handler > void
    MsgHandlerBase<Handler>::close()
    {
        this->flush();
        if(logFile)
        {
            logFile->close();
            delete logFile;
            logFile = 0;
        }
    }

    template < class Handler > bool
    MsgHandlerBase<Handler>::addMsgInfo(const MsgInfo *info)
    {
        if(!info) {
            return false;
        }
        return msgTable.insert(std::make_pair(info->msgno, info)).second;
    }

    template < class Handler > MsgInfo::MsgType
    MsgHandlerBase<Handler>::getMsgType(int msgno) const
    {
        MsgTable::const_iterator iter = msgTable.find(msgno);
        if(iter == msgTable.end()) {
            return MsgInfo::NOTYPE;
        }

        return iter->second->type;
    }

    template < class Handler > int 
    MsgHandlerBase<Handler>::getMsgCount(MsgInfo::MsgType type) const
    {
        int count = 0;
        for(int i = 0; i < msgLog.size(); ++i) {
            if(this->getMsgType(msgLog[i].first) == type) {
                ++count;
            }
        }
        return count;
    }

    
}
#endif
