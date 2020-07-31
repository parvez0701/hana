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

#ifndef _SYNTHCELL_H_
#define _SYNTHCELL_H_
#include <map>
#include <string>

#include "sylist.h"
#include "syutils.h"

namespace Synth
{
    class SyModule;
    class SyModInst;
    class SyScPort;
    class SynthCELL
    {
        friend class CellMgr;
        public:
            const SyModule* getModule() const {
                return mod;
            }
            int getDelay() const {
                return delay;
            }
            int getArea() const {
                return delay;
            }
            int getMaxFanout(const std::string&) const;
            void addMaxFanout(const std::string&, int);

            const SyList<SyScPort*>& getPorts() const;
            const SyList<SyScPort*>* getInPorts() const {
                return inports;
            }
            SyList<SyScPort*>* getOutPorts() const {
                return outports;
            }
            SyList<SyScPort*>* getInOutPorts() const {
                return inoutports;
            }

            SyList<SyScPin*>* getInPins(const SyModInst*) const;
            SyList<SyScPin*>* getOutPins(const SyModInst*) const;


            void addPort(const SyScPort*);
            virtual SyModInst *instantiate(
                SyModule*,
                const std::string&) const;
        protected:
            SynthCELL(
                const SyModule* mod,
                const int delay,
                const int area);
            SynthCELL();
            virtual ~SynthCELL();
            virtual SynthCELL *create(
                    //0-> delay, 1, area then inputs follow
                    const std::vector<int>&) const = 0;
            virtual std::string getName(const std::vector<int>&) const = 0;
            //mostly used for technology gates
            void addPinMap(
                    const std::string& p1, 
                    const std::string& p2)
            {
                genToTechPortNames.insert(std::make_pair(p1, p2));
            }
        private:
            SynthCELL(const SynthCELL&);
            SynthCELL& operator=(const SynthCELL&);

            SyList<SyScPin*>* getPins(
                    const SyModInst*,
                    const SyScPort::SyDirType) const;

            SyModule *mod;
            int delay;
            int area;
            SyList<SyScPort*>* inports;
            SyList<SyScPort*>* outports;
            SyList<SyScPort*>* inoutports;
            std::map<std::string, int, cmp_str> fanouts;
            std::map<const std::string, const std::string, cmp_str> genToTechPortNames;
    };

    class SynthGND : public SynthCELL
    {
        friend class CellMgr;
        public:
        /*
            virtual SyModInst *instantiate(
                    SyModule*,
                    const std::string&) const;
        */
        protected:
            SynthGND(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthGND();
            virtual SynthGND *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthGND();
            SynthGND(const SynthGND&);
            SynthGND& operator=(const SynthGND&);
    };
    class SynthTGND : public SynthCELL
    {
    };
    class SynthVCC : public SynthCELL
    {
        friend class CellMgr;
        public:
  //          virtual SyModInst *instantiate(
  //                  SyModule*,
  //                  const std::string&) const;
        protected:
            SynthVCC(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthVCC();
            virtual SynthVCC *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthVCC();
            SynthVCC(const SynthVCC&);
            SynthVCC& operator=(const SynthVCC&);
    };
    class SynthBUF : public SynthCELL
    {
        friend class CellMgr;
        public:
    //        virtual SyModInst *instantiate(
    //            SyModule*,
    //            const std::string&) const;
        protected:
            SynthBUF(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthBUF();
            virtual SynthBUF *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthBUF();
            SynthBUF(const SynthBUF&);
            SynthBUF& operator=(const SynthBUF&);
    };
    class SynthNOT : public SynthCELL
    {
        friend class CellMgr;
        public:
      //      virtual SyModInst *instantiate(
      //          SyModule*,
      //          const std::string&) const;
        protected:
            SynthNOT(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthNOT();
            virtual SynthNOT *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthNOT();
            SynthNOT(const SynthNOT&);
            SynthNOT& operator=(const SynthNOT&);
    };
    class SynthAND : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthAND(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthAND();
            virtual SynthAND *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthAND();
            SynthAND(const SynthAND&);
            SynthAND& operator=(const SynthAND&);
    };
    class SynthNAND : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthNAND(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthNAND();
            virtual SynthNAND *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthNAND();
            SynthNAND(const SynthNAND&);
            SynthNAND& operator=(const SynthNAND&);
    };
    class SynthOR : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthOR(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthOR();
            virtual SynthOR *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthOR();
            SynthOR(const SynthOR&);
            SynthOR& operator=(const SynthOR&);
    };
    class SynthNOR : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthNOR(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthNOR();
            virtual SynthNOR *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthNOR();
            SynthNOR(const SynthNOR&);
            SynthNOR& operator=(const SynthNOR&);
    };
    class SynthXOR : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthXOR(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthXOR();
            virtual SynthXOR *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthXOR();
            SynthXOR(const SynthXOR&);
            SynthXOR& operator=(const SynthXOR&);
    };
    class SynthXNOR : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthXNOR(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthXNOR();
            virtual SynthXNOR *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthXNOR();
            SynthXNOR(const SynthXNOR&);
            SynthXNOR& operator=(const SynthXNOR&);
    };
    class SynthMUX : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthMUX(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthMUX();
            virtual SynthMUX *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthMUX();
            SynthMUX(const SynthMUX&);
            SynthMUX& operator=(const SynthMUX&);
    };
    class SynthFF : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthFF(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthFF();
            virtual SynthFF *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthFF();
            SynthFF(const SynthFF&);
            SynthFF& operator=(const SynthFF&);
    };
    class SynthFFR : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthFFR(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthFFR();
            virtual SynthFFR *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthFFR();
            SynthFFR(const SynthFFR&);
            SynthFFR& operator=(const SynthFFR&);
    };
    class SynthLD : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthLD(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthLD();
            virtual SynthLD *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthLD();
            SynthLD(const SynthLD&);
            SynthLD& operator=(const SynthLD&);
    };
    class SynthLDR : public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthLDR(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthLDR();
            virtual SynthLDR *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthLDR();
            SynthLDR(const SynthLDR&);
            SynthLDR& operator=(const SynthLDR&);
    };

