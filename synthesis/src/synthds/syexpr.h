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

#ifndef _SYEXPR_H_
#define _SYEXPR_H_
#include <vector>
#include <sstream>

#include <limits.h>

#include "sylist.h"
namespace Synth
{
    class SyTaskOrFuncCall;
    class SyFunc;
    class SyVCntv;
    class SyConst;
    class SyVisitorBase;
    class TVisitor;
    class SyModule;
    class SyScCntv;
    class SyScPin;
    class SyConcat;

    class SyExpr: public SyBase
    {
        friend class SyExprMgr;
        friend class SyExprExtVisitor;
        public:
            typedef enum {
                NOOP,
                UNPLUS,
                UNMINUS,
                LOGNEG,
                LOGAND,
                LOGOR,
                GTHAN,
                GTHANEQ,
                LTHAN,
                LTHANEQ,
                LOGEQ,
                LOGENEQ,
                CASEEQ,
                CASEENEQ,
                BITNEG,
                BITAND,
                BITNAND,
                BITOR,
                BITNOR,
                BITXOR,
                BITNXOR,
                BITEQUIV,
                REDAND,
                REDNAND,
                REDNOR,
                REDOR,
                REDXOR,
                REDXNOR,
                LSHIFT,
                RSHIFT,
                RRSHIFT,
                PLUS,
                MINUS,
                MULT,
                DIV,
                MOD,
                COND,
                POW
            } SyOp;

            // SyExpr() : csign(false), csize(0) {
            //}

            virtual ~SyExpr() {
              //  delete csize;
            }

            virtual bool isSigned() const = 0;
            virtual SyExpr *copy() const = 0;
            virtual bool contains(const std::string&) const = 0;
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*) {
            }
            virtual std::string toString() const = 0;

            virtual SyConst* evaluate() const { return 0;}
            virtual SyConst* size() const;
            virtual void addVal(const SyExpr *v, bool discard=true){
            }
            virtual void removeVal() {
            }

            virtual SyList<SyScCntv*>* toGate(SyModule* mod) {
                return NULL;
            }

            bool isConst() const;

