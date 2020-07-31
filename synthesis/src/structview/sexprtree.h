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

#ifndef _SEXPRTREE_H_
#define _SEXPRTREE_H_

#include <string>
#include <set>

namespace stview
{
    class SPN;
                                
    struct LiteralInfo
    {
        LiteralInfo(const SPN *data) :
            m_data(data), m_invcount(0), m_bufcount(0) {
        }

        bool operator < (const LiteralInfo& rhs) const {
            return m_data < rhs.m_data;
        }    
        const SPN *m_data;
        int m_invcount;
        int m_bufcount;
    };

    class SNode
    {
        friend class SExprTree;
        friend class SMemMgr;;
        typedef std::set<LiteralInfo> InfoSet;
        public:
            typedef enum { BUF, ZERO, ONE, LITRL, INVL, NOT, OR, AND } Type;
            
            static SNode* tautology() { return s_one; }
            static SNode* absurdity() { return s_zero; }

            std::string toString() const;

        private:    

            explicit SNode(Type type, const SPN *data = 0, SNode *left = 0, SNode *right = 0);
            explicit SNode(const SPN *spn);
            ~SNode();
            SNode(const SNode&);

            SNode& operator=(const SNode&);
            bool operator==(const SNode& rhs) const;
            bool operator!=(const SNode& rhs) const;

            SNode* operator!();
            SNode* operator *();
            static void product(SNode*, SNode*, SNode*);
            static SNode* rmbufs(SNode*);
            
            static SNode* simplify(SNode*);
            static SNode* simplify2(SNode*);

            SNode* invert(bool);
            static void populateLiteralInfo(SNode*, InfoSet&);

            
            //data
            Type m_type;
            const SPN *m_data;
            SNode *m_left;
            SNode *m_right;

            static SNode *s_one;
            static SNode *s_zero;
    };

    class SExprTree
    {
        public:
            
        public:
            explicit SExprTree(
                SNode::Type type, 
                const SPN *data = 0, 
                SNode *left = 0, 
                SNode *right = 0);
            explicit SExprTree(const SPN *spn);
            explicit SExprTree(SNode *);
            ~SExprTree();

            bool operator==(const SExprTree&) const;
            bool operator!=(const SExprTree&) const;
            SNode* operator()();

            void sop();


            static bool addNode(SNode *node, SNode *toBeAdded);
            static bool addNode(SNode *, const SPN*);
            static SNode* createNode(SNode::Type type, 
                const SPN *data = 0, 
                SNode *left = 0, 
                SNode *right = 0);
            static SNode* createNode(const SPN *spn);
            inline const SNode* getRoot() const;
            inline SNode* getRoot();
            


            static void init();
            void clear();
            static SNode* subTreeCopy(SNode* node);
            static void subTreeDelete(SNode* node);

            std::string toString() const;
        private:
            //No copying
            SExprTree(const SExprTree&);
            SExprTree& operator=(const SExprTree&);



            SNode *m_root;
    };

    typedef SNode SExpr;
}
#endif
