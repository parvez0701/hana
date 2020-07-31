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

#ifndef _SROOT_H_
#define _SROOT_H_

#include <list>
#include <map>

#include <string>

#include "sutils.h"


namespace Synth {
    class SyRoot;
}    
namespace stview
{
    class SBlock;
    class SRoot
    {
        public:
            static SRoot* instance();
            //Not expected any derivation from class
            ~SRoot();
            //Initializes the structural view from Rtl root.
            void initialize(const Synth::SyRoot*);

            //A block corresponds to a module definition
            const SBlock*    getBlock(const std::string&) const;
            SBlock*            getBlock(const std::string&);
            void            addBlock(const SBlock*);
            inline const std::list<SBlock*>& getBlocks() const;
            const std::list<SBlock*>& getTopBlocks() const;

            void            resetVisitCount();
        private:
            SRoot();
            SRoot(const SRoot&);
            SRoot& operator=(const SRoot&);

            static SRoot *root;

            //Blocks corresponding to top modules
            mutable std::list<SBlock*> topBlocks;
            //Blocks as a result of partitionning
            std::list<SBlock*> blocks;
            std::map<std::string, SBlock*, cmp_str> blockTable;
    };


    const std::list<SBlock*>&
    SRoot::getBlocks() const{
        return blocks;
    }



}
#endif
