#ifndef _PARAMETERS_HH_
#define _PARAMETERS_HH_

using namespace std;

namespace CDC8600
{
    extern bool tracing;		// run-time tracing

    namespace params
    {
	namespace micro
	{

	    enum
	    {
		Xs	  = 16,
		MODEFLAGS = 17,
		CONDFLAGS = 18,
		CMPFLAGS  = 19,
		RA	  = 20,
		XA	  = 21,
		FL	  = 22,
		P	  = 23
	    };

	    const u32	nFPUs = 1;
	    const u32	nFXUs = 1;
	    const u32	nBRUs = 1;
	    const u32   nLDUs = 1;
	    const u32 	nSTUs = 1;

	    const u32	nregs = 24;	// number of micro-architected registers
	    				// 0-15 : X registers
					//   16 : X scratch register
					//   17 : mode flags
					//   18 : condition flags
					//   19 : compare flags
					//   20 : RA
					//   21 : XA
					//   22 : FL
					//   23 : P

	    const u32	pregs = 64;	// number of physical registers

	    const u32	maxdispatch = 4;// maximum # of operations that can be dispatched per cycle
	}

	namespace MEM
	{
	    const u32	N = 262144;	// Memory size = 256Ki words
	    const u32	latency = 30;	// Memory latency
	} // namespace MEM

	namespace Proc
	{
	    const u32 	N = 4;		// Number of processors
	} // namespace Proc

	namespace L1
	{
	    const u32	linesize = 4;	// # of words in a cache line
	    const u32	nsets = 8;	// # of sets in the cache
	    const u32	nways = 4;	// cache associativity
	    const u32	latency = 4;	// L1 latency
	} // namespace L1
    } // namespace params
} // namespace CDC8600

#endif // _PARAMETERS_HH_
