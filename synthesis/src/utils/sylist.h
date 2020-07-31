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

#ifndef _SYLIST_H_
#define _SYLIST_H_
/***********************************************************************
  * Author: Parvez Ahmad
  * Start Date: 13/05/2009
  * This is light weight general purpose list structure. We are not using
  * STL list purely for speed and space reason.
*************************************************************************/
#include <algorithm>

namespace Synth
{
    template <class T>
    struct SyNode
    {
        T val;
        struct SyNode* next;
    };

    template <class T>
    class SyList
    {
        public:
            SyList();
            explicit SyList(const T& t);
            SyList(const SyList<T>&);
            ~SyList();
            SyList<T>& operator=(const SyList<T>&);
            inline void push_back(const T& t);
            inline void push_front(const T& t);
            inline bool remove(const T& t, const bool all=true);
            inline bool remove_front();
            inline bool remove_back();
            inline void splice(const SyList<T>*);
            inline void merge(const SyList<T>&);
            inline void reverse();
            inline void clear();
            int size() const 
            {
                int s = 0;
                for(struct SyNode<T>* tmp = head; tmp; tmp = tmp->next) ++s;
                return s;
            }
            const T& front() const {
                //note no sanity checks
                return head->val;
            }
            const T& last() const {
                return tail->val;
            }
            bool empty() const {
                return head == 0;
            }
        //private:
            struct SyNode<T>* head;
            struct SyNode<T>* tail;
    };

    //Constructor with no args, should be used judiciously
    template<class T>
    SyList<T>::SyList(): head((SyNode<T>*)0), tail((SyNode<T>*)0){
    }

    //Recommended constructor, construct with at least one value.
    template <class T>
    SyList<T>::SyList(const T& t)
    {
        head = new struct SyNode<T>();
        head->val = t;
        head->next = 0;
        tail = head;
    }
    template <class T>
    SyList<T>::SyList(const SyList<T>& lhs)
    {
        head = 0;
        tail = 0;
        *this = lhs;
    }

    template <class T>
    SyList<T>::~SyList<T>()
    {
        tail = head;
        while(tail)
        {
            struct SyNode<T>* tmp = tail->next;
            delete tail;
            tail = tmp;
        }
    }

    template <class T> SyList<T>& 
    SyList<T>::operator=(const SyList<T>& rhs)
    {
        struct SyNode<T>* thead = head;
        if(thead){
            thead = thead->next;
        }

        for(; thead; thead = thead->next)
        {
            delete head;
            head = thead;
        }
        if(rhs.size() == 0) 
        {
            head = 0;
            tail = 0;
            return *this;
        }
        head = new struct SyNode<T>();
        head->val = rhs.head->val;
        head->next = 0;
        tail = head;
        for(thead = rhs.head->next; thead; thead = thead->next)
        {
            tail->next = new struct SyNode<T>();
            tail = tail->next;
            tail->val = thead->val;
            tail->next = 0;
        }
        return *this;
    }

    template<class T> void
    SyList<T>::clear()
    {
        tail = head;
        while(tail)
        {
            struct SyNode<T>* tmp = tail->next;
            delete tail;
            tail = tmp;
        }
        head = 0;
        tail = 0;
    }

    template <class T> void
    SyList<T>::push_back(const T& t)
    {
        struct SyNode<T>* node = new struct SyNode<T>();
        node->val = const_cast<T&>(t);
        node->next = 0;
        if(!tail) {
            //list constructed via constuctor without arguments
            tail = node;
            head = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }

    template <class T> void
    SyList<T>::push_front(const T& t)
    {
        struct SyNode<T>* node = new struct SyNode<T>();
        node->val = const_cast<T&>(t);
        node->next = head;
        head = node;
        if(!tail){
            tail = head;
        }
    }

    template <class T> bool
    SyList<T>::remove(
        const T& t,
        const bool all)
    {
        //returns true if it is able to successfully remove.
        bool success = false;


        for(struct SyNode<T>* node = head; node;)
        {
            if(node->val == t)
            {
                if(node == head)
                {
                    head = head->next;
                    if(tail == node){
                        //list has one member only
                        tail = head;
                    }
                    delete node;
                    node = head;
                }
                else if(node == tail)
                {
                    struct SyNode<T>* tnode;
                    for(tnode = head; tnode->next != node; 
                        tnode = tnode->next);
                    tnode->next = 0;
                    delete node;
                    tail = tnode;
                    node = 0;
                }
                else 
                {
                    struct SyNode<T>* tnode;
                    for(tnode = head; tnode->next != node; 
                        tnode = tnode->next);
                    tnode->next = node->next;
                    delete node;
                    node = tnode->next;
                }
                if(!all){
                    return true;
                }
                success = true;
            }
            else {
                node = node->next;
            }
        }
        return success;
    }

    template <class T> bool
    SyList<T>::remove_front() {
        if(head) {
            return this->remove(head->val);
        }
        return false;
    }    

    template <class T> bool
    SyList<T>::remove_back() {
        if(tail) {
            return this->remove(tail->val);
        }
        return false;
    }    

    template <class T> void
    SyList<T>::splice(const SyList<T>* tobeconctd)
    {
        if(!tobeconctd){
            return;
        }
        if(!tail)
        {
            head = tobeconctd->head;
            tail = tobeconctd->tail;
        }
        else
        {
            this->tail->next = tobeconctd->head;
            this->tail = tobeconctd->tail;
        }
    }

    template <class T> void
    SyList<T>::merge(const SyList<T>& tobeconctd)
    {
        for(SyNode<T>* node = tobeconctd.head; node; node = node->next){
            this->push_back(node->val);
        }
    }    
        
    template <class T> void
    SyList<T>::reverse()
    {
        if(this->size() <= 1) {
            return;
        }    
        if(this->size() == 2) {
            head->next = 0;
            tail->next = head;
            SyNode<T>* tmp = head;
            head = tail;
            tail = tmp;
            return;
        }    
        SyNode<T>* n1 = head; 
        SyNode<T>* n2 = head->next; 
        n1->next = 0;

        while(n2)
        {
            SyNode<T>* n3 = n2->next;
            n2->next = n1;
            n1 = n2;
            n2 = n3;
        }    
        tail = head;
        head = n1;
    }
}

 


#endif
