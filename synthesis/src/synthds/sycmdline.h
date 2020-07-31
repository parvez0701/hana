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

#ifndef _SYCMDLINE_H_
#define _SYCMDLINE_H_

#include <string>
#include "syutils.h"

namespace Synth
{
    typedef struct {
        int argc;
        char **argv;
    } SyCmdOpts;
    
    class SyCmdLine
    {
        public:
            static SyCmdLine*    instance();
            virtual             ~SyCmdLine();

            bool                parseCmdLine(int argc, char **argv);
            const SyCmdOpts&    getAnalyzerOptions() const;
            const SyCmdOpts&    getSynthOptions() const;

            const std::string&    getSynthOutFile() const;
            const std::string&    getIntOutFile() const;
            const std::string&    getRtlOutFile() const;
            const std::string&    getLogFile() const;

            bool                logFileSpecified() const;
            bool                rtlFileSpecified() const;
            bool                synthFileSpecified() const;
            bool                intOutFileSpecified() const;
            bool                genIntermediateCode() const { return this->intOutFileSpecified(); }
            bool                synthesize() const { return this->synthFileSpecified();}
            bool                noOpt() const { return noOptimization;}
			bool                sop() const { return sumOfProduct; }
            bool                cellLibrarySpecified() const;
            const std::string&  getCellLibraryPath() const;
        private:
            SyCmdLine();
            DISABLE_COPYCONST_ASSGNOP(SyCmdLine);
            void                 printHelp() const;
            void                 printVersion() const;
            void                printOptions() const;
            bool                checkForMissingArg(int, char**, int) const;

            static SyCmdLine    *cmdLine;
            std::string            logFile;
            std::string            rtlFile;
            std::string            synthFile;
            std::string            intOutFile;
            std::string            cellLibPath;
            std::string            version;

            SyCmdOpts              analyzerOpts;
            SyCmdOpts              synthOpts;
            bool                   noOptimization;
			bool                   sumOfProduct;
    };
}
#endif

