#ifndef _TYPES_HH_
#define _TYPES_HH_

using namespace std;

namespace CDC8600
{
    typedef uint64_t            u64;
    typedef uint32_t            u32;
    typedef uint8_t             u08;
    typedef int64_t             i64;
    typedef int32_t             i32;
    typedef double              f64;
    typedef complex<double>     c128;

    namespace units
    {
        class unit
        {
            private:
                set<u64>        _busy;  // the cycles this unit is busy

            public:
                unit() { }
                void clear()               { _busy.clear(); }                   // clear the unit usage
                bool busy(u64 cycle) const { return _busy.count(cycle); }       // is the unit busy at that cycle?
                void claim(u64 cycle)      { _busy.insert(cycle); }             // claim the unit for that cycle
                bool isfree(u64 begin, u64 end)                                 // is the unit free for cycles [begin, end)?
                {
                    for (u64 i = begin; i < end; i++) if (busy(i)) return false;
                    return true;
                }
        };
    } // namespace units

    namespace L1
    {
        class cacheset
        {
            private:
                vector<bool>    _valid;
                vector<u32>     _tag;
                vector<u64>     _used;

            public:
                cacheset();
                void reset();
                bool valid(u32 i) { return _valid[i]; }
                u32& tag(u32 i) { return _tag[i]; }
                u64& used(u32 i) { return _used[i]; }
                void invalidate(u32 i) { _valid[i] = false; }
                void validate(u32 i) { _valid[i] = true; }
        };

        class cache
        {
            private:
                vector<cacheset>        _sets;

            public:
                cache();
                void reset();
                bool hit(u32 addr);
                bool loadhit(u32 addr, u64 cycle);
                bool storehit(u32 addr, u64 cycle);
        };
    }

    class word          // 64 bits, interpreted as signed, unsigned, or float
    {
        private:

            union
            {
                u64     u;
                i64     i;
                f64     f;
            } _data;

        public:

            word& operator=(i64 x)
            {
                _data.i = x;
                return *this;
            }

            u64& u()       { return _data.u; } 
            u64  u() const { return _data.u; }
            i64& i()       { return _data.i; }
            i64  i() const { return _data.i; }
            f64& f()       { return _data.f; }
            f64  f() const { return _data.f; }

            operator u64() { return _data.u; }
            operator i64() { return _data.i; }
            operator f64() { return _data.f; }
    };

    template<int n> class reg
    {
        private:
            u32 _loc;   // location of memory word containing this register
            u08 _first; // first bit in word for this register
        public:

            reg(u32 loc, u08 first) : _loc(loc), _first(first) { assert(n <= 20); assert(_first + n <= 64); }
            u64 u();
            reg<1> operator()(uint8_t);
            reg<n>& operator=(bool);
            reg<n>& operator=(u64);
    };

    namespace operations
    {
	class operation;
    } // namespace operations

    class instruction                                   // Generic instruction class
    {
        protected:
            u32         _line;                          // line number of instruction in source file
            u32         _addr;                          // byte (not word) address of instruction in memory
            string      _trace;                         // additional trace information
        public:
            virtual ~instruction() { }                  // virtual destructor
            virtual bool execute() = 0;                 // every instruction must have a method "execute" that implements its semantics and returns "true" if branch is taken
            virtual bool ops() { return false; }        // the ops method processes the internal ops that implement the instrution
            virtual u08 len() const = 0;                // length of instruction in bytes (2 or 4)
            virtual string mnemonic() const = 0;        // mnemonic for the instruction
            virtual string dasm() const = 0;            // disassembly for the instruction
            virtual u32 encoding() const = 0;           // instruction encoding
            virtual void fixit() { }                    // used to fix displacements in branches
            u32& line() { return _line; }               // line number of instruction
            u32& addr() { return _addr; }               // address of instruction
            virtual const string& trace() { return _trace; }    // additional trace content 
            virtual bool match(u08 F) { return false; }         // match encoding to this instruction
            virtual void decode(u32 code) { assert(false); }    // decode the 16-/32-bit encoding
	    virtual vector<operations::operation*> crack() { return vector<operations::operation*>(); }
    };

    class Processor
    {
        private:

        public:
            u08                         _XA;                    // The address of the current exchange package
            word&                       X(u08);                 // X registers in current exchange package
            word&                       P(u32);                 // Physical register
            reg<4>                      mode();                 // mode field of current XPW
            reg<8>                      cond();                 // cond field of current XPW
            reg<12>                     RA();                   // RA field of current XPW
            reg<8>                      XA();                   // XA field of current XPW
            reg<12>                     FL();                   // FL field of current XPW
            reg<20>                     P();                    // P field of current XPW
            L1::cache                   L1D;                    // L1 data cache for this processor
            vector<units::unit>         BRUs;                   // branch units
            vector<units::unit>         FXUs;                   // fixed-point units
            vector<units::unit>         FPUs;                   // floating-point units
            vector<units::unit>         LDUs;                   // load units
            vector<units::unit>         STUs;                   // store units
            map<u32, u32>               line2addr;              // line -> instruction address map
            map<u32, u32>               line2encoding;          // line -> instruction encoding map
            map<u32, u32>               line2len;               // line -> instruction length map
            map<string, u32>            label2line;             // label -> line map
            vector<instruction*>        trace;                  // instruction trace
            vector<u64>                 REGready;               // ready cycle for microarchitected registers
            vector<u64>                 Pready;                 // ready cycle for physical register
            vector<u64>                 Pused;                  // last used cycle for physical register
            u32                         pnext;                  // next physical register to use
            u32                         pfind();                // find a physical register to use
            set<u32>                    pfree;                  // set of free physical registers
            map<u32,u32>                mapper;                 // logical -> physical register mapping
            map<u32,u32>                niap;                   // next instruction address predictor
            u64                         op_count;               // operation count
            u64                         op_nextdispatch;        // next operation dispatch cycle
            u64                         op_lastdispatch;        // last operation dispatch cycle
            u64                         dispatched;             // count of dispatched operations in a cycle
            u64                         op_maxcycle;            // maximum observed completion cycle
            u32                         instr_count;            // Current instruction count
            bool                        instr_target;           // Is the current instruction the target of a branch?
            u32                         instr_forcealign;       // Align this instruction at a word boundary
            bool                        labeling;               // In instruction address labeling mode
            u32                         runningaddr;            // Running instruction address during labeling
            void                        reset(u32);             // Reset a particular processor number
    };
} // namespace CDC8600

#endif // _TYPES_HH
