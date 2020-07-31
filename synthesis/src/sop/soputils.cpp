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
#if 0
#include "soputils.h"

#include "sexpr.h"
#include "sopexprtypevisitor.h"

using namespace stview;
namespace SOP
{

	static void literals(std::vector<SExpr*>& lits, SExpr* expr)
	{
	    //assumed to be in product form
	    if(expr->literal()) 
	    {
	        lits.push_back(expr);
	        return;
        }
        SopExprTypeVisitor v;
        expr->accept(&v);
        if(v.isAndExpr())
        {
            literals(lits, v.getAndExpr()->getLExpr());
            literals(lits, v.getAndExpr()->getRExpr());
        }
     }

	static bool isinverted(SExpr* expr) 
	{
        SopExprTypeVisitor v;
        expr->accept(&v);
		if(!v.isInvExpr()) {
		    return false;
		}	
	    return !isinverted(v.getInvExpr()->getExpr());
	}

	struct Cmp
	{
		bool operator()(const SExpr* e1, const SExpr* e2) const 
		{
		    return cmp_str()(e1->toString(), e2->toString());
		}
	};
	class Simplifier : public std::unary_function<SExpr*, SExpr*>
	{
	    public:
		    typedef std::map<SExpr*, std::pair<int, int>, Cmp> InfoMap;
		    SExpr* operator()(SExpr *expr) 
			{
			    if(!expr->prodterm()) {
				    return expr;
				}	
				std::vector<SExpr*> lits;
				literals(lits, expr);
				InfoMap infoMap;
				for(int i = 0; i < lits.size(); ++i)
				{
					bool inv = isinverted(lits[i]);
				    InfoMap::iterator iter = infoMap.find(lits[i]);
					if(iter == infoMap.end())
					{
						infoMap.insert(std::make_pair(literal(lits[i]),
						    std::make_pair(inv?0 : 1, inv ? 1 : 0)));
					}
					else {
					    if(inv) {
						    iter->second.second++;
						}
						else {
						    iter->second.first++;
						}
					}
				}
				SExpr *retexpr = 0;
				InfoMap::iterator info = infoMap.begin();

				for(; info != infoMap.end(); ++info)
				{
				    if(info->second.first && info->second.second)
					{
					    retexpr = Absurdity();
						break;
					}
					if(info->second.first)
					{
					    if(!retexpr) {
						    retexpr = info->first->copy();
						}
						else {
						    retexpr = new SAnd(retexpr, info->first);
						}	
					}
					else 
					{
					    if(!retexpr) {
						    retexpr = new SInv(info->first->copy());
						}
						else {
						    retexpr = new SAnd(retexpr, new SInv(info->first));
						}	
					}
				}
				return retexpr;
			}
	    private:
		    SExpr* literal(SExpr *expr)
			{
                SopExprTypeVisitor v;
		        expr->accept(&v);
				if(!v.isInvExpr()) {
				    return expr;
				}
				return literal(v.getInvExpr()->getExpr());
			}	
	};

	class SimplifySum : public std::unary_function<SExpr*, SExpr*>
	{
	    public:
		    SExpr* operator()(SExpr *expr)
			{
			    std::vector<SExpr*> prods;
				getProds(prods, expr);
				std::set<SExpr*, Cmp> infoSet;
				for(int i = 0; i < prods.size(); ++i)
				{
				    //Does not take care of logically equivalent
					//but out of order products
				    infoSet.insert(prods[i]); 
				}
				std::vector<SExpr*> redprods;
				std::set<SExpr*, Cmp>::iterator info = infoSet.begin();
				for(; info != infoSet.end(); ++info)
				{
				    redprods.push_back(*info);
				}
				std::vector<SExpr*> toberemoved;
				for(int i = 0; i < redprods.size(); ++i)
				{
				    for(int j = i+1; j < redprods.size(); ++j)
					{
					    if(contains(redprods[i], redprods[j])) {
						    toberemoved.push_back(redprods[j]);
						}
						else if(contains(redprods[j], redprods[i])) {
						    toberemoved.push_back(redprods[i]);
						}
					}
				}
				for(int i = 0; i < toberemoved.size(); ++i) {
				    infoSet.erase(toberemoved[i]);
				}	


				SExpr *retexpr = 0;
				info = infoSet.begin();
				for(; info != infoSet.end(); ++info)
				{
				    if(!retexpr) {
					    retexpr = *info;
					}
					else {
					    retexpr = new SOr(retexpr, *info);
					}	
				}
				//delete expr;
				return retexpr;
			}
	    private:
		    void getProds(std::vector<SExpr*>& prods, SExpr* expr) const
			{
			    if(expr->prodterm()) 
				{
				    prods.push_back(expr);
					return;
				}	
                SopExprTypeVisitor v;
		        expr->accept(&v);
				if(v.isOrExpr()) 
				{
				    getProds(prods, v.getOrExpr()->getLExpr());
				    getProds(prods, v.getOrExpr()->getRExpr());
				}
			}

