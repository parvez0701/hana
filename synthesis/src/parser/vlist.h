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

#ifndef _VLIST_H_
#define _VLIST_H_

/*
    * Light weight unidirectional linked list and iterator class
    * Author: Parvez Ahmad
    * Date: 12 April 2009
*/
template <class T> class List;
template <class T> class Iterator;
template < class T >
class Link
{
    friend class List<T>;
    friend class Iterator<T>;
    public:
        Link(T& t): val(t), next(0){
        }

    private:
        Link();
        Link(const Link&);
        ~Link(){ delete next; }
        const Link& operator=(const Link&);
        T val;
        Link* next;
};
template < class T >
class Iterator
{
    public:
    ~Iterator() {
    }
    bool operator==(const Iterator<T>& rhs){
        return link == rhs.link;
    }
    bool operator !=(const Iterator<T>& rhs){
        return !(*this == rhs);
    }
    const Iterator& operator++(){
        if(!link){
          return *this;
            //List<T>::Enditer;
        }
        link = link->next;
        return *this;
    }
    const Iterator& operator++(const int i){
        if(!link){
            return *this;
        }
        link = link->next;
        return *this;
    }
    T* operator->() const {
        return &(link->val);
    }

    T& operator*() const {
        return link->val;
    }

    private:
        Iterator(Link<T>* l): link(l){
        }
        Iterator();
        Iterator(const Iterator&);
        const Iterator& operator=(const Iterator&);
        Link<T>* link;
};


template <class T>
class List
{
    friend class Iterator<T>;
    public:
        typedef Iterator<T> iterator;
        List(): /*count(0), */head(0), tail(0){
        }

        List(const List& rhs) :
            /*count(rhs.count),*/ head(rhs.head), tail(rhs.tail){
        }

        ~List(){
            Link<T>* plink = head;
            while(head){
                head = head->next;
                delete plink;
                plink = head;
            }
        }

        const List& operator=(const List& rhs)
        {
            //count = rhs.count;
            head = rhs.head;
            tail = rhs.tail;
        }
/*
        T& operator[](const long index) const throw(Error){
            //It follows array style index counting
            if(index < 0 || index >= count){
                THROW_ERROR_FLAG("Index out of range.");
            }
            Link* link = head;
            for(int i = 0; i <= index; ++i){
                link = link->next;
            }
            return link->val;
        }
*/
        bool empty() const {
            return head == 0;
        }
        int size() const {
            int s = 0;
            Link<T>* link = head;
            while(link){
                ++s;
                link = link->next;
            }
            return s;
            //return count;
        }
        iterator begin() const {
            return iterator(head);
        }
        iterator end() const {
            return eiter;
        }
        void push_back(T& t){
            Link<T>* link = new Link<T>(t);
            if(head == 0){
                head = link;
                tail = link;
            }
            else {
                tail->next = link;
            }
            //++count;
        }

        void push_front(T& t){
            Link<T>* link = new Link<T>(t);
            if(head == 0){
                head = link;
                tail = link;
            }
            else {
              link->next = head;
              head = link;
            }
            //++count;
        }

        T& front() const{
            return head->val;
        }

        void remove(T& t){
            Link<T>* plink = head;
            if(plink->val == t){
                head = head->next;
                //--count;
                delete plink;
                return;
            }
            for(Link<T>* link = plink->next; link != tail; link = link->next){
                if(link->val == t){
                    plink->next = link->next;
                    delete link;
                 //   --count;
                    return;
                }
            }
        }
    private:
        //long count;
        Link<T>* head;
        Link<T>* tail;
        static iterator eiter;
};

#endif
