#ifndef _OPERATIONS_HH_
#define _OPERATIONS_HH_

using namespace std;

namespace CDC8600
{
    namespace operations
    {
	extern vector<u64>	MEMready;	// ready cycles for memory locations

	extern void initmappers();

	class operation
	{
	     protected:
		 u64 _count;	// operation #
		 u64 _ready;    // inputs ready
		 u64 _dispatch;	// dispatch time
                 u64 _issue;    // issue time
                 u64 _complete; // completion time (output ready)

	     public:
		 operation() { }

		 virtual    u64 complete() const { return _complete; }		// operation completion cycle
		 virtual    u64 ready() const = 0; 				// time physical input registers are ready
		 virtual   void target(u64 cycle) = 0;				// update ready time of output physical register
		 virtual   void used(u64 cycle) = 0;				// update used time of input physical register
		 virtual    u64 latency() const = 0; 				// operation latency
		 virtual    u64 throughput() const = 0; 			// operation inverse throughput
		 virtual    u64 tgtused() const = 0;				// time target register was last used
		 virtual string mnemonic() const = 0;				// operation mnemonic
		 virtual string dasm() const = 0;				// operation disassembly with physical registers
		 virtual vector<units::unit>& units() = 0;			// the units that can execute this operation
		 virtual    u64 encode() const { return 0; }			// 64-bit encoding of the operation

		 virtual   void dump(ostream &out)			// operation trace
		 {
		     ios	state(nullptr);
		     state.copyfmt(out);
		     out << setw(74) << setfill(' ');
		     out << " | " << setw( 9) << setfill(' ') << _count;
		     out << " | " << setw(24) << setfill(' ') << dasm();
		     out << " | " << setw( 9) << setfill(' ') << _dispatch;
		     out << " | " << setw( 9) << setfill(' ') << _ready;
		     out << " | " << setw( 9) << setfill(' ') << _issue;
		     out << " | " << setw( 9) << setfill(' ') << _complete;
		     out << endl;
		     out.copyfmt(state);
		 }

		 virtual   void claim(units::unit* fu, u32 start, u32 len)
		 {
		     if (!fu) return;
		     for (u32 i=0; i<len; i++)
		     {
		       	fu->claim(start + i);
		     }
		 }

		 virtual units::unit* firstavailable(u64& start)
		 {
		     if (0 == units().size()) return 0;			// If #units == 0, treat as unbounded resource
		     u32 minstart = UINT32_MAX;
		     u32 minfu = 0;
		     for (u32 fu = 0; fu < units().size(); fu++)	// Check how early can issue to each unit of this type
		     {
			 u32 candidate = start;
			 while (!(units()[fu].isfree(candidate, candidate + throughput()))) candidate++;
			 if (candidate < minstart)
			 {
			     minstart = candidate;
			     minfu = fu;
			 }
		     }
		     start = minstart;
		     return &(units()[minfu]);
		 }

		 virtual   void process(u64 dispatch)		// process this operation, using physical registers
		 {
		     _count = PROC[me()].op_count;		// current instruction count
		     dispatch = max(dispatch, tgtused());	// adjust dispatch time to no earlier than target last used time
		     _dispatch = dispatch;                      // remember dispatch time
		     _ready = ready();				// check ready time for inputs
		     _issue = max(_ready, dispatch);		// account for operation dispatch
		     units::unit* fu = firstavailable(_issue);	// find first unit avaialble starting at candidate issue cycle
		     claim(fu, _issue, throughput());		// claim the unit starting at issue cycle for throughput cycles
		     _complete = _issue + latency();		// time operation completes
		     target(_complete);				// update time output is ready
		     used(_issue);				// update time inputs are used
		     if (tracing) dump(cout);			// generate operation trace using physical registers
		 }
	};

	void process(operation *op);