    class SynthADD: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthADD(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthADD();
            virtual SynthADD *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthADD();
            SynthADD(const SynthADD&);
            SynthADD& operator=(const SynthADD&);
    };
    class SynthSUB: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthSUB(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthSUB();
            virtual SynthSUB *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthSUB();
            SynthSUB(const SynthSUB&);
            SynthSUB& operator=(const SynthSUB&);
    };
    class SynthMUL: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthMUL(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthMUL();
            virtual SynthMUL *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthMUL();
            SynthMUL(const SynthMUL&);
            SynthMUL& operator=(const SynthMUL&);
    };
    class SynthDIV: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthDIV(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthDIV();
            virtual SynthDIV *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthDIV();
            SynthDIV(const SynthDIV&);
            SynthDIV& operator=(const SynthDIV&);
    };
    class SynthCMP: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthCMP(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthCMP();
            virtual SynthCMP *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthCMP();
            SynthCMP(const SynthCMP&);
            SynthCMP& operator=(const SynthCMP&);
    };
    class SynthLSH: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthLSH(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthLSH();
            virtual SynthLSH *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthLSH();
            SynthLSH(const SynthLSH&);
            SynthLSH& operator=(const SynthLSH&);
    };
    class SynthRSH: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthRSH(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthRSH();
            virtual SynthRSH *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthRSH();
            SynthRSH(const SynthRSH&);
            SynthRSH& operator=(const SynthRSH&);
    };
    class SynthENC: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthENC(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthENC();
            virtual SynthENC *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthENC();
            SynthENC(const SynthENC&);
            SynthENC& operator=(const SynthENC&);
    };
    class SynthDEC: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthDEC(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthDEC();
            virtual SynthDEC *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthDEC();
            SynthDEC(const SynthDEC&);
            SynthDEC& operator=(const SynthDEC&);
    };

    class SynthINC: public SynthCELL
    {
        friend class CellMgr;
        public:
        //    virtual SyModInst *instantiate(
        //        SyModule*,
        //        const std::string&) const;
        protected:
            SynthINC(
                const SyModule* mod,
                const int delay,
                const int area);
            virtual ~SynthINC();
            virtual SynthINC *create(const std::vector<int>&) const;
            virtual std::string getName(const std::vector<int>&) const;
        private:
            SynthINC();
            SynthINC(const SynthINC&);
            SynthINC& operator=(const SynthINC&);
    };
}
#endif
