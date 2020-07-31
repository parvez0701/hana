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

#ifndef _SYGRAPH_H_
#define _SYGRAPH_H_

namespace Synth
{
    //Generic graph node
    template <class T1, class T2=int>
        class SyGraphNode
        {
            public:
                SyGraphNode():{
                    edges = new SyList<SyEdge*>();
                }
                SyGraphNode(const T1& t): 
                    val(const_cast<T1&>(t)) {
                    edges = new SyList<SyEdge*>();
                    }
                            
            //private:
                T1     val;
                T2     attrib;
                SyList<SyEdge*>* edges;
        };

    template <class T1, class T2=int>
        class SyEdge
        {
            public:
                SyEdge(): node1(0), node2(0) {
                }
                SyEdge(const SyGraphNode<T1, T2>* n1,
                        const SyGraphNode<T1, T2>* n2)
                {
                    node1 = const_cast<SyGraphNode<T1, T2>*>(n1);
                    node2 = const_cast<SyGraphNode<T1, T2>*>(n2);
                }
                SyGraphNode* first() const {
                    return node1;
                }
                SyGraphNode* second const {
                    return node2;
                }
            private:
                //In case of directed graph this is source node
                SyGraphNode<T1, T2>* node1;
                //In case of directed graph this is destination node
                SyGraphNode<T1, T2>* node2;
        };

    template <class T1, class T2=int>
        class SyGraph
        {
            public:
                SyGraph();
                void addStartNode(const T1& t)
                {
                    if(!startNodes){
                        startNodes = new SyList<SyGraphNode<T1, T2>*>();
                    }
                    SyGraphNode<T1, T2>* node = 
                        new SyGraphNode<T1, T2>(t);
                    SyEdge* edge = new SyEdge(node, 0);
                    node->edges->push_back(edge);
                    startNodes->push_back(node);
                }
                SyList<SyGraphNode*>* getStartNodes() const {
                    return startNodes;
                }
            private:
                SyList<SyGraphNode<T1, T2>*>* startNodes;
        };
}
#endif
