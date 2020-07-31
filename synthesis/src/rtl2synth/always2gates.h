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

#ifndef _ALWAYS2GATES_H_
#define _ALWAYS2GATES_H_
#include <stdexcept>

#include "expr2gates.h"
#include "stmt2gates.h"
#include "syprocmgr.h"

namespace Synth
{
    class Always2Gates : public ConVisitorBase
    {
        public:
            friend class Stmt2Gates;
            Always2Gates(const SyModule*, const SyModule*, const SyScope*);
            virtual ~Always2Gates();

            virtual void visit(const SyAlways*);

            virtual void setCurrMod(SyModule*);
            virtual void setCurrRtlMod(SyModule*);
            virtual void setCurrSynthMod(SyModule*);
            virtual void setCurrScope(SyScope*);

            void setStmt2Gates(Stmt2Gates *s2g);

        private:
            DISABLE_COPYCONST_ASSGNOP(Always2Gates);

            //private functions
            void            createGates(std::vector<OutCntvInfoSet*>&);
            void            createInputList(SyList<SyCntv*>&,
                            SyCntv*,
                            bool,
                            std::map<SyCntv*, SyCntv*>&);

            //private data
            const AlwaysInfo_ *alwaysInfo;
            Stmt2Gates *stmt2Gates;

    };

}
#endif
