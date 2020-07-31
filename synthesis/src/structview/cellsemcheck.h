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
#ifndef _CELLSEMCHECK_H_
#define _CELLSEMCHECK_H_

#include <string>
#include <vector>

namespace stview
{
    class SCell;

    class CellSemCheck
    {
        public:
            CellSemCheck(const SCell*);
            virtual ~CellSemCheck();
            virtual bool doChecks() const = 0;
            virtual void setAttribs() = 0;
            void setCell(const SCell*);
            const SCell* getCell() const;
            SCell* getCell();
        protected:
            bool checkSizeMatch(const std::string&, const std::string&) const;
            bool checkNumInputs(int) const;
            bool checkNumOutputs(int) const;
            bool checkExists(const std::string& attrib) const;
            bool checkSize(const std::string&, int) const;
            std::vector<std::string> getDataInputs() const;
            std::vector<std::string> getDataOutputs() const;

            int getDataInputsCount() const;
            int getDataOutputsCount() const;
            int getSize(const std::string&) const;


        private:    
            CellSemCheck();
            CellSemCheck(const CellSemCheck&);
            CellSemCheck& operator=(const CellSemCheck&);

            const SCell *cell;
    };

    class BufSemCheck : public CellSemCheck
    {
        public:
            BufSemCheck(const SCell*);
            virtual ~BufSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            BufSemCheck();
            BufSemCheck(const BufSemCheck&);
            BufSemCheck& operator=(const BufSemCheck&);
    };
    class TriBufSemCheck : public CellSemCheck
    {
        public:
            TriBufSemCheck(const SCell*);
            virtual ~TriBufSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            TriBufSemCheck();
            TriBufSemCheck(const TriBufSemCheck&);
            TriBufSemCheck& operator=(const TriBufSemCheck&);
    };
    class InvSemCheck : public CellSemCheck
    {
        public:
            InvSemCheck(const SCell*);
            virtual ~InvSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            InvSemCheck();
            InvSemCheck(const InvSemCheck&);
            InvSemCheck& operator=(const InvSemCheck&);
    };

    class AONNXXSemCheck: public CellSemCheck
    {
        public:
            virtual ~AONNXXSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        protected:    
            AONNXXSemCheck(const SCell*);
        private:
            AONNXXSemCheck();
            AONNXXSemCheck(const AONNXXSemCheck&);
            AONNXXSemCheck& operator=(const AONNXXSemCheck&);
    };
    class AndSemCheck: public AONNXXSemCheck
    {
        public:
            AndSemCheck(const SCell*);
            virtual ~AndSemCheck();
        private:
            AndSemCheck();
            AndSemCheck(const AndSemCheck&);
            AndSemCheck& operator=(const AndSemCheck&);
    };

    class OrSemCheck: public AONNXXSemCheck
    {
        public:
            OrSemCheck(const SCell*);
            virtual ~OrSemCheck();
        private:
            OrSemCheck();
            OrSemCheck(const OrSemCheck&);
            OrSemCheck& operator=(const OrSemCheck&);
    };
    class NandSemCheck: public AONNXXSemCheck
    {
        public:
            NandSemCheck(const SCell*);
            virtual ~NandSemCheck();
        private:
            NandSemCheck();
            NandSemCheck(const NandSemCheck&);
            NandSemCheck& operator=(const NandSemCheck&);
    };
    class NorSemCheck: public AONNXXSemCheck
    {
        public:
            NorSemCheck(const SCell*);
            virtual ~NorSemCheck();
        private:
            NorSemCheck();
            NorSemCheck(const NorSemCheck&);
            NorSemCheck& operator=(const NorSemCheck&);
    };
    class XorSemCheck: public AONNXXSemCheck
    {
        public:
            XorSemCheck(const SCell*);
            virtual ~XorSemCheck();
        private:
            XorSemCheck();
            XorSemCheck(const XorSemCheck&);
            XorSemCheck& operator=(const XorSemCheck&);
    };
    class XnorSemCheck: public AONNXXSemCheck
    {
        public:
            XnorSemCheck(const SCell*);
            virtual ~XnorSemCheck();
        private:
            XnorSemCheck();
            XnorSemCheck(const XnorSemCheck&);
            XnorSemCheck& operator=(const XnorSemCheck&);
    };
    class EncSemCheck : public CellSemCheck
    {
        public:
            EncSemCheck(const SCell*);
            virtual ~EncSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            EncSemCheck();
            EncSemCheck(const EncSemCheck&);
            EncSemCheck& operator=(const EncSemCheck&);
    };
    class DecSemCheck : public CellSemCheck
    {
        public:
            DecSemCheck(const SCell*);
            virtual ~DecSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            DecSemCheck();
            DecSemCheck(const DecSemCheck&);
            DecSemCheck& operator=(const DecSemCheck&);
    };

    class MuxSemCheck : public CellSemCheck
    {
        public:
            MuxSemCheck(const SCell*);
            virtual ~MuxSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            MuxSemCheck();
            MuxSemCheck(const MuxSemCheck&);
            MuxSemCheck& operator=(const MuxSemCheck&);
    };

