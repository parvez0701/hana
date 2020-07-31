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

#include "symsghandler.h"


using std::string;
using std::ofstream;
using std::ostringstream;
using std::cout;
using std::pair;
using std::make_pair;

using Rtl::MsgInfo;
namespace Synth
{

    SyMsgHandler::SyMsgHandler()  {
        this->buildMsgTable();
    }

    SyMsgHandler::~SyMsgHandler() {
        this->close();
    }

    void
        SyMsgHandler::buildMsgTable()
        {
            addMsgInfo(new MsgInfo("[SYNTH_INFO 0]: Starting synthesis of the design .....\n",
                        0, MsgInfo::IMM, MsgInfo::INFO));
            addMsgInfo(new MsgInfo("[SYNTH_INFO 1]: Doing synthesizability check .....\n",
                        1, MsgInfo::IMM, MsgInfo::INFO));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 2]: Line %d Module %s not synthesizable because the presence of %s.\n",
                        2, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 3]: In module %s gate type %s will be ignored for synthesis.\n",
                        3, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 4]: In file %s UDP will be ignored for synthesis.\n",
                        4, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 5]: Line %d Module %s is not synthesizable because loop index is assigned non-const.\n",
                        5, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 6]: Can not open file %s for writing, stdout will be used for output.\n",
                        6, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 7]: Line %d module %s is not synthesizable because always is without sensitivity list\n",
                        7, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 8]: Line %d module %s is not synthesizable, the sensitivity list contains both level as well as posedge events.\n",
                        8, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 9]: Line %d module %s is not synthesizable, the sensitivity list contains both level as well as negedge events.\n",
                        9, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 10]: Line %d module %s is not synthesizable, the sensitivity list specified is beyond the scope of this tool.\n",
                        10, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 11]: Line %d module %s is not synthesizable, the sensitivity list contains complex expression, only primary expressions and bit select are allowed.\n",
                        11, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 12]: Line %d module %s is not synthesizable, register %s is target of both blocking and non blocking assignments in the same always block.\n",
                        12, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 13]: Line %d module %s is not synthesizable, expecting only if statement.\n",
                        13, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 14]: Module %s is not synthesizable, case statement has non-const label.\n",
                        14, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_INFO 15]: Synthesizing module %s ........... \n",
                        15, MsgInfo::IMM, MsgInfo::INFO));            
            addMsgInfo(new MsgInfo("[SYNTH_INFO 16]: Done. \n",
                        16, MsgInfo::IMM, MsgInfo::INFO));            
            addMsgInfo(new MsgInfo("[SYNTH_WARN 17]: Line %d memory element %s is not synthesizable. Use -memmod option to specify memory modules instead.\n",
                        17, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 18]: Line %d system task/function %s will be ignored for synthesis.\n",
                        18, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 19]: Line %d all variables read in the block are not specified in the sensitivity list.\n",
                        19, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 20 ]: Line %d constant function is not supported for synthesis.\n",
                        20, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 21 ]: Line %d Module %s is not synthesizable because of black box instance %s.\n",
                        21, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 22 ]: In module %s, bit select %s is out of range.\n",
                        22, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 23 ]: Could not open cell library file %s for reading.\n",
                        23, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 24 ]: For cell %s mandatory port %s not specified.\n",
                        24, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 25 ]: For cell %s insufficient number of ports specified.\n",
                        25, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 26 ]: For cell %s expecting equal size for ports %s and %s.\n",
                        26, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 27 ]: In module %s illegal set/reset condition for event signal %s.\n",
                        27, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 28 ]: In module %s invalid reset value for reset variable %s, expected 0.\n",
                        28, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 29 ]: In module %s, inconsistent use of Set/Reset/Clock signals. \n",
                        29, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 30 ]: In module %s, incorrect argument specification for task or function call %s. \n",
                        30, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 31 ]: In module %s, event signal %s can not be part of nested if condition. \n",
                        31, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 32 ]: In module %s, can not infer clock signal from always block logic. \n",
                        32, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[ERROR 33 ]: In module %s, multiple set/reset assignments inside always block for variable %s. \n",
                        33, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 34 ]: In module %s, size mismatch between formal and actual for task/function call %s. \n",
                        34, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[ERROR 35 ]: In module %s, illegal output port connection for task call %s. \n",
                        35, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 36 ]: In module %s, logic specified for signal %s does not map to standard flip flop. \n",
                        36, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 37]: Module %s not synthesizable because of presence of %s.\n",
                        37, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_INFO 38]: Starting level %d netlist optimization...\n",
                        38, MsgInfo::IMM, MsgInfo::INFO));
            addMsgInfo(new MsgInfo("[SYNTH_INFO 39]: Level %d netlist optimization done.\n",
                        39, MsgInfo::IMM, MsgInfo::INFO));
            addMsgInfo(new MsgInfo("[SYNTH_INFO 40]: Started mapping to generic gates.....\n",
                        40, MsgInfo::IMM, MsgInfo::INFO));
            addMsgInfo(new MsgInfo("[SYNTH_INFO 41]: Generic mapping done.\n",
                        41, MsgInfo::IMM, MsgInfo::INFO));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 42]: Module %s not synthesizable because of non-const power expression %s.\n",
                        42, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 43]: Function call in statement context is not supported, use task instead, for %s in module %s.\n",
                        43, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_WARN 44]: In module %s, %s %s is not synthesizable.\n",
                        44, MsgInfo::IMM, MsgInfo::WARN));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 45]: Line %d in module %s, event statements are synthesizable only as always statement.\n",
                        45, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 46]: Line %d, recursive %s %s not supported.\n",
                        46, MsgInfo::IMM, MsgInfo::ERROR));
            addMsgInfo(new MsgInfo("[SYNTH_ERROR 47]: In module %s, `for' loop does not seem to terminate.\n",
                        47, MsgInfo::IMM, MsgInfo::ERROR));
                        
            addMsgInfo(new MsgInfo("[CELL_ERROR 301]: For cell %s, incorrect number of data %s ports, expected %d, found %d\n",
                        301, MsgInfo::IMM, MsgInfo::ERROR));            
            addMsgInfo(new MsgInfo("[CELL_ERROR 302]: For cell %s, data input port size does not match data output port size.\n",
                        302, MsgInfo::IMM, MsgInfo::ERROR));            
            addMsgInfo(new MsgInfo("[CELL_ERROR 303]: For cell %s, incorrect size for port %s, expected %d, found %d.\n",
                        303, MsgInfo::IMM, MsgInfo::ERROR));            
            addMsgInfo(new MsgInfo("[CELL_ERROR 304]: In cell %s, port of type %s not found.\n",
                        304, MsgInfo::IMM, MsgInfo::ERROR));            
            addMsgInfo(new MsgInfo("[CELL_ERROR 305]: In cell %s, size of port %s should be a power of 2.\n",
                        305, MsgInfo::IMM, MsgInfo::ERROR));            
            addMsgInfo(new MsgInfo("[CELL_ERROR 306]: In cell %s, size mismatch between ports %s and %s.\n",
                        306, MsgInfo::IMM, MsgInfo::ERROR));            
            addMsgInfo(new MsgInfo("[PARSE_INFO 101]: File name not specified after %s option \n",
                        101, MsgInfo::IMM, MsgInfo::INFO));            
            addMsgInfo(new MsgInfo("[SYNTH_INFO 500]: Synthesis completed with %d error(s) and %d warning(s).\n",
                        500, MsgInfo::IMM, MsgInfo::INFO));            
            addMsgInfo(new MsgInfo("[SYNTH_INFO 501]: Exceeded maximum stack limit %d for function/task evaluation, exiting.\n",
                        501, MsgInfo::IMM, MsgInfo::INFO));            
        }

}