			bool contains(SExpr *expr1, SExpr* expr2) 
			{
				std::vector<SExpr*> lits1;
				std::vector<SExpr*> lits2;
				literals(lits1, expr1);
				literals(lits2, expr2);
				std::set<SExpr*, Cmp> infoSet;

				for(int i = 0; i < lits1.size(); ++i) {
				    infoSet.insert(lits1[i]);
				}

				for(int i = 0; i < lits2.size(); ++i) {
				    if(infoSet.find(lits2[i]) == infoSet.end()) {
					    return false;
					}
				}	
				return true;	    
			}
	};

    bool operator==(const SExpr& expr1, const SExpr& expr2)
	{
	    if(expr1.toString() == expr2.toString()) {
		    return true;
		}	
        SopExprTypeVisitor v1;
        SopExprTypeVisitor v2;

		SExpr& e1 = const_cast<SExpr&>(expr1);
		SExpr& e2 = const_cast<SExpr&>(expr2);

        e1.accept(&v1);
        e2.accept(&v2);

		if(v1.isInvExpr() && v2.isInvExpr()) {
		    return *v1.getInvExpr()->getExpr() == 
			    *v2.getInvExpr()->getExpr();
		}
		if(v1.isAndExpr() && v2.isAndExpr())
		{
		    const SExpr *firstLExpr = v1.getAndExpr()->getLExpr();
		    const SExpr *firstRExpr = v1.getAndExpr()->getRExpr();


		    const SExpr *secondLExpr = v2.getAndExpr()->getLExpr();
		    const SExpr *secondRExpr = v2.getAndExpr()->getRExpr();

			if((*firstLExpr == *secondLExpr) &&
			    (*firstRExpr == *secondRExpr)) {
				return true;
			}	
			if((*firstLExpr == *secondRExpr) &&
			    (*firstRExpr == *secondLExpr)) {
				return true;
			}	
		}
		if(v1.isOrExpr() && v2.isOrExpr())
		{
		    const SExpr *firstLExpr = v1.getOrExpr()->getLExpr();
		    const SExpr *firstRExpr = v1.getOrExpr()->getRExpr();


		    const SExpr *secondLExpr = v2.getOrExpr()->getLExpr();
		    const SExpr *secondRExpr = v2.getOrExpr()->getRExpr();

			if((*firstLExpr == *secondLExpr) &&
			    (*firstRExpr == *secondRExpr)) {
				return true;
			}	
			if((*firstLExpr == *secondRExpr) &&
			    (*firstRExpr == *secondLExpr)) {
				return true;
			}	
		}
		return false;
	}

    bool operator!=(const SExpr &expr1, const SExpr &expr2)
	{
	    return !(expr1 == expr2); 
	}
    SOne*
    Tautology() {
        return new SOne();
    }

    SZero*
    Absurdity() {
        return new SZero();
    }    

