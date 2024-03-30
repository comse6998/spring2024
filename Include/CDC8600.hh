#ifndef _CDC8600_HH_
#define _CDC8600_HH_

#include<stdlib.h>
#include<stdint.h>
#include<assert.h>
#include<vector>
#include<map>
#include<set>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<complex>
#include<types.hh>
#include<globals.hh>
#include<parameters.hh>
#include<operations.hh>

using namespace std;

namespace CDC8600
{
    void reset();

    void *memalloc(u64);
    void memfree(void*, u64);

    template<typename T> void label(T (*f)());
    void addlabel(string, u32);

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
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;
            PROC[me()].X(3).u() = (word*)arg4 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg5;

            _f();
        }

        void operator()(u64 arg1, c128 *arg2, i64 arg3, c128 *arg4, i64 arg5)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;
            PROC[me()].X(3).u() = (word*)arg4 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg5;

            _f();
        }

        void operator()(u64 arg1, c128 arg2, c128 *arg3, i64 arg4, c128 *arg5, i64 arg6)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).f() = arg2.real();
            PROC[me()].X(2).f() = arg2.imag();
            PROC[me()].X(3).u() = (word*)arg3 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg4;
            PROC[me()].X(5).u() = (word*)arg5 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(6).i() = arg6;

            _f();
        }

        void operator()(u64 arg1, f64 *arg2, i64 arg3, f64 *arg4, i64 arg5, f64 arg6, f64 arg7)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;
            PROC[me()].X(3).u() = (word*)arg4 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg5;
            PROC[me()].X(5).f() = arg6;
            PROC[me()].X(6).f() = arg7;

            _f();
            }

        void operator()(i64 arg1, c128 *arg2, i64 arg3, c128 *arg4, i64 arg5, f64 arg6, f64 arg7)
        {
            label(_f);

            PROC[me()].X(0).i() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;
            PROC[me()].X(3).u() = (word*)arg4 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg5;
            PROC[me()].X(5).f() = arg6;
            PROC[me()].X(6).f() = arg7;

            _f();
        }

        void operator()(u64 arg1, f64 arg2, f64 *arg3, i64 arg4)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).f() = arg2;
            PROC[me()].X(2).u() = (word*)arg3 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(3).i() = arg4;

            _f();
        }

        void operator()(i64 arg1, c128 arg2, c128 *arg3, i64 arg4)
        {
            label(_f);

            PROC[me()].X(0).i() = arg1;
            PROC[me()].X(1).f() = arg2.real();
            PROC[me()].X(2).f() = arg2.imag();
            PROC[me()].X(3).u() = (word*)arg3 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg4;

            _f();
        }       
        
        void operator()(u64 arg1, f64 arg2, f64 *arg3, i64 arg4, f64 *arg5, i64 arg6)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).f() = arg2;
            PROC[me()].X(2).u() = (word*)arg3 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(3).i() = arg4;
            PROC[me()].X(4).u() = (word*)arg5 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(5).i() = arg6;

            _f();
        }
    };

    template<typename T0>
    class func0
    {
      private:
        T0 (*_f)();

      public:
        func0(T0 (*f)())
        {
            _f = f;
        }

        T0 operator()(u64 arg1, f64 *arg2, i64 arg3)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;

            _f();
                
            return (T0) PROC[me()].X(0);
        }

        f64 operator()(u64 arg1, f64 *arg2, i64 arg3, f64 *arg4, i64 arg5)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;
            PROC[me()].X(3).u() = (word*)arg4 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg5;

            _f();

            return PROC[me()].X(0).f();
        }

        c128 operator()(u64 arg1, c128 *arg2, i64 arg3, c128 *arg4, i64 arg5)
        {
            label(_f);

            PROC[me()].X(0).u() = arg1;
            PROC[me()].X(1).u() = (word*)arg2 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(2).i() = arg3;
            PROC[me()].X(3).u() = (word*)arg4 - &(MEM[PROC[me()].RA().u()*256]);
            PROC[me()].X(4).i() = arg5;

            _f();

            return c128(PROC[me()].X(0).f(), PROC[me()].X(1).f());
        }
    };

    template <typename T0, typename T1, typename T2, typename T3> class func3
    {
      private:
        T0 (*_f)(T1 arg1, T2 arg2, T3 arg3);

      public:
        func3(T0 (*f)(T1 arg1, T2 arg2, T3 arg3))
        {
            _f = f;
        }
        T0 operator()(T1 arg1, T2 arg2, T3 arg3)
        {
            return _f(arg1, arg2, arg3);
        }
    };

    template <typename T1, typename T2, typename T3, typename T4> class call4
    {
      private:
        void (*_f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4);

      public:
        call4(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4))
        {
            _f = f;
        }
        void operator()(T1 arg1, T2 arg2, T3 arg3, T4 arg4)
        {
            _f(arg1, arg2, arg3, arg4);
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

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5> class func5
    {
      private:
        T0 (*_f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5);

      public:
        func5(T0 (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5))
        {
            _f = f;
        }
        T0 operator()(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
        {
            return _f(arg1, arg2, arg3, arg4, arg5);
        }
    };

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> class call6
    {
    private:
            void (*_f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6);

    public:
            call6(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6))
            {
                    _f = f;
            }
            void operator()(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
            {
                    _f(arg1, arg2, arg3, arg4, arg5, arg6);
            }
    };

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> class call7
    {
      private:
        void (*_f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7);

      public:
        call7(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7))
        {
            _f = f;
        }
        void operator()(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
        {
            _f(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    };

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10> class call10
    {
      private:
        void (*_f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10);

      public:
        call10(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10))
        {
            _f = f;
        }
        void operator()(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10)
        {
            _f(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
        }
    };

    call0 Call(void (*f)());

    template<typename T0>
    func0<T0> Func(T0 (*f)())
    {
        return func0<T0>(f);
    }

    template <typename T0, typename T1, typename T2, typename T3>
    func3<T0, T1, T2, T3> Func(T0 (*f)(T1 arg1, T2 arg2, T3 arg3))
    {
        return func3<T0, T1, T2, T3>(f);
    }

    template <typename T1, typename T2, typename T3, typename T4>
    call4<T1, T2, T3, T4> Call(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4))
    {
        return call4<T1, T2, T3, T4>(f);
    }   

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    call5<T1, T2, T3, T4, T5> Call(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5))
    {
        return call5<T1, T2, T3, T4, T5>(f);
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    func5<T0, T1, T2, T3, T4, T5> Func(T0 (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5))
    {
        return func5<T0, T1, T2, T3, T4, T5>(f);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    call6<T1, T2, T3, T4, T5, T6> Call(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6))
    {
            return call6<T1, T2, T3, T4, T5, T6>(f);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    call7<T1, T2, T3, T4, T5, T6, T7> Call(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7))
    {
        return call7<T1, T2, T3, T4, T5, T6, T7>(f);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    call10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> Call(void (*f)(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10))
    {
        return call10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(f);
    }

    class Fijk : public instruction     // Instructions of 4/4/4/4 format
    {
        protected:
            u08 _F;     // 4-bit function code
            u08 _i;     // 4-bit i field
            u08 _j;     // 4-bit j field
            u08 _k;     // 4-bit k field
        public:
            Fijk(u08 F, u08 i, u08 j, u08 k)
            {
                assert(F < 16);
                assert(i < 16);
                assert(j < 16);
                assert(k < 16);
                _F = F;
                _i = i;
                _j = j;
                _k = k;
            }
            u08 len() const { return 2; }
            string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
            u32 encoding() const { return (_F << 12) + (_i << 8) + (_j << 4) + (_k << 0); }
    };

    class FijK : public instruction     // Instructions of 4/4/4/20 format
    {
        protected:
            u08 _F;     // 4-bit function code
            u08 _i;     // 4-bit i field
            u08 _j;     // 4-bit j field
            u32 _K;     // 20-bit K field
        public:
            FijK(u08 F, u08 i, u08 j, u32 K)
            {
                assert(F < 16);
                assert(i < 16);
                assert(j < 16);
                assert(K < (1 << 20));
                _F = F;
                _i = i;
                _j = j;
                _K = K;
            }
            u08 len() const { return 4; }
            string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_K) + ")"; }
            u32 encoding() const { return (_F << 28) + (_i << 24) + (_j << 20) + (_K << 0); }
    };

    class Fjn : public instruction      // Instructions of 6/4/6 format
    {
        protected:
            u08 _F;     // 6-bit function code
            u08 _j;     // 4-bit j field
            u08 _n;     // 6-bit n field
        public:
            Fjn(u08 F, u08 j, u08 n)
            {
                assert(F < 64);
                assert(j < 16);
                assert(n < 64);
                _F = F;
                _j = j;
                _n = n;
            }
            u08 len() const { return 2; }
            string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_n) + ")"; }
    };

    class Fjk : public instruction      // Instructions of 8/4/4 format
    {
        protected:
            u08 _F;     // 8-bit Function code
            u08 _j;     // 4-bit j field
            u08 _k;     // 4-bit k field
        public:
            Fjk(u08 F, u08 j, u08 k)
            {
                assert(F < 256);
                assert(j < 16);
                assert(k < 16);
                _F = F;
                _j = j;
                _k = k;
            }
            u08 len() const { return 2; }
            string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
            u32 encoding() const { return (_F << 8) + (_j << 4) + (_k << 0); }
    };

    class FjK : public instruction      // Instructions of 8/4/20 fomrat
    {
        protected:
            u08 _F;     // 8-bit Function code
            u08 _j;     // 4-bit j field
            u32 _K;     // 20-bit K field
        public:
            FjK(u08 F, u08 j, u32 K)
            {
                assert(F < 256);
                assert(j < 16);
                assert(K < (1 << 20));
                _F = F;
                _j = j;
                _K = K;
            }
            u08 len() const { return 4; }
            string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_K) + ")"; }
            u32 encoding() const { return (_F << 24) + (_j << 20) + (_K << 0); }
    };

    namespace instructions
    {
#include<instructions/pass.hh>                          // Pass                                                         (p54)
#include<instructions/jmp.hh>                           // Jump to P+K                                                  (p86)
#include<instructions/jmpz.hh>                          // Jump to P + K if (Xj) equal to 0                             (p94)
#include<instructions/jmpp.hh>                          // Jump to P + K if (Xj) positive                               (p98)
#include<instructions/jmpn.hh>                          // Jump to P + K if (Xj) negative                               (p100)
#include<instructions/jmpk.hh>                          // Subroutine exit, computed jump to (Xj) + k                   (p110)
#include<instructions/jmpk0.hh>                         // Subroutine exit, computed jump to (Xj) + k and return 0 
#include<instructions/xkj.hh>                           // Transmit k to Xj                                             (p55)
#include<instructions/compk.hh>                         // Copy complement of (Xk) to Xj                                (p41)
#include<instructions/lpjkj.hh>                         // Logical product of (Xj) times (Xk) to Xj                     (p37)
#include<instructions/isjki.hh>                         // Integer sum of (Xj) plus (Xk) to Xi                          (p122)
#include<instructions/idjki.hh>                         // Integer difference of (Xj) plus (Xk) to Xi                   (p123)
#include<instructions/ipjkj.hh>                         // Integer product of (Xj) times (Xk) to Xj                     (p52)
#include<instructions/idjkj.hh>                         // Integer difference of (Xj) minus k to Xj                     (p58)
#include<instructions/isjkj.hh>                         // Integer sum of (Xj) plus k to Xj                             (p57)
#include<instructions/idzkj.hh>                         // Integer difference of zero minus (Xk) to Xj                  (p62)
#include<instructions/rdKj.hh>                          // Read data at address K to Xj                                 (p74)
#include<instructions/rdjki.hh>                         // Read data at address (Xj) + (Xk) to (Xi)                     (p133)
#include<instructions/rdjk.hh>                          // Read data at address (Xk) to (Xj)                            (p75)
#include<instructions/sdjki.hh>                         // Store data at address (Xj) + (Xk) from Xi                    (p135)
#include<instructions/fmul.hh>                          // floating point multiplication Xi = Xj * Xk                   (p124)
#include<instructions/fadd.hh>                          // floating point addition Xi = Xj + Xk                         (p126)
#include<instructions/fsub.hh>                          // floating point subtraction Xi = Xj - Xk                      (p128)
#include<instructions/bb.hh>                            // Branch backward i words if (Xj) < (Xk)
#include<instructions/jmpnz.hh>                         // Jump to P + K if (Xj) unequal to 0 
    } // namespace instructions

    extern bool                 tracing;

    extern bool process(instruction*, u32);

    extern void dump(vector<instruction*>&);

    extern void dump(vector<instruction*>&, const char* filename);

    namespace instructions
    {
	class basemaker
	{
	    public:
		basemaker() { }
		virtual instruction* make() = 0;
	};

	template<typename T>
	class maker : public basemaker
	{
	    public:
		maker() : basemaker() { }
		virtual instruction* make() { return new T; }
	};

	extern vector<basemaker*> makeinstr;

	extern void initmakers();

	extern vector<u64> decode(u32);
    } // namespace instructions

    namespace pipeline
    {
	typedef vector<bool> bitvector;

	template<u32 m, u32 n>
	class stage
	{
	    public:
		bitvector in;		// stage inputs  (m bits)
		bitvector out;		// stage outputs (n bits)
		bool txready;		// stage outputs are ready
		bool txdone;		// outputs have been sent to next stage
		bool rxready;		// stage ready for new inputs
		bool rxdone;		// inputs have been received from previous stage

		stage() : in(m), out(n) { }
		virtual void tick() 	
		{
		   if (txdone && rxdone)
		   {
		      for (u32 i=0; i<n; i++) out[i] = false;
		      for (u32 i=0; i<min(m,n); i++) out[i] = in[i];
		      rxdone = false; rxready = true;
		      txready = true; txdone = false;
		   }
		}
		virtual void tock() 	{ assert(false); }
		virtual bool busy()	{ return false; }
		virtual void reset()	{ rxready = true; rxdone = true; txready = true; txdone = true; }
	};

	void copy(u32 N, u64 u, bitvector& v, u32 first);
	void copy(u32 N, const bitvector& u, u32 first, u64& v);
	void copy(u32 N, const bitvector& u, u32 first, u32& v);
	void copy(u32 N, const bitvector& u, u32 ufirst, bitvector& v, u32 vfirst);

	template<u32 m, u32 n, u32 p, u32 q>
	void transfer(u32 N, stage<m,n>& src, u32 srcfirst, stage<p,q>& dst, u32 dstfirst)
	{
	    assert(srcfirst < n); assert((srcfirst + N) <= n);
	    assert(dstfirst < p); assert((dstfirst + N) <= p);

	    if (!dst.rxready) 		// destination ready?
	    {
		// destination not ready
		dst.rxdone = false;
		src.txdone = false;
		return;
	    }
	    if (!src.txready)		// source ready?
	    {
		// source not ready
		src.txdone = false;
		dst.rxdone = false;
		return;
	    }

	    // both source and destination are ready
	    for (u32 i=0; i<N; i++) dst.in[dstfirst + i] = src.out[srcfirst + i];
	    dst.rxdone = true;
	    src.txdone = true;
	}

	class IFstage : public stage<0,96>
	{
	    private:
		vector<u64> fetchgroups;
		u32	    fetchcount;
	    public:
		void init(const char* filename);
		void tick();
		bool busy();
	};

	extern IFstage IF;

	class ICstage : public stage<48,80>
	{
	    public:
		vector<bitvector>	opsq;

	    public:
		void tick();
	};

	extern ICstage IC[2];

	class RMstage : public stage<160,192>
	{
	    public:
		u32  opcount;
		void tick();
		void init() { opcount = 0; }
	};

	extern RMstage RM;

	class ODstage : public stage<96, 96>
	{
	};

	extern ODstage OD[2];

	class IQstage : public stage<96,96>
	{
	};
	
	extern IQstage IQ[2];

	class OIstage : public stage<96,96>
	{
	    private:
		u32	_ix;			// operation issue index (0/1)
	    public:
		void init(u32 ix) { _ix = ix; }	// initialize this operation issue stage to a particular index (0/1)
		stage<96,96>& target();		// target unit for next issue
	};

	extern OIstage OI[2];

	class BRstage : public stage<96,96>
	{
	};

	extern BRstage BR[2];

	class FXstage : public stage<96,96>
	{
	};

	extern FXstage FX[2];

	class FPstage : public stage<96,96>
	{
	};

	extern FPstage FP[2];

	class LDstage : public stage<96,96>
	{
	};

	extern LDstage LD[2];

	class STstage : public stage<96,96>
	{
	};

	extern STstage ST[2];

	class CQstage : public stage<96,96>
	{
	};

	extern CQstage CQ[2];

	class COstage : public stage<192,0>
	{
	    public:
		void tick();
	};

	extern COstage CO;

	void reset();
	void tick();
	void tock();
	bool busy();
	void transfer();
	void run(const char* filename);
    } // namespace pipeline

} // namespace CDC8600

#endif // _CDC8600_HH_
