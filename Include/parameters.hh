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

		#ifndef _NFPU_
		#define _NFPU_ 1
		#endif

		#ifndef _NFXU_
		#define _NFXU_ 1
		#endif

		#ifndef _NBRU_
		#define _NBRU_ 1
		#endif

		#ifndef _NLDU_
		#define _NLDU_ 1
		#endif

		#ifndef _NSTU_
		#define _NSTU_ 1
		#endif

		
	    const u32	nFPUs = _NFPU_;
	    const u32	nFXUs = _NFXU_;
	    const u32	nBRUs = _NBRU_;
	    const u32   nLDUs = _NLDU_;
	    const u32 	nSTUs = _NSTU_;

	    const u32	nregs = 24;	// number of micro-architected registers
	    				// 0-15 : X registers
					// 16 : X scratch register
					// 17 : mode flags
					// 18 : condition flags
					// 19 : compare flags
					// 20 : RA
					// 21 : XA
					// 22 : FL
					// 23 : P
	}
	namespace MEM
	{
	    const u32	N = 262144;	// Memory size = 256Ki words
	} // namespace MEM
    } // namespace params
} // namespace CDC8600

#endif // _PARAMETERS_HH_
