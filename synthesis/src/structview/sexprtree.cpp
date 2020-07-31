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

#include "sexprtree.h"

#include "spn.h"

namespace stview
{
    class SMemMgr
    {
        public:
            typedef std::map<const SPN*, std::pair<SNode*, SNode*> > NodeTable; //second in the pair is for inversion
        public: 
            SMemMgr() { }
            ~SMemMgr() { }
            SNode* createNode(
                SNode::Type type, 
                const SPN *data = 0, 
                SNode *left = 0, 
                SNode *right = 0)
            {
                if(type == SNode::ONE) {
                    return SNode::tautology();
                }    

                if(type == SNode::ZERO) {
                    return SNode::absurdity();
                }

                if(data && type == SNode::LITRL)
                {
                    return createLitNode(data);
                }    
                if(type == SNode::NOT || type == SNode::INVL)
                {
                    return createInvNode(data, left);
                }    
                return new SNode(type, data, left, right);
                
            }
            SNode* createNode(const SPN *spn) {
                return createNode(SNode::LITRL, spn);
            }    
            void deleteNode(SNode *node)
            {
                if(!node || node->m_type == SNode::ZERO || 
                    node->m_type == SNode::ONE ||
                    node->m_type == SNode::INVL ||
                    node->m_type == SNode::LITRL) {
                    return;
                }

                delete node;
            }    
            SNode* nodeCopy(SNode *node)
            {
                if(!node) {
                    return 0;
                }    
                if(node->m_type == SNode::ZERO || 
                    node->m_type == SNode::ONE ||
                    node->m_type == SNode::INVL ||
                    node->m_type == SNode::LITRL) {
                    return node;
                }    
                return new SNode(node->m_type, node->m_data);
            }    
            SNode* subTreeCopy(SNode* node)
            {
                if(!node) {
                    return 0;
                }    
                SNode *tmp = nodeCopy(node);
                tmp->m_left = subTreeCopy(node->m_left);
                tmp->m_right = subTreeCopy(node->m_right);
                return tmp;
            }
            void subTreeDelete(SNode *node)
            {
                if(!node) {
                    return;
                }    
                subTreeDelete(node->m_left);
                deleteNode(node->m_left);
                subTreeDelete(node->m_right);
                deleteNode(node->m_right);
           }

           void clear() { m_nodeTable.clear(); }
           SNode* getComplement(const SNode* node) 
           {
                if(!node) {
                    return 0;
                }    
                if(node->m_type == SNode::ZERO) {
                    return SNode::s_one;
                }    
                if(node->m_type == SNode::ONE) {
                    return SNode::s_zero;
                }    
                if(node->m_type == SNode::LITRL ||
                   node->m_type == SNode::INVL)
                {   
                    NodeTable::iterator iter = m_nodeTable.find(node->m_data);
                    assert(iter != m_nodeTable.end());
                    if(node->m_type == SNode::LITRL) {
                        return iter->second.second;
                    }
                    return iter->second.first;
                }
                return 0;
            }    
        private:    
            SNode *createLitNode(const SPN* data)
            {
                NodeTable::iterator iter = m_nodeTable.find(data);
                if(iter != m_nodeTable.end()) {
                     return iter->second.first;
                } 
                std::pair<SNode*, SNode*> p = createLitInvNode(data);
                m_nodeTable.insert(std::make_pair(data, p));
                return p.first;
            }

            SNode *createInvNode(const SPN* data, SNode *left)
            {
                if(left)
                {
                    switch(left->m_type)
                    {
                        case SNode::ZERO:
                        case SNode::ONE:
                        case SNode::LITRL:
                        case SNode::INVL:
                            return getComplement(left);
                        default:
                            break;
                    }
                }

                if(!data) {
                    return new SNode(SNode::NOT, 0, left);
                }
                NodeTable::iterator iter = m_nodeTable.find(data);
                if(iter != m_nodeTable.end()) {
                    return iter->second.second;
                } 
                std::pair<SNode*, SNode*> p = createLitInvNode(data);
                m_nodeTable.insert(std::make_pair(data, p));
                return p.second;

            }    

