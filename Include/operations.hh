#ifndef _OPERATIONS_HH_
#define _OPERATIONS_HH_

using namespace std;

namespace CDC8600
{
    namespace operations
    {
	extern u64 		count;		// operation count
	extern u64 		nextdispatch;	// next dispatch cycle
	extern u64		maxcycle;	// maximum completion cycle
	extern vector<u64>	MEMready;	// ready cycles for memory locations
	extern vector<u64>	REGready;	// ready cycles for registers

	class operation
	{
	     private:
		 u64 _count;	// operation #
		 u64 _ready;    // inputs ready
		 u64 _dispatch;	// dispatch time
                 u64 _issue;    // issue time
                 u64 _complete; // completion time (output ready)

	     public:
		 operation() { }

		 virtual    u64 complete() const { return _complete; }	// operation completion cycle
		 virtual    u64 ready() const = 0; 			// time inputs are ready
		 virtual    u64 target(u64 cycle) = 0;			// update ready time of output
		 virtual    u64 latency() const = 0; 			// operation latency
		 virtual    u64 throughput() const = 0; 		// operation inverse throughput
		 virtual string mnemonic() const = 0;			// operation mnemonic
		 virtual string dasm() const  = 0;			// operation disassembly
		 virtual vector<units::unit>& units() = 0;		// the units that can execute this operation

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

		 virtual   bool process(u64 dispatch)		// process this operation
		 {
		     _count = count;				// current instruction count
		     _dispatch = dispatch;                      // remember dispatch time
		     _ready = ready();				// check ready time for inputs
		     _issue = max(_ready, dispatch);		// account for operation dispatch
		     units::unit* fu = firstavailable(_issue);	// find first unit avaialble starting at candidate issue cycle
		     claim(fu, _issue, throughput());		// claim the unit starting at issue cycle for throughput cycles
		     _complete = _issue + latency();		// time operation completes
		     target(_complete);				// update time output is ready
		     if (tracing) dump(cout);			// generate operation trace
		 }
	};

	bool process(operation *op);

	class FXop : public operation
	{
	    public:
		vector<units::unit>& units() { return units::FXUs; }
	};

	class FPop : public operation
	{
	    public:
		vector<units::unit>& units() { return units::FPUs; }
	};

	class LDop : public operation
	{
	    public:
		vector<units::unit>& units() { return units::LDUs; }
	};

	class STop : public operation
	{
	    public:
		vector<units::unit>& units() { return units::STUs; }
	};

	class BRop : public operation
	{
	    public:
		vector<units::unit>& units() { return units::BRUs; }
	};

	class xkj : public FXop
	{
	    private:
	        u08 _j;
	        u08 _k;