    class AddSemCheck : public CellSemCheck
    {
        public:
            AddSemCheck(const SCell*);
            virtual ~AddSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            AddSemCheck();
            AddSemCheck(const AddSemCheck&);
            AddSemCheck& operator=(const AddSemCheck&);
    };
    class SubSemCheck : public CellSemCheck
    {
        public:
            SubSemCheck(const SCell*);
            virtual ~SubSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            SubSemCheck();
            SubSemCheck(const SubSemCheck&);
            SubSemCheck& operator=(const SubSemCheck&);
    };
    class MulSemCheck : public CellSemCheck
    {
        public:
            MulSemCheck(const SCell*);
            virtual ~MulSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            MulSemCheck();
            MulSemCheck(const MulSemCheck&);
            MulSemCheck& operator=(const MulSemCheck&);
    };
    class FFSemCheck : public CellSemCheck
    {
        public:
            FFSemCheck(const SCell*);
            virtual ~FFSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        protected:    
            FFSemCheck();
        private:
            FFSemCheck(const FFSemCheck&);
            FFSemCheck& operator=(const FFSemCheck&);
    };
    class RFFSemCheck : virtual public FFSemCheck
    {
        public:
            RFFSemCheck(const SCell*);
            virtual ~RFFSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        protected:    
            RFFSemCheck();
        private:
            RFFSemCheck(const RFFSemCheck&);
            RFFSemCheck& operator=(const RFFSemCheck&);
    };
    class SFFSemCheck : virtual public FFSemCheck
    {
        public:
            SFFSemCheck(const SCell*);
            virtual ~SFFSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        protected:    
            SFFSemCheck();
        private:
            SFFSemCheck(const SFFSemCheck&);
            SFFSemCheck& operator=(const SFFSemCheck&);
    };
    class RSFFSemCheck : virtual public RFFSemCheck, virtual public SFFSemCheck
    {
        public:
            RSFFSemCheck(const SCell*);
            virtual ~RSFFSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            RSFFSemCheck();
            RSFFSemCheck(const RSFFSemCheck&);
            RSFFSemCheck& operator=(const RSFFSemCheck&);
    };
    class SRFFSemCheck : public RSFFSemCheck
    {
        public:
            SRFFSemCheck(const SCell*);
            virtual ~SRFFSemCheck();
            virtual void setAttribs();
        private:
            SRFFSemCheck();
            SRFFSemCheck(const SRFFSemCheck&);
            SRFFSemCheck& operator=(const SRFFSemCheck&);
    };
    class LDSemCheck : public CellSemCheck
    {
        public:
            LDSemCheck(const SCell*);
            virtual ~LDSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            LDSemCheck();
            LDSemCheck(const LDSemCheck&);
            LDSemCheck& operator=(const LDSemCheck&);
    };
    class ShiftSemCheck : public CellSemCheck
    {
        public:
            ShiftSemCheck(const SCell*);
            virtual ~ShiftSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            ShiftSemCheck();
            ShiftSemCheck(const ShiftSemCheck&);
            ShiftSemCheck& operator=(const ShiftSemCheck&);
    };
    class LShiftSemCheck : public ShiftSemCheck
    {
        public:
            LShiftSemCheck(const SCell*);
            virtual ~LShiftSemCheck();
        private:
            LShiftSemCheck();
            LShiftSemCheck(const LShiftSemCheck&);
            LShiftSemCheck& operator=(const LShiftSemCheck&);
    };
    class RShiftSemCheck : public ShiftSemCheck
    {
        public:
            RShiftSemCheck(const SCell*);
            virtual ~RShiftSemCheck();
        private:
            RShiftSemCheck();
            RShiftSemCheck(const RShiftSemCheck&);
            RShiftSemCheck& operator=(const RShiftSemCheck&);
    };
    class IncSemCheck : public CellSemCheck
    {
        public:
            IncSemCheck(const SCell*);
            virtual ~IncSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            IncSemCheck();
            IncSemCheck(const IncSemCheck&);
            IncSemCheck& operator=(const IncSemCheck&);
    };
    class VccSemCheck : public CellSemCheck
    {
        public:
            VccSemCheck(const SCell*);
            virtual ~VccSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            VccSemCheck(const VccSemCheck&);
            VccSemCheck& operator=(const VccSemCheck&);
    };

    class GndSemCheck : public CellSemCheck
    {
        public:
            GndSemCheck(const SCell*);
            virtual ~GndSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            GndSemCheck(const GndSemCheck&);
            GndSemCheck& operator=(const GndSemCheck&);
    };

    class CmpSemCheck : public CellSemCheck
    {
        public:
            CmpSemCheck(const SCell*);
            virtual ~CmpSemCheck();
            virtual bool doChecks() const;
            virtual void setAttribs();
        private:
            CmpSemCheck(const CmpSemCheck&);
            CmpSemCheck& operator=(const CmpSemCheck&);
    };
}
#endif
