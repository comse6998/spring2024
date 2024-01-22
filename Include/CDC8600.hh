#ifndef _CDC8600_HH_
#define _CDC8600_HH_

#include<stdlib.h>
#include<stdint.h>
#include<assert.h>
#include<vector>

using namespace std;

namespace CDC8600
{
    typedef uint64_t	u64;
    typedef int64_t	i64;
    typedef double	f64;

    namespace params
    {

	namespace MEM
	{
	    const uint32_t	N = 262144;	// Memory size = 256Ki words
	} // namespace Memory

    } // namespace params

    class word		// 64 bits, interpreted as signed, unsigned, or float
    {
	private:

	    union
	    {
		u64	u;
		i64	i;
		f64	f;
	    } _data;

	public:

	    word& operator=(i64 x)
	    {
		_data.i = x;
		return *this;
	    }
    };

    class Processor
    {
	private:

	public:

	    uint8_t	XA;
	    word&	X(uint8_t);
    };

    extern vector<word>	MEM;
    extern uint32_t	FreeMEM;
    extern Processor	PROC;

    void reset();

    void *memalloc(u64);

    class call0
    {
      private:
        void (*_f)();

      public:
        call0(void (*f)())
        {
            _f = f;
        }

        void operator()(u64 arg1, f64 *arg2, i64 arg3, f64 *arg4, i64 arg5)
        {
	    assert(false);
	}
    };

    template <typename T1, typename T2, typename T3, typename T4, typename T5> class call5
    {
      private:
        void (*_f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);

      public:
        call5(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5))
        {
            _f = f;
        }
        void operator()(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
        {
	    _f(arg1, arg2, arg3, arg4, arg5);
	}
    };

    call0 Call(void (*f)());

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    call5<T1, T2, T3, T4, T5> Call(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5))
    {
        return call5<T1, T2, T3, T4, T5>(f);
    }

    namespace instructions
    {
	bool jmpz(uint8_t);			// Jump to P + K if (Xj) equal to 0 (p94)
	void xkj(uint8_t, uint8_t);		// Transmit k to Xj (p55)
	void rdjki(uint8_t, uint8_t, uint8_t);	// Read data at address (Xj) + (Xk) to (Xi)
	void sdjki(uint8_t, uint8_t, uint8_t);	// Store data at address (Xj) + (Xk) from Xi
	void isjki(uint8_t, uint8_t, uint8_t);	// Integer sum of (Xj) plus (Xk) to Xi
	void idjkj(uint8_t, uint8_t);		// Integer difference of (Xj) minus k to Xj (p58)
    } // namespace instructions
} // namespace CDC8600

#endif // _CDC8600_HH_