	    public:
		xkj(u08 j, u08 k) { _j = j; _k = k; }
		u64 ready() const { return 0; }
		u64 target(u64 cycle) { REGready[_j] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "xkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class idzkj : public FXop
	{
	    private:
		u08	_j;
		u08	_k;

	    public:
		idzkj(u08 j, u08 k) { _j = j; _k = k; }
		u64 ready() const { return REGready[_k]; }
		u64 target(u64 cycle) { REGready[_j] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idzkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class idjkj : public FXop
	{
	    private:
		u08	_j;
		u08	_k;

	    public:
		idjkj(u08 j, u08 k) { _j = j; _k = k; }
		u64 ready() const { return REGready[_j]; }
		u64 target(u64 cycle) { REGready[_j] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "idjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class isjkj : public FXop
	{
	    private:
		u08	_j;
		u08	_k;

	    public:
		isjkj(u08 j, u08 k) { _j = j; _k = k; }
		u64 ready() const { return max(REGready[_k], REGready[_j]); }
		u64 target(u64 cycle) { REGready[_j] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "isjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class isjki : public FXop
	{
	    private:
		u08	_i;
		u08	_j;
		u08	_k;

	    public:
		isjki(u08 i, u08 j, u08 k) { _i = i; _j = j; _k = k; }
		u64 ready() const { return max(REGready[_k], REGready[_j]); }
		u64 target(u64 cycle) { REGready[_i] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "isjki"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class agen : public FXop
	{
	    private:
		u08	_i;
		u08	_j;
		u08	_k;

	    public:
		agen(u08 i, u08 j, u08 k) { _i = i; _j = j; _k = k; }
		u64 ready() const { return max(max(REGready[_k], REGready[_j]), max(REGready[params::micro::RA], REGready[params::micro::FL])); }
		u64 target(u64 cycle) { REGready[_i] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 2; }
		string mnemonic() const { return "agen"; }
		string dasm() const { return mnemonic() + "(" + to_string(_i) + ", " + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class rdw : public LDop
	{
	    private:
		u08	_j;
		u08	_k;
		u32	_addr;

	    public:
		rdw(u08 j, u08 k, u32 addr) { _j = j; _k = k; _addr = addr; }
		u64 ready() const { return max(REGready[_k], MEMready[_addr]); }
		u64 target(u64 cycle) { REGready[_j] = cycle; }
		u64 latency() const { return 30; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "rdw"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_addr) + ")"; }
	};

	class stw : public STop
	{
	    private:
		u08	_j;
		u08	_k;
		u32	_addr;

	    public:
		stw(u08 j, u08 k, u32 addr) { _j = j; _k = k; _addr = addr; }
		u64 ready() const { return max(REGready[_k], REGready[_j]); }
		u64 target(u64 cycle) { MEMready[_addr] = cycle; }
		u64 latency() const { return 30; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "stw"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_addr) + ")"; }
	};

	class ipjkj : public FXop
	{
	    private:
		u08	_j;
		u08	_k;

	    public:
		ipjkj(u08 j, u08 k) { _j = j; _k = k; }
		u64 ready() const { return max(REGready[_k], REGready[_j]); }
		u64 target(u64 cycle) { REGready[_j] = cycle; }
		u64 latency() const { return 2; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "ipjkj"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class cmpz : public FXop
	{
	    private:
	        u08 _j;

	    public:
		cmpz(u08 j) { _j = j; }
		u64 ready() const { return REGready[_j]; }
		u64 target(u64 cycle) { REGready[params::micro::CMPFLAGS] = cycle; }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "cmpz"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ")"; }
	};

	class jmp : public BRop
	{
	    private:

	    public:
		jmp() { }
		u64 ready() const { return 0; }
		u64 target(u64 cycle) { nextdispatch = cycle; }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmp"; }
		string dasm() const { return mnemonic() + "(" + ")"; }
	};

	class jmpk : public BRop
	{
	    private:
		u08	_j;
		u08	_k;

	    public:
		jmpk(u08 j, u08 k) { _j = j; _k = k; }
		u64 ready() const { return REGready[_j]; }
		u64 target(u64 cycle) { nextdispatch = cycle; }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpk"; }
		string dasm() const { return mnemonic() + "(" + to_string(_j) + ", " + to_string(_k) + ")"; }
	};

	class jmpz : public BRop
	{
	    private:

	    public:
		jmpz() { }
		u64 ready() const { return REGready[params::micro::CMPFLAGS]; }
		u64 target(u64 cycle) { nextdispatch = cycle; }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpz"; }
		string dasm() const { return mnemonic() + "(" + ")"; }
	};

	class jmpp : public BRop
	{
	    private:

	    public:
		jmpp() { }
		u64 ready() const { return REGready[params::micro::CMPFLAGS]; }
		u64 target(u64 cycle) { nextdispatch = cycle; }
		u64 latency() const { return 1; }
		u64 throughput() const { return 1; }
		string mnemonic() const { return "jmpp"; }
		string dasm() const { return mnemonic() + "(" + ")"; }
	};
    } // namespace operations
} // namespace CDC8600

#endif // _OPERATIONS_HH_