            std::pair<SNode*, SNode*> createLitInvNode(const SPN* data)
            {
                std::pair<SNode*, SNode*> p;
                p.first = new SNode(SNode::LITRL, data);
                p.second = new SNode(SNode::INVL, data);
                return p;
            }
            SMemMgr(const SMemMgr&);
            SMemMgr& operator=(const SMemMgr&);
            NodeTable m_nodeTable;
    };

    static SMemMgr s_memMgr;

    ///////////////////////////////////////////////////////////////////////
    // class : SNode
    ///////////////////////////////////////////////////////////////////////

    SNode* SNode::s_one = new SNode(ONE);
    SNode* SNode::s_zero = new SNode(ZERO);

    SNode::SNode(Type type, const SPN *data, SNode *left, SNode *right) :
        m_type(type), m_data(data), m_left(left), m_right(right) {
    }    


    SNode::SNode(const SPN *spn) : 
        m_type(LITRL), m_data(spn), m_left(0), m_right(0) {
    }


    SNode::~SNode() {
    }

    bool
    SNode::operator==(const SNode& rhs) const
    {
        if(m_type != rhs.m_type ||
            m_data != rhs.m_data) {
            return false;
        }    

        if(m_left && !rhs.m_left) {
            return false;
        }    
        if(m_right && !rhs.m_right) {
            return false;
        }    

        bool status1 = true;
        bool status2 = true;

        if(m_left) {
            status1 = *m_left == *rhs.m_left;
        }
        if(m_right) {
            status2 = *m_right == *rhs.m_right;
        }
        return status1 && status2;
    }
    
    bool
    SNode::operator!=(const SNode& rhs) const {
        return !(*this == rhs);
    }    

    SNode*
    SNode::operator!() {
        return this->invert(false);
    }    

    SNode*
    SNode::operator *()
    {
        if(m_left) {
            **m_left;
        }    
        if(m_right) {
            **m_right;
        }    
        if(m_type == AND) 
        {
            product(this, m_left, m_right);
            return this;
        }    
        return this;
    }  

    void
    SNode::product(SNode *parent, SNode *lchild, SNode* rchild)
    {
        //right subtree is to be duplicated
        if(!parent || !lchild || !rchild){
            return;
        }    
        while(lchild->m_type == BUF) 
        {
            //skip over buffers
            SNode *tmp = lchild;
            lchild = lchild->m_left;
            s_memMgr.deleteNode(tmp);
        }
        while(rchild->m_type == BUF) 
        {
            //skip over buffers
            SNode *tmp = rchild;
            rchild = rchild->m_left;
            s_memMgr.deleteNode(tmp);
        }
        parent->m_left = lchild;
        parent->m_right = rchild;

        if(rchild->m_type == OR) 
        {
            std::swap(lchild, rchild);
            std::swap(parent->m_left, parent->m_right);
        }    

        if(lchild->m_type == OR)
        {
            SNode *left = s_memMgr.createNode(AND, 0, s_memMgr.subTreeCopy(rchild), lchild->m_left);
            SNode *right = s_memMgr.createNode(AND, 0, rchild, lchild->m_right);
            lchild->m_left = left;
            lchild->m_right = right;
            product(lchild->m_left, lchild->m_left->m_left, lchild->m_left->m_right);
            if(lchild->m_right) {
                product(lchild->m_right, lchild->m_right->m_left, lchild->m_right->m_right);
            }    
            
            parent->m_type = BUF;
            parent->m_right = 0;
            parent->m_left = simplify(parent->m_left);
        }

    }

    SNode*
    SNode::rmbufs(SNode *node)
    {
        if(!node) {
            return 0;
        }
        if(node->m_left && node->m_left->m_type == BUF)
        {
            SNode *tmp = node->m_left;
            node->m_left = tmp->m_left;
            s_memMgr.deleteNode(tmp);
        }
        if(node->m_right && node->m_right->m_type == BUF)
        {
            SNode *tmp = node->m_right;
            node->m_right = tmp->m_left;
            s_memMgr.deleteNode(tmp);
        }
        node->m_left = rmbufs(node->m_left);
        node->m_right = rmbufs(node->m_right);
        return node;
    }

