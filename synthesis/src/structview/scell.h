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

#ifndef _SCELL_H_
#define _SCELL_H_
#include <list>
#include <map>
#include "sbase.h"
#include "sutils.h"
namespace stview
{
    class SPort;
    class SCell : public SBase
    {
        friend int CellLibparse();
        friend class SCellMgr;
        public:
            typedef enum {
                CELLTYPE_BEGIN=1, 
                GND, 
                VCC, 
                BUF, 
                TRIBUF, 
                NOT, 
                AND, 
                NAND, 
                OR, 
                NOR, 
                XOR,
                XNOR, 
                MUX, 
                FF, 
                RFF, 
                SFF, 
                RSFF, 
                SRFF, 
                LD, 
                RLD, 
                ADD, 
                SUB, 
                MUL, 
                DIV, 
                CMP, 
                LSH,
                RSH, 
                ENC, 
                DEC, 
                INC,
                CELLTYPE_END
            } CellType;

            typedef std::map<std::string, const SPort*, cmp_str> SpecialPorts;
            
            SCell(const std::string&, CellType, bool=true);
            virtual ~SCell();
            
            inline const std::list<SPort*>& getPorts() const;
            
            const SPort* getPort(const std::string&) const;
            const SPort* getSpecialPort(const std::string&) const;
            
            void addPort(const SPort*);
            
            

            inline CellType getCellType() const;
            inline void setArea(float);
            inline void setDelay(float);
            inline void setCellType(CellType);

            inline float getArea() const;
            inline float getDelay() const;

            
            bool doChecks() const;
            bool isGeneric() const {
                return generic;
            }    

            virtual void accept(NetlistVisitorBase*);
            //const SpecialPorts& getSpecialPorts() const;
            std::list<const SPort*> getSpecialPorts(const std::string&) const;
            
        private:
            //Copy constructor and assignment operator disabled.
            //Only pointer and refernces to be used
            SCell( const SCell& );
            SCell& operator=( const SCell& );
            //data
            std::list<SPort*>    ports;
            CellType             cellType;
            float                area;
            float                delay;
            bool                generic;
            SpecialPorts        specialPorts;
    };

    //inline functions
    const std::list<SPort*>& SCell::getPorts() const {
        return ports;
    }

    SCell::CellType SCell::getCellType() const {
        return cellType;
    }

    void SCell::setArea(float a) {
        area = a;
    }

    void SCell::setDelay(float d) {
        delay = d;
    }    

    float SCell::getArea() const {
        return area;
    }

    float SCell::getDelay() const {
        return delay;
    }    

    void SCell::setCellType(CellType ct) {
        cellType = ct;
    }    

    void operator++(SCell::CellType& ct);
    SCell::CellType operator++(SCell::CellType& ct, int); 

}
#endif
