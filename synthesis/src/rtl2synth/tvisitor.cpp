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

#include "tvisitor.h"
#include "sycontainer.h"
#include "syexpr.h"
#include "syconst.h"
#include "sycntvs.h"

namespace Synth
{
    void
        TVisitor::visit(
                SyModule *mod,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyBModule *mod,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyScPort *port,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyVPort *vport,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyScNet *net,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyVNet *vnet,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyScReg *reg,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyVReg *vreg,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyBitSel *bitsel,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyPartSel *partsel,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyConcat *conc,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyUnary *unary,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyBinary *binary,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyAlways *always,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyEventStmt *event,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyIf *if_,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyCase *case_,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyScPin *pin,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyVPin *vpin,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyCondExpr *cexpr,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyFuncCall *fcall,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyTaskCall *tcall,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyFunc *func,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyTask *task,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyPrimInst *pinst,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyModInst *minst,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyConst *const_,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyContAssign *cassign,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyBlockAssign *bassign,
                SyModule *pmod)
        {
        }
    void
        TVisitor::visit(
                SyNonBlockAssign *nbassign,
                SyModule *pmod)
        {
        }
}