    SNode*
    SNode::simplify(SNode *node)
    {
        node = rmbufs(node);
        if(!node) {
            return 0;
        }

        if(node->m_type == AND) {
            return simplify2(node);
        }

        if(node->m_type == OR)
        {
            SNode *left = simplify(node->m_left);
            SNode *right = simplify(node->m_right);

            if(right->m_type == ZERO) 
            {
                s_memMgr.deleteNode(node);
                return left;
            }
            else if(left->m_type == ZERO) 
            {
                s_memMgr.deleteNode(node);
                return right;
            }
        }
        return node;
    }

    SNode*
    SNode::simplify2(SNode *node)
    {
        assert(node);
        InfoSet infoSet;
        populateLiteralInfo(node, infoSet);

        bool zero = false;
        bool chngreq = false;
        for(InfoSet::iterator iter = infoSet.begin();
            iter != infoSet.end(); ++iter)
        {
            if(iter->m_invcount > 0 && iter->m_bufcount > 0)
            {
                zero = true;
                break;
            }
            if(iter->m_invcount > 1 || 
                iter->m_bufcount > 1) {
                chngreq = true;
            }    
        }
        if(zero) 
        {
            s_memMgr.subTreeDelete(node);
            s_memMgr.deleteNode(node);
            return s_zero;
        }
        if(!chngreq) {
            return node;
        }    
        s_memMgr.subTreeDelete(node);
        s_memMgr.deleteNode(node);
        InfoSet::iterator iter = infoSet.begin();
        if(iter->m_invcount % 2) {
            node = s_memMgr.createNode( INVL, iter->m_data);
        }
        else {
            node = s_memMgr.createNode(iter->m_data);
        }    
        ++iter;

        for(; iter != infoSet.end(); ++iter)
        {
            if(iter->m_invcount % 2) {
                node = s_memMgr.createNode(AND, 0, node, 
                    s_memMgr.createNode(INVL, iter->m_data));
            }
            else {
                node = s_memMgr.createNode(AND, 0, node, 
                    s_memMgr.createNode(iter->m_data));
            }
        }
        return node;
    }

    void    
    SNode::populateLiteralInfo(SNode *node, InfoSet& infoSet)
    {
        if(!node) {
            return;
        }    

        if(node->m_type == INVL)
        {
            LiteralInfo tmp(node->m_data);
            InfoSet::iterator info = infoSet.find(tmp);
            if(info == infoSet.end())
            {
                LiteralInfo tmp(node->m_data);
                ++tmp.m_invcount;
                infoSet.insert(tmp);
            }
            else {
                ++(const_cast<LiteralInfo&>(*info).m_invcount);
            }    
        }
        else if(node->m_type == LITRL)
        {
            InfoSet::iterator info = infoSet.find(node->m_data);
            if(info == infoSet.end())
            {
                LiteralInfo tmp(node->m_data);
                ++tmp.m_bufcount;
                infoSet.insert(tmp);
            }
            else {
                ++(const_cast<LiteralInfo&>(*info).m_bufcount);
            }    
        }
        else
        {
            populateLiteralInfo(node->m_left, infoSet);
            populateLiteralInfo(node->m_right, infoSet);
        }    
    }


    SNode*
    SNode::invert(bool inv)
    {
        //Operates on inversions and pushes them to the
        //leaves so that they become part of the literal
        switch(m_type)
        {
            case ZERO:
            case ONE: 
            case LITRL:
            case INVL:
                if(inv) {
                    return s_memMgr.getComplement(this);
                }
                return this;
            case NOT:
                inv = !inv;
                return this->m_left->invert(inv);
            case OR:    
                if(inv) {
                    m_type = AND;
                }
                m_left = m_left->invert(inv);
                m_right = m_right->invert(inv);
                break;
            case AND:    
                if(inv) {
                   m_type = OR;
                }
                m_left = m_left->invert(inv);
                m_right = m_right->invert(inv);
                break;
            default:
                break;
        }
        return this;
    }