            static std::string getOpString(SyOp op)
            {
                switch (op)
                {
                    case SyExpr::UNPLUS: return "+";
                    case SyExpr::UNMINUS: return "-";
                    case SyExpr::LOGNEG: return "!";
                    case SyExpr::LOGAND: return "&&";
                    case SyExpr::LOGOR: return "||";
                    case SyExpr::GTHAN: return ">";
                    case SyExpr::GTHANEQ: return ">=";
                    case SyExpr::LTHAN: return "<";
                    case SyExpr::LTHANEQ: return "<=";
                    case SyExpr::LOGEQ: return "==";
                    case SyExpr::LOGENEQ: return "!=";
                    case SyExpr::CASEEQ: return "===";
                    case SyExpr::CASEENEQ: return "!==";
                    case SyExpr::BITXOR: return "^";
                    case SyExpr::BITNEG: return "~";
                    case SyExpr::BITAND: return "&";
                    case SyExpr::BITNAND: return "~&";
                    case SyExpr::BITNOR: return "~|";
                    case SyExpr::BITOR: return "|";
                    case SyExpr::BITEQUIV: return ""; //TBD
                    case SyExpr::REDAND:  return "&";
                    case SyExpr::REDNAND: return "~&";
                    case SyExpr::REDNOR: return "~|";
                    case SyExpr::REDOR: return "|";
                    case SyExpr::REDXOR: return "^";
                    case SyExpr::REDXNOR: return "~^";
                    case SyExpr::LSHIFT: return "<<";
                    case SyExpr::RSHIFT: return ">>";
                    case SyExpr::RRSHIFT: return ">>>";
                    case SyExpr::PLUS: return "+";
                    case SyExpr::MINUS: return "-";
                    case SyExpr::MULT: return "*";
                    case SyExpr::DIV: return "/";
                    case SyExpr::MOD: return "%";
                    case SyExpr::POW: return "**";
                    default: return "";
                }
            }
            virtual void repVCntvByConcat() {
            }
            /*
            bool isCtxtSigned() const {
                return csign;
            }
            const SyConst *getCtxtSize() const {
                return csize;
            }

            void setCtxtSign(bool s) {
                csign = s;
            }    
        private:
            bool csign;
            SyConst* csize;
            */
    };


    class SyUnary: public SyExpr
    {
        friend class SyOMVisitor;
        friend class SyExprMgr;
        friend class SyExprExtVisitor;
        public:
            SyUnary(const SyExpr *e, const SyOp o) :
                SyExpr(), expr(const_cast<SyExpr*>(e)), op(o) {
                }
            virtual ~SyUnary() {
            }
            const SyExpr* getExpr() const {
                return expr;
            }
            SyOp getOperator() const {
                return op;
            }
            virtual std::string toString() const {
                return std::string("(") + getOpString(op) + 
                    expr->toString() + std::string(")");
            }

            virtual SyUnary *copy() const {
                return new SyUnary(expr->copy(), op);
            }
            virtual bool isSigned() const {
                return expr->isSigned();
            }

            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            virtual bool contains(const std::string&) const;
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;

            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
            virtual SyList<SyScCntv*>* toGate(SyModule* mod);

            void setExpr(const SyExpr *e) {
                expr = const_cast<SyExpr*>(e);
            }    
            virtual void removeVal();
            virtual void repVCntvByConcat();
        private:
            SyList<SyScPin*>* toUminus(SyModule*, SyList<SyScCntv*>*) const;
            SyExpr *expr;
            SyOp op;
    };

    class SyBinary: public SyExpr
    {
        public:
            friend class SyExprMgr;
            friend class SyOMVisitor;
            friend class SyExprExtVisitor;
        public:
            SyBinary(
                    const SyExpr *le,
                    const SyExpr *re,
                    const SyOp o) :
                SyExpr(),
                lexpr(const_cast<SyExpr*>(le)),
                rexpr(const_cast<SyExpr*>(re)),
                op(o) {
                }
            virtual ~SyBinary() {
            }

            const SyExpr *getLExpr() const {
                return lexpr;
            }
            const SyExpr *getRExpr() const {
                return rexpr;
            }
            SyOp getOperator() const {
                return op;
            }
            virtual std::string toString() const {
                return std::string("(") + lexpr->toString() +
                    getOpString(op) + rexpr->toString() + std::string(")");
            }

            virtual SyBinary *copy() const {
                return new SyBinary(lexpr->copy(), rexpr->copy(), op);
            }

            virtual bool isSigned() const; 
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            virtual bool contains(const std::string&) const;
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
            virtual SyList<SyScCntv*>* toGate(SyModule* mod){
                return NULL;
            }

            bool isShiftExpr() const;

            void setLExpr(const SyExpr *le) {
                lexpr = const_cast<SyExpr*>(le);
            }

            void setRExpr(const SyExpr *re) {
                rexpr = const_cast<SyExpr*>(re);
            }    
            virtual void removeVal(); 
            virtual void repVCntvByConcat();

        private:
            SyExpr *lexpr;
            SyExpr *rexpr;
            SyOp op;
    };

    class SyCondExpr: public SyExpr
    {
        friend class SyOMVisitor;
        friend class SyExprMgr;
        friend class SyExprExtVisitor;
        public:
            SyCondExpr(
                    const SyExpr *c,
                    const SyExpr *t,
                    const SyExpr *f):
                SyExpr(),
                cond(const_cast<SyExpr*>(c)),
                texpr(const_cast<SyExpr*>(t)),
                fexpr(const_cast<SyExpr*>(f)){
                }

            virtual ~SyCondExpr() {
            }

            const SyExpr *getCond() const {
                return cond;
            }
            const SyExpr *getTrueExpr() const {
                return texpr;
            }
            const SyExpr *getFalseExpr() const {
                return fexpr;
            }

            virtual std::string toString() const {
                return cond->toString() + std::string(" ? ") +
                    texpr->toString() + std::string(" : ") +
                    fexpr->toString();
            }

            virtual SyCondExpr *copy() const {
                return new SyCondExpr(cond->copy(), texpr->copy(), 
                        fexpr->copy());
            }

            virtual bool isSigned() const {
                return cond->isSigned() && texpr->isSigned() &&
                    fexpr->isSigned();
            }
            virtual void repSubExpr(
                    const std::string&, 
                    const SyExpr*);
            virtual SyConst* evaluate() const;
            virtual SyConst* size() const;
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
            virtual SyList<SyScCntv*>* toGate(SyModule* mod){
                return NULL;
            }
            virtual bool contains(const std::string&) const;

            void setCond(const SyExpr* c) {
                cond = const_cast<SyExpr*>(c);
            }

            void setTrueExpr(const SyExpr *te) {
                texpr = const_cast<SyExpr*>(te);
            }    
            void setFalseExpr(const SyExpr *fe) {
                fexpr = const_cast<SyExpr*>(fe);
            }    
            virtual void removeVal(); 
            virtual void repVCntvByConcat();
        private:
            SyExpr *cond;
            SyExpr *texpr;
            SyExpr *fexpr;
    };

    class SyFuncCall: public SyExpr
    {
        friend class SyExprMgr;
        friend class SyExprExtVisitor;
        public:
            SyFuncCall( const SyTaskOrFuncCall*);
            virtual ~SyFuncCall(); 

            const SyTaskOrFuncCall *getFuncCall() const {
                return funcCall;
            }
            const SyFunc *getFunc() const; 
            SyFunc *getFunc(); 
            void setFunc(const SyFunc*);
            void addArg(const SyExpr* arg); 
            const std::vector<SyExpr*>& getArgs() const; 



            virtual std::string toString() const;
            virtual SyFuncCall *copy() const; 
            virtual bool isSigned() const;
            virtual void repSubExpr(
                    const std::string& str, 
                    const SyExpr* expr);
            virtual void accept(SyVisitorBase*) const;
            virtual void accept(TVisitor*, SyModule*);
            virtual SyList<SyScCntv*>* toGate(SyModule* mod){
                return NULL;
            }
            virtual SyConst* size() const;

            operator const SyTaskOrFuncCall* () const {
                return funcCall;
            }    

            const std::string& getName() const; 
            virtual bool contains(const std::string&) const;
            virtual void removeVal(); 
        private:
            SyTaskOrFuncCall *funcCall;
            //SyFunc *func;
    };


    class SyVString: public SyExpr
    {
        public:
            SyVString(const std::string& str):
                vstr(str) {
                }

            virtual ~SyVString() {
            }
            virtual bool isSigned() const {
                return true;
            }
            virtual SyVString* copy() const {
                return new SyVString(vstr);
            }
            virtual std::string toString() const {
                return vstr;
            }
            virtual void accept(SyVisitorBase*) const;
            virtual bool contains(const std::string&) const;
            SyConcat *toConcat() const;
            SyConst *toConst() const;

        private:
            std::string vstr;
    };







}
#endif