	class basemapper
	{
	    public:
		virtual void map
		(
		    u32& i,	// target register
		    u32& j,	// source register 
		    u32& k	// source register
		)
		{
		    j = 0;
		    k = 0;
		    i = 0;
		}
	};

	template<typename T>		// For fixed- and floating-point operations;
					// branches, loads, and stores will have the same signature
					// but need to be specialized
	class mapper	: public basemapper
	{
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register 
		    u32& k	// source register
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		    k = PROC[me()].mapper[k];				// physical register for X(k)
		    u08 tgtreg = PROC[me()].pfind();			// find next target physical register
		    PROC[me()].pfree.erase(tgtreg);			// target physical register comes out of the free set
		    PROC[me()].pfree.insert(PROC[me()].mapper[i]);	// old physical register goes back to the free set
		    PROC[me()].mapper[i] = tgtreg;			// new mapping of target logical register to target physical register
		    i = PROC[me()].mapper[i];				// physical register for X(i)
		}
	};

	template<typename T>		// For fixed- and floating-point operations
	void process
	(
	    u08 i,	// target register
	    u08 j,	// source register 
	    u08 k,	// source register
	    u32 K	// immediate field
	)
	{
	    j = PROC[me()].mapper[j];									// physical register for X(j)
	    k = PROC[me()].mapper[k];									// physical register for X(k)
	    u08 tgtreg = PROC[me()].pfind();								// find next target physical register
	    PROC[me()].pfree.erase(tgtreg);								// target physical register comes out of the free set
	    PROC[me()].pfree.insert(PROC[me()].mapper[i]);						// old physical register goes back to the free set
	    PROC[me()].mapper[i] = tgtreg;								// new mapping of target logical register to target physical register
	    i = PROC[me()].mapper[i];									// physical register for X(i)
	    process(new T(i, j, k, K));									// process new operation
	}

	template<typename T>		// For branches
	void process
	(
	    u32 	K,	// immediate displacement
	    u08 	j,	// compare flags register
	    u32		addr,	// branch instruction address
	    bool 	taken,	// was the branch taken?
	    string 	label	// branch target
	)
	{
	    process(new T(K, PROC[me()].mapper[j], addr, taken, label));
	}

	template<typename T>		// For loads and stores
	void process
	(
	    u08		j,
	    u08		k,
	    u32		addr
	);

	class FXop : public operation 			// Fixed-point operation
	{
	    public:
		u08	_i;
		u08	_j;
		u08	_k;
		u32 	_K;
		u64 tgtused() const { return PROC[me()].Pused[_i]; }
		vector<units::unit>& units() { return PROC[me()].FXUs; }
		FXop(u08 i, u08 j, u08 k, u32 K) { _i = i; _j = j; _k = k; _K = K; }
	};

	class FPop : public operation			// Floating-point operation
	{
	    public:
		u08	_i;
		u08	_j;
		u08	_k;
		u32 	_K;
		u64 tgtused() const { return PROC[me()].Pused[_i]; }
		vector<units::unit>& units() { return PROC[me()].FPUs; }
		FPop(u08 i, u08 j, u08 k, u32 K) { _i = i; _j = j; _k = k; _K = K; }
	};

	class LDop : public operation			// Load operation
	{
	    public:
		u08	_j;
		u08	_k;
		u32	_addr;
		vector<units::unit>& units() { return PROC[me()].LDUs; }
		LDop(u08 j, u08 k, u32 addr) { _j = j; _k = k; _addr = addr; }
		u64 tgtused() const { return PROC[me()].Pused[_j]; }
	};

	class STop : public operation			// Store operation
	{
	    public:
		u08	_j;
		u08	_k;
		u32	_addr;
		vector<units::unit>& units() { return PROC[me()].STUs; }
		STop(u08 j, u08 k, u32 addr) { _j = j; _k = k; _addr = addr; }
		u64 tgtused() const { return 0; }
	};

