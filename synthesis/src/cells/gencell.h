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

#ifndef _GENCELL_H_
#define _GENCELL_H_

//To generate definition of a cell module to be used in intermediate code
//on the fly

#include <sstream>
#include <vector>
#include <string>
#include "sycontainer.h"
#include "syutils.h"
#include "cellutil.h"


namespace Synth
{

    class GenCellBase : public SyModule
    {
        friend class GenCellMgr;
        public:
            short getActualInputSize(CellType, short) const;
            std::pair<std::string, std::pair<short, short> >isVecPort(const std::string&) const;
        protected:
            GenCellBase() : SyModule("") {
            }
            GenCellBase(const std::string& name) : SyModule(name) {
            }
            virtual GenCellBase* create(const std::string& cellName, 
                                        const std::vector<std::string> &, 
                                        const std::vector<std::string>&) = 0;
            virtual std::string createName(short, short) const = 0;
            void createVectorPort(const std::string&, int, int, SyScPort::SyDirType);
            void createScalarPort(const std::string&, SyScPort::SyDirType);
        private:
            DISABLE_COPYCONST_ASSGNOP(GenCellBase);
    };

    
    template <CellType T> //Used to create distinct type
    class GenCell : public GenCellBase
    {
        friend class GenCellMgr;
        public:
            GenCell();
            GenCell(const std::string&);
        protected:
            virtual GenCell<T>* create(    const std::string&,
                                        const std::vector<std::string>&, 
                                        const std::vector<std::string>&);
            virtual std::string createName(short, short) const;
        private:
            DISABLE_COPYCONST_ASSGNOP(GenCell);

    };
    template <CellType T>
    GenCell<T>::GenCell() {
    }
    template <CellType T>
    GenCell<T>::GenCell(const std::string& name) : GenCellBase(name) {
    }
    template <CellType T>
    std::string GenCell<T>::createName(short numIn, short numOut) const
    {
        numIn = this->getActualInputSize(T, numIn);
        std::ostringstream str;
        str << CellType2String(T) << "_" << numIn << "_" << numOut;
        return str.str();
    }    
    template <CellType T>
    GenCell<T>* GenCell<T>::create(    const std::string& cellName,
                                    const std::vector<std::string>& inputs,
                                    const std::vector<std::string>& outputs)
    {
        GenCell<T>* cell = new GenCell<T>(cellName);
        //Create output ports
        for(unsigned int i = 0; i < outputs.size(); ++i)
        {
            std::pair<std::string, std::pair<short, short> > portInfo =
                this->isVecPort(outputs[i]);
            
            if(portInfo.second.first == portInfo.second.second){ //scalar
                cell->createScalarPort(portInfo.first, SyScPort::OUT);
            }
            else {
                cell->createVectorPort(    portInfo.first, portInfo.second.first,
                                        portInfo.second.second, SyScPort::OUT);
            }

            //i += portInfo.second.first - portInfo.second.second;
        }    
        for(unsigned int i = 0; i < inputs.size(); ++i)
        {
            std::pair<std::string, std::pair<short, short> > portInfo =
                this->isVecPort(inputs[i]);
            
            if(portInfo.second.first == portInfo.second.second){ //scalar
                cell->createScalarPort(portInfo.first, SyScPort::IN);
            }
            else {
                cell->createVectorPort(    portInfo.first, portInfo.second.first,
                                        portInfo.second.second, SyScPort::IN);
            }

            //i += portInfo.second.first - portInfo.second.second;
        }    
        return cell;
    }    

}
#endif

