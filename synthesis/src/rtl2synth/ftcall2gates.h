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

#ifndef _FTCALL2GATES_H_
#define _FTCALL2GATES_H_

#include "stmt2gates.h"
namespace Synth
{
    typedef std::pair<SyCntv*, SyCntv*> PortNetPair;
    struct InOutCntvs
    {
        SyList<PortNetPair>    inputs;
        SyList<PortNetPair>    outputs;
        SyList<PortNetPair>    inouts;

        bool empty() const {
            return inputs.empty() && outputs.empty() && inouts.empty();
        }    
    };    
    
    typedef std::map<std::string, InOutCntvs, cmp_str> CallInOuts;
    typedef std::map<std::string, CallInOuts, cmp_str> ModFuncTaskCallTable;
    typedef std::map<SyCntv*, SyCntv*, cmp_cntvs> Ports2Nets;

    class FuncTaskCall2Gates
    {
        public:
            static FuncTaskCall2Gates* instance();
            ~FuncTaskCall2Gates();
            void inlineExpand(    const SyModule*, 
                                const SyModule*, 
                                const SyTaskOrFuncCall*,
                                Out2Rep=Out2Rep());
            TaskFuncCallInfo getTaskFuncCallInfo();                    
                                
            
        private:
            FuncTaskCall2Gates();
            DISABLE_COPYCONST_ASSGNOP(FuncTaskCall2Gates);

            InOutCntvs     getInOutInfo(const std::string&, const std::string&) const;
            void        addInOutInfo(const std::string&, const std::string&,
                            InOutCntvs&);
            
            void        associateOut2Net(InOutCntvs&, SyCntv*, SyCntv*) const;

            InOutCntvs    createNets(const SyList<SyScPort*>&);
            void        createFuncOutNets(const SyTaskOrFuncCall*, InOutCntvs&);
            void connectPortsWithArgs(const SyTaskOrFuncCall*, InOutCntvs&);
            void createBufs(const SyNode<SyCntv*>*, 
                            const SyExpr*, 
                            bool, 
                            bool=false,
                            InOutCntvs* = 0);
            SyList<SyStmt*>* replacePortsByNets(const SyList<SyStmt*>&) const;
            SyStmt* replacePortsByNets(const SyStmt*) const;
            SyExpr* replacePortsByNets(const SyExpr*) const;

            OutCntvInfoSet*    expand(const SyList<SyStmt*>&, Out2Rep);
            void    mergeInOutCntvs(const OutCntvInfoSet*, 
                SyList<PortNetPair>&);


            //Only for non-reentrant tasks and functions, since they
            //are shared across call.
            ModFuncTaskCallTable modFuncTaskCallTable;
            Ports2Nets    ports2Nets;

            Always2Gates    *always2Gates;
            TaskFuncCallInfo    tfCallInfo;
            static FuncTaskCall2Gates    *funcTaskCall2Gates;        
    };
}
#endif