	class BRop : public operation			// Branch operation
	{
	    public:
		u32	_addr;
		u08	_j;
		u32	_K;
		bool	_taken;
		string	_label;
		vector<units::unit>& units() { return PROC[me()].BRUs; }
		BRop(u32 K, u08 j, u32 addr, bool taken, string label) { _K = K; _j = j; _taken = taken; _addr = addr; _label = label; }
		u64 tgtused() const { return 0; }
	};

	class xKi : public FXop
	{
	    public:
		xKi(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return 0; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }	// update ready cycle of target physical register
		void used(u64 cycle) { }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "xKi"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_K) + ")"; }
		u64 encode() const { return ((u64)0x10 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
	};

	class idzkj : public FXop
	{
	    public:
		idzkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return PROC[me()].Pready[_k]; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }	// update ready cycle of target physical register
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idzkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x17 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
	};

	class idjkj : public FXop
	{
	    public:
		idjkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_K) + ")"; }
	};

        class idjki : public FXop
	{
	    public:
		idjki(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idjki"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class isjkj : public FXop
	{
	    public:
		isjkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]  ); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "isjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x12 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
	};

	class isjki : public FXop
	{
	    public:
		isjki(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "isjki"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class agen : public FXop
	{
	    public:
		agen(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return max(max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]), max(PROC[me()].Pready[PROC[me()].mapper[params::micro::RA]], PROC[me()].Pready[PROC[me()].mapper[params::micro::FL]])); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "agen"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class rdw : public LDop
	{
	    public:
		rdw(u08 j, u08 k, u32 addr) : LDop(j, k, addr) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], MEMready[_addr]); }
		void target(u64 cycle) { PROC[me()].Pready[_j] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); }
		u64 latency() const { return PROC[me()].L1D.loadhit(_addr, _issue) ? params::L1::latency : params::MEM::latency; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "rdw"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ", " + to_string(_addr) + ")"; }
	};

	template<> void process<rdw>(u08, u08, u32);
	   
	class stw : public STop
	{
	    public:
		stw(u08 j, u08 k, u32 addr) : STop(j, k, addr) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { MEMready[_addr] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return PROC[me()].L1D.storehit(_addr, _issue) ? params::L1::latency : params::MEM::latency; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "stw"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ", " + to_string(_addr) + ")"; }
	};

	template<> void process<stw>(u08, u08, u32);

	class ipjkj : public FXop
	{
	    public:
		ipjkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "ipjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class cmpz : public FXop
	{
	    public:
		cmpz(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }	// update ready cycle of target physical register
		void used(u64 cycle) { PROC[me()].Pused[_j] = cycle; }		// update used cycle of source physical register
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "cmpz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ")"; }
	};

	class cmp : public FXop
	{
	    public:
		cmp(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_j], PROC[me()].Pready[_k]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "cmp"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class jmp : public BRop
	{
	    public:
		jmp(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return 0; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmp"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ")"; }
	};

	class jmpk : public BRop
	{
	    public:
		jmpk(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpk"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_K) + ")"; }
	};

	class jmpz : public BRop
	{
	    public:
		jmpz(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
	};

        class jmpnz : public BRop
	{
	    public:
		jmpnz(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpnz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
	};

	class jmpp : public BRop
	{
	    public:
		jmpp(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpp"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
	};

	class jmpn : public BRop
	{
	    public:
		jmpn(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpn"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
	};

	class bb : public BRop
	{
	    public:
		bb(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "bb"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
	};

        class fadd : public FPop
        {
	    public:
		fadd(u08 i, u08 j, u08 k, u08 K) : FPop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "fadd"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
        };

        class fmul : public FPop
        {
	    public:
		fmul(u08 i, u08 j, u08 k, u08 K) : FPop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "fmul"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
        };

        class fsub : public FPop
        {
	    public:
		fsub(u08 i, u08 j, u08 k, u08 K) : FPop(i, j, k, K) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "fsub"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
        };
    } // namespace operations
} // namespace CDC8600

#endif // _OPERATIONS_HH_
