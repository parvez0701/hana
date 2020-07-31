#ifndef _NETLISTVISITORBASE_H_
#define _NETLISTVISITORBASE_H_

namespace stview
{
    class SBlock;
    class SBlockInst;
    class SCell;
    class SConst;
    class SInst;
    class SNet;
    class SPin;
    class SPort;

    class NetlistVisitorBase
    {
        public:
            NetlistVisitorBase() { }
            virtual ~NetlistVisitorBase() { }

            //default visits are no-op
            virtual void visit(SBlock*) { }
            virtual void visit(SBlockInst*) { }
            virtual void visit(SCell*) { }
            virtual void visit(SConst*) { }
            virtual void visit(SInst*) { }
            virtual void visit(SNet*) { }
            virtual void visit(SPin*) { }
            virtual void visit(SPort*) { }
    };
}
#endif
