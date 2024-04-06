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
		 virtual pipes::pipe_t pipe() const { return pipes::FXArith; }	// execution pipe for this operation
		 virtual pipes::dep_t dep() const { return pipes::no_dep; }	// type of dependence for this operation

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
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = 0;
		    k = 0;
		    i = 0;
		}

		virtual pipes::pipe_t pipe
		(
		)
		{
		    return pipes::FXArith;
		}

		virtual pipes::dep_t dep
		(
		)
		{
		    return pipes::no_dep;
		}
	};

	template<typename T>		// For fixed- and floating-point operations;
					// branches, loads, and stores will have the same signature
					// but need to be specialized
	class mapper	: public basemapper
	{
	    private:
		T	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register 
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		    k = PROC[me()].mapper[k];				// physical register for X(k)
		    u32 tgtreg = PROC[me()].pfind();			// find next target physical register
		    PROC[me()].pfree.erase(tgtreg);			// target physical register comes out of the free set
		    PROC[me()].precycle.insert(PROC[me()].mapper[i]);	// old physical register goes back to the recyclable set
		    PROC[me()].Plastop[PROC[me()].mapper[i]] = op;	// old physical register will be recycled with this operation finishes
		    PROC[me()].Pfull[tgtreg] = false;			// the target register is now empty
		    // cout << "Physical register " << tgtreg << " is now empty" << endl;
		    PROC[me()].mapper[i] = tgtreg;			// new mapping of target logical register to target physical register
		    i = PROC[me()].mapper[i];				// physical register for X(i)
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}

		pipes::dep_t dep()
		{
		    return _op.dep();
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
		u08	_i;
		u08	_k;
		u32	_K;
		bool	_taken;
		string	_label;
		vector<units::unit>& units() { return PROC[me()].BRUs; }
		BRop(u32 K, u08 j, u32 addr, bool taken, string label) { _K = K; _j = j; _taken = taken; _addr = addr; _label = label; _i = 0; _k = 0; }
		u64 tgtused() const { return 0; }
	};

	class xKi : public FXop
	{
	    public:
		xKi(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		xKi() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return 0; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }	// update ready cycle of target physical register
		void used(u64 cycle) { }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "xKi"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_K) + ")"; }
		u64 encode() const { return ((u64)0x10 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::no_dep; }
		pipes::pipe_t pipe() { return pipes::FXLogic; }
	};

	class idzkj : public FXop
	{
	    public:
		idzkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		idzkj() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return PROC[me()].Pready[_k]; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }	// update ready cycle of target physical register
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idzkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x17 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::k_dep; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
	};

	class idjkj : public FXop
	{
	    public:
		idjkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		idjkj() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_K) + ")"; }
		u64 encode() const { return ((u64)0x13 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
	};

        class idjki : public FXop
	{
	    public:
		idjki(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		idjki() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idjki"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x70 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::jk_dep; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
	};

	class isjkj : public FXop
	{
	    public:
		isjkj(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		isjkj() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]  ); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "isjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x12 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
	};

	class isjki : public FXop
	{
	    public:
		isjki(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		isjki() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "isjki"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x6 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::jk_dep; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
	};

	class agen : public FXop
	{
	    public:
		agen(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		agen() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return max(max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]), max(PROC[me()].Pready[PROC[me()].mapper[params::micro::RA]], PROC[me()].Pready[PROC[me()].mapper[params::micro::FL]])); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "agen"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0xb3 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
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
		stw(u08 j, u08 k) : STop(j, k, 0) { }
		stw() : STop(0, 0, 0) { }
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
		ipjkj() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return max(PROC[me()].Pready[_k], PROC[me()].Pready[_j]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "ipjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0x0d << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::pipe_t pipe() { return pipes::FXMul; }
		pipes::dep_t dep() { return pipes::jk_dep; }
	};

	class cmpz : public FXop
	{
	    public:
		cmpz(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		cmpz() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }	// update ready cycle of target physical register
		void used(u64 cycle) { PROC[me()].Pused[_j] = cycle; }		// update used cycle of source physical register
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "cmpz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ")"; }
		u64 encode() const { return ((u64)0xb1 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
		pipes::dep_t dep() { return pipes::j_dep; }
	};

	class cmp : public FXop
	{
	    public:
		cmp(u08 i, u08 j, u08 k, u32 K) : FXop(i, j, k, K) { }
		cmp() : FXop(0, 0, 0, 0) { }
		u64 ready() const { return max(PROC[me()].Pready[_j], PROC[me()].Pready[_k]); }
		void target(u64 cycle) { PROC[me()].Pready[_i] = cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_k] = max(PROC[me()].Pused[_k], cycle); PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "cmp"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
		u64 encode() const { return ((u64)0xb2 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::pipe_t pipe() { return pipes::FXArith; }
	};

	class jmp : public BRop
	{
	    public:
		jmp(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		jmp(u32 K) : BRop(K, 0, 0, 0, "") {}
		jmp() : BRop(0, 0, 0, 0, "") {}
		u64 ready() const { return 0; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmp"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ")"; }
		u64 encode() const { return ((u64)0x30 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::pipe_t pipe() { return pipes::BR; }
		pipes::dep_t dep() {return pipes::no_dep;}
	};

	class jmpk : public BRop
	{
	    public:
		jmpk(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		jmpk(u32 K, u08 j) : BRop(K, j, 0, 0, "") { }
		jmpk() : BRop(0, 0, 0, 0, "") {}
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpk"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_K) + ")"; }
		u64 encode() const { return ((u64)0x3C << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::BR; }
	};

	class jmpz : public BRop
	{
	    public:
		jmpz(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		jmpz(u32 K, u08 j) : BRop(K, j, 0, 0, "") { }
		jmpz() : BRop(0, 0, 0, 0, "") { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
		u64 encode() const { return ((u64)0x34 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::BR; }
	};

	class jmpnz : public BRop
	{
	    public:
		jmpnz(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		jmpnz(u32 K, u08 j) : BRop(K, j, 0, 0, "") {}
		jmpnz() : BRop(0, 0, 0, 0, "") { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpnz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
		u64 encode() const { return ((u64)0x35 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::BR; }
	};

	class jmpp : public BRop
	{
	    public:
		jmpp(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		jmpp(u32 K, u08 j) : BRop(K, j, 0, 0, "") { }
		jmpp() : BRop(0, 0, 0, 0, "") { }
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpp"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
		u64 encode() const { return ((u64)0x36 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::BR; }
	};

	template<>
	class mapper<jmp>	: public basemapper
	{
	    private:
		jmpz	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
	};

	template<>
	class mapper<jmpnz>	: public basemapper
	{
	    private:
		jmpz	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
	};

	template<>
	class mapper<jmpk>	: public basemapper
	{
	    private:
		jmpz	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
	};


	template<>
	class mapper<jmpz>	: public basemapper
	{
	    private:
		jmpz	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
	};

	template<>
	class mapper<jmpp>	: public basemapper
	{
	    private:
		jmpp	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
	};

	class jmpn : public BRop
	{
	    public:
		jmpn(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		jmpn(u32 K, u08 j) : BRop(K, j, 0, 0, "") {}
		jmpn() : BRop(0, 0, 0, 0, "") {}
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpn"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
		u64 encode() const { return ((u64)0x37 << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::j_dep; }
		pipes::pipe_t pipe() { return pipes::BR; }
	};

	class bb : public BRop
	{
	    public:
		bb(u32 K, u08 j, u32 addr, bool taken, string label) : BRop(K, j, addr, taken, label) { }
		bb(u32 K, u08 j) : BRop(K, j, 0, 0, "") {}
		bb() : BRop(0, 0, 0, 0, "") {}
		u64 ready() const { return PROC[me()].Pready[_j]; }
		void target(u64 cycle) { PROC[me()].op_nextdispatch = prediction(_addr, _taken, _label) ? PROC[me()].op_nextdispatch : cycle; }
		void used(u64 cycle) { PROC[me()].Pused[_j] = max(PROC[me()].Pused[_j], cycle); }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "bb"; }
		string dasm() const { return mnemonic() + "(" + to_string(_K) + ", " + to_string(_j) + ")"; }
		u64 encode() const { return ((u64)0xB << 56) | ((u64)_i << 44) | ((u64)_j << 32) | ((u64)_k << 20) | _K; }
		pipes::dep_t dep() { return pipes::jk_dep; }
		pipes::pipe_t pipe() { return pipes::BR; }
	};

	template<>
	class mapper<jmpn>	: public basemapper
	{
	    private:
		jmpp	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
	};

	template<>
	class mapper<bb>	: public basemapper
	{
	    private:
		jmpp	_op;
	    public:
		void map
		(
		    u32& i,	// target register
		    u32& j,	// source register
		    u32& k,	// source register
		    u32  op	// operation #
		)
		{
		    j = PROC[me()].mapper[j];				// physical register for X(j)
			k = PROC[me()].mapper[k];
		}

		pipes::pipe_t pipe()
		{
		    return _op.pipe();
		}
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