    std::string
    SNode::toString() const
    {
        switch(m_type)
        {
            case ZERO:
                return "1'b0";
            case ONE:
                return "1'b1";
            case LITRL:
                return m_data->getIxName().toString();
            case INVL:
                return "~" + m_data->getIxName().toString();
            case BUF:
                if(m_left)
                {
                    assert(!m_right);
                    return m_left->toString();
                }
                if(m_right)
                {
                    assert(!m_left);
                    return m_right->toString();
                }
                return " BUF ";
            case NOT:
                assert(!m_right);
                return "~" + m_left->toString(); 
            case OR:    
                assert(m_right);
                assert(m_left);
                return "(" + m_left->toString() + "|" + m_right->toString() + ")"; 
            case AND:    
                assert(m_right);
                assert(m_left);
                return "(" + m_left->toString() + "&" + m_right->toString() + ")"; 
            default:
                assert(0);
        }
        
    }
    ///////////////////////////////////////////////////////////////////////
    // End class : SNode
    ///////////////////////////////////////////////////////////////////////

    
    ///////////////////////////////////////////////////////////////////////
    // class : SExprTree
    ///////////////////////////////////////////////////////////////////////

    SExprTree::SExprTree(
        SNode::Type type, 
        const SPN *data, 
        SNode *left, 
        SNode *right) :
        m_root(s_memMgr.createNode(type, data, left, right)) {
    }    

    SExprTree::~SExprTree()
    {
    }

    SExprTree::SExprTree(SNode *root) : m_root(root) {
    }

    SNode*
    SExprTree::operator()()
    {
        sop();
        return m_root;
    }

    bool
    SExprTree::operator==(const SExprTree& rhs) const
    {
        const SNode *other = rhs.getRoot();
        if(!other && !m_root) {
            return true;
        }

        if(!other || !m_root) {
            return false;
        }
        return *other == *m_root;
    }

    bool
    SExprTree::operator!=(const SExprTree& rhs) const {
        return !(*this == rhs);
    }    
    void
    SExprTree::sop()
    {
        if(!m_root) {
            return;
        }

        m_root = !*m_root;
        **m_root;
        //print();
    }

    bool    
    SExprTree::addNode(SNode *node, SNode *toBeAdded) 
    {
        if(!node || (node->m_left && node->m_right)) {
            return false;
        }
        if(!node->m_left) {
            node->m_left = toBeAdded;
        }
        else {
            node->m_right = toBeAdded;
        }
        return true;
    }

    bool    
    SExprTree::addNode(SNode *node, const SPN *data) 
    {
        SNode *toBeAdded = s_memMgr.createNode(data);

        if(!addNode(node, toBeAdded)) 
        {
            s_memMgr.deleteNode(toBeAdded);
            return false;
        }
        return true;
    }

    SNode*
    SExprTree::createNode(SNode::Type type,
        const SPN *data, 
        SNode *left, 
        SNode *right
        ) {
        return s_memMgr.createNode(type, data, left, right);
    }    
    
    SNode*
    SExprTree::createNode(const SPN *data) {
        return s_memMgr.createNode(data);
    }    


    const SNode*
    SExprTree::getRoot() const {
        return m_root;
    }    

    SNode*
    SExprTree::getRoot() {
        return m_root;
    }    

    void
    SExprTree::init() {
        s_memMgr.clear();
    }    
    void
    SExprTree::clear()
    {
        if(!m_root) {
            return;
        }
        s_memMgr.subTreeDelete(m_root);
        s_memMgr.deleteNode(m_root);
        m_root = 0;
        s_memMgr.clear();
    }

    SNode*
    SExprTree::subTreeCopy(SNode *node){
        return s_memMgr.subTreeCopy(node);
    }

    void    
    SExprTree::subTreeDelete(SNode* node){
        return s_memMgr.subTreeDelete(node);
    }
    std::string
    SExprTree::toString() const {
        return m_root ? m_root->toString() : "";
    }

}
