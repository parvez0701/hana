/* 
Copyright (C) 2009-2011 Parvez Ahmad
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

#ifndef _SOPMUXTRANS_H_
#define _SOPMUXTRANS_H_

#include "soptransbase.h"
namespace SOP
{
    class SopMuxTrans : public SopTransBase
    {
        public:
            virtual ~SopMuxTrans() { }
            virtual SopMuxTrans* copy() const { return new SopMuxTrans(); }
            virtual void transform(const SInst *inst)
            {
                std::list<const SPin*> input = inst->getSpecialPins("DATAIN");
                std::list<const SPin*> select = inst->getSpecialPins("SELECT");
                int incount = input.size();
                int selcount = select.size();
                std::list<const SPin*>::const_iterator seliter = select.begin();
                std::list<const SPin*>::const_iterator initer = input.begin();
                SExpr *expr = 0;
                for(int count = 0; count < incount; ++count, ++initer)
                {
                    SExpr *texpr = this->getExpr(*initer);
                    seliter = select.begin();
                    for(int sel = 1; sel <= selcount; ++sel, ++seliter)
                    {
                        if((1 << (sel-1)) & count) {
                            //check if the select bit is set
                            texpr = SExprTree::createNode(
                                SExpr::AND, 0, texpr, SExprTree::subTreeCopy(
                                this->getExpr(*seliter)));
                        }
                        else {
                            texpr = SExprTree::createNode(
                                SExpr::AND, 0, texpr, 
                                SExprTree::createNode(
                                    SExpr::NOT, 0, SExprTree::subTreeCopy(
                                    this->getExpr(*seliter))));
                        }
                    }
                    expr = expr ? SExprTree::createNode(SExpr::OR, 0, expr, texpr) : texpr;
                }
                this->addExpr(inst->getSpecialPin("DATAOUT"), expr);
            }
    };
}
#endif