    SExpr*
    Sum(SExpr *expr1, SExpr *expr2) 
	{
	    if(*expr1 == *expr2) {
		    // a|a
		    return expr1->copy();
		}	
        SopExprTypeVisitor v1;
        SopExprTypeVisitor v2;

        expr1->accept(&v1);
        expr2->accept(&v2);

		if(v1.isInvExpr() && !v2.isInvExpr()) {
		    //a|~a
		    if(*v1.getInvExpr()->getExpr() == *expr2) {
			    return Tautology();
			}
		}
		else if(!v1.isInvExpr() && v2.isInvExpr()) {
		    //a|~a
		    if(*expr1 == *v2.getInvExpr()->getExpr()) {
			    return Tautology();
			}
		}
		if(v1.isOneExpr() || v2.isOneExpr()) {
		    return Tautology();
		}
		if(v1.isZeroExpr() && v2.isZeroExpr()) {
		    return Absurdity();
		}
		if(v1.isZeroExpr()) {
		    return expr2->copy();
		}	
		if(v2.isZeroExpr()) {
		    return expr1->copy();
		}	
        return SimplifySum()(new SOr(expr1, expr2));
    }

    SExpr* 
    Product(SExpr *expr1, SExpr *expr2)
    {
	    if(*expr1 == *expr2) {
		    // a&a
		    return expr1->copy();
		}	
        SopExprTypeVisitor v1;
        SopExprTypeVisitor v2;

        expr1->accept(&v1);
        if(v1.isOrExpr()) {
            return ProductOr(v1.getOrExpr(), expr2);
        }

        expr2->accept(&v2);
        if(v2.isOrExpr()) {
            return ProductOr(v2.getOrExpr(), expr1);
        }

		if(v1.isInvExpr() && !v2.isInvExpr()) {
		    //a&~a
		    if(*v1.getInvExpr()->getExpr() == *expr2) {
			    return Absurdity();
			}
		}
		else if(!v1.isInvExpr() && v2.isInvExpr()) {
		    //a&~a
		    if(*expr1 == *v2.getInvExpr()->getExpr()) {
			    return Absurdity();
			}
		}
		if(v1.isZeroExpr() || v2.isZeroExpr()) {
		    return Absurdity();
		}
		if(v1.isOneExpr() && v2.isOneExpr()) {
		    return Tautology();
		}
		if(v1.isOneExpr()) {
		    return expr2->copy();
		}	
		if(v2.isOneExpr()) {
		    return expr1->copy();
		}	
        return Simplifier()( new SAnd(expr1, expr2) );
            
    }

    SExpr*
    ProductOr(SOr *orexpr, SExpr *expr)
    {
		SExpr *lexpr = Product(orexpr->getLExpr(), expr->copy());
		SExpr *rexpr = Product(orexpr->getRExpr(), expr->copy());
		//delete orexpr;
        return Sum(lexpr, rexpr);
    }

    SExpr*
    Complement(SExpr *expr)
    {
        SopExprTypeVisitor v;
        expr->accept(&v);

        if(v.isAndExpr())
        {
            SAnd *andexpr = v.getAndExpr();
            SExpr *lexpr = andexpr->getLExpr()->copy();
            SExpr *rexpr = andexpr->getRExpr()->copy();

            //delete andexpr;
            lexpr = Complement(lexpr);
            rexpr = Complement(rexpr);
            return Sum(lexpr, rexpr);
        }
        else if(v.isOrExpr())
        {
            SOr *orexpr = v.getOrExpr();
            SExpr *lexpr = orexpr->getLExpr();
            SExpr *rexpr = orexpr->getRExpr();

            //delete orexpr;
            lexpr = Complement(lexpr);
            rexpr = Complement(rexpr);
            return Product(lexpr, rexpr);
        }
        else if(v.isInvExpr())
        {
            SInv *invexpr = v.getInvExpr();
            SExpr *texpr = invexpr->getExpr()->copy();
            //delete invexpr;
            return texpr;
        }
        else if(v.isOneExpr())
        {
            //delete v.getOneExpr();
            return new SZero();
        }
        else if(v.isZeroExpr())
        {
            //delete v.getZeroExpr();
            return new SOne();
        }

        return new SInv(expr);

    }

}
#endif
