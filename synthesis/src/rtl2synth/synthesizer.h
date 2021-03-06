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

#ifndef _SYNTHESIZER_H_
#define _SYNTHESIZER_H_

#include "syutils.h"
#include "rtl2netlist.h"

namespace Synth
{
    class Synthesizer
    {
        public:
            ~Synthesizer();
            static Synthesizer* instance();
            void synthesize();


            bool         isModuleSynthesized(const std::string&) const;
            SyModule*    getSynthModule(const std::string&) const;
        private:
            Synthesizer();
            DISABLE_COPYCONST_ASSGNOP(Synthesizer);

            void synthesize(const SyModule*);
            
            //data
            static Synthesizer *synthesizer;
            Rtl2Netlist rtl2Netlist;
    };
}
#endif
