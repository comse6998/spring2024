#ifndef _GLOBALS_HH_
#define _GLOBALS_HH_

using namespace std;

namespace CDC8600
{
    extern vector<word> 	MEM;		// shared memory
    extern u32	     		FreeMEM;	// beginning of shared heap
    extern vector<Processor>    PROC;		// Processors
    extern u32			me();		// Processor number of current thread
    extern u32			nump();		// Total number of processors available
} // namespace CDC8600

#endif // _GLOBALS_HH
