#include<iostream>
#include<iomanip>
#include<CDC8600.hh>
#include<ISA.hh>

namespace CDC8600
{
    vector<word> 	MEM(params::MEM::N);
    uint32_t		FreeMEM;
    Processor		PROC;

    word& Processor::X
    (
        uint8_t	i
    )
    {
	return MEM[_XA*32 + i];
    }

    reg<20> Processor::P
    (
    )
    {
	return reg<20>(_XA*32+16, 0);
    }

    reg<12> Processor::FL
    (
    )
    {
	return reg<12>(_XA*32+16, 20);
    }

    reg<8> Processor::XA
    (
    )
    {
	return reg<8>(_XA*32+16, 32);
    }

    reg<12> Processor::RA
    (
    )
    {
	return reg<12>(_XA*32+16, 40);
    }

    reg<8> Processor::cond
    (
    )
    {
	return reg<8>(_XA*32+16, 52);
    }

    reg<4> Processor::mode
    (
    )
    {
	return reg<4>(_XA*32+16, 60);
    }

    template<int n> 
    u64	reg<n>::u()
    {
	u64 v = MEM[_loc].u();
	v = v >> _first;
	v = v & ((1UL << n) - 1);
	return v;
    }

    template<int n>
    reg<1> reg<n>::operator()
    (
        uint8_t bit
    )
    {
	assert(false);
    }

    template<int n>
    reg<n>& reg<n>::operator=
    (
        bool b
    )
    {
	assert(false);
    }

    template<int n>
    reg<n>& reg<n>::operator=
    (
        u64 u
    )
    {
	assert (u < (1UL << n));
	u64 v = MEM[_loc].u();
	u64 left = v >> (_first + n);
	u64 right = v & ((1UL << _first) - 1);
	MEM[_loc].u() = (left << (_first + n)) + (u << _first) + right;
	return *this;
    }

    void reset
    (
    )
    {
	for (u32 i = 0; i < params::MEM::N; i++) MEM[i].u() = 0;			// Zero the memory
	for (u32 i = 0; i < params::MEM::N; i++) operations::MEMready[i] = 0;		// Zero the ready time for all`memory locations
	for (u32 i = 0; i < params::micro::nregs; i++) operations::REGready[i] = 0;	// Zero the ready time for all microarchitected registers
	FreeMEM = 4*8192;								// Heap starts in page 4
	PROC._XA = 4;									// User context for PROC[0] is in frame 4
	PROC.FL() = (u64)(29 * 8192 / 256);						// User data memory is 29 pages
	PROC.RA() = (u64)( 3 * 8192 / 256);						// User data memory begins in page 3
	instructions::count = 0;							// Instruction count starts at 0
	instructions::target = true;							// First instruction is target of a branch
	instructions::forcealign = 0;							// Force instruction address to align to word boundary
	operations::count = 0;								// Operation count starts at 0
	operations::nextdispatch = 0;							// Start dispatching operations at cycle 0
	operations::maxcycle = 0;							// Maximum completion time observed
	for (u32 i=0; i<trace.size(); i++) delete trace[i];				// Delete all previous instructions
	trace.clear();									// Clear the trace
	line2addr.clear();								// Clear line -> address map
	line2encoding.clear();								// Clear line -> encoding map
	line2len.clear();								// Clear line -> len map
	for (u32 i=0; i < params::micro::nBRUs; i++) units::BRUs[i].clear();		// Cluear usage of BRUs
	for (u32 i=0; i < params::micro::nFXUs; i++) units::FXUs[i].clear();		// Cluear usage of FXUs
	for (u32 i=0; i < params::micro::nFPUs; i++) units::FPUs[i].clear();		// Cluear usage of FPUs
	for (u32 i=0; i < params::micro::nLDUs; i++) units::LDUs[i].clear();		// Cluear usage of LDUs
	for (u32 i=0; i < params::micro::nSTUs; i++) units::STUs[i].clear();		// Cluear usage of STUs
    }

    void *memalloc
    (
        u64	N
    )
    {
	void *addr = &(MEM[FreeMEM]);
	FreeMEM += N;
	return addr;
    }

    call0 Call(void (*f)())
    {
	return call0(f);
    }

    template class reg<12>;
    template class reg<8>;
    template class reg<4>;
    template class reg<1>;
    template class reg<20>;

    bool			tracing = false;		// Trace during the simulation?
    vector<instruction*> 	trace;				// instruction trace
    map<u32, u32>		line2addr;			// line -> address map
    map<u32, u32>		line2encoding;			// line -> encoding map
    map<u32, u32>		line2len;			// line -> instruction length map
    map<string, u32>		label2line;			// label -> line map

    namespace instructions
    {
	u32	count;
	bool 	target;
	u32	forcealign;
	bool	labeling;
	u32	runningaddr;
    } // namespace instructions

    void addlabel
    (
        string label,
	u32    line
    )
    {
	if (label2line.count(label)) return;
	label2line[label] = line;
    }

    template<typename T> void label
    (
        T (*f)()
    )
    {
	line2addr.clear();
	line2encoding.clear();
	line2len.clear();
	label2line.clear();
	instructions::labeling = true;
	instructions::runningaddr = 0;
	f();
	instructions::labeling = false;
    }
    
    template void label(void (*f)());
    template void label(f64  (*f)());
    template void label(i64  (*f)());
    template void label(c128 (*f)());

    void labeladdr
    (
        instruction*	instr
    )
    {
	if (0 == instructions::runningaddr) instructions::runningaddr = instr->line() * 8;
	if (line2addr.count(instr->line())) return;

	if (instructions::runningaddr % instr->len())
	{
		cout << "Instruction at line # " << instr->line() 
		     << " has length " << instr->len() << " bytes, but has a byte offset of "
		     << (instructions::runningaddr % 8) << endl;
		assert(false);
	}
	line2addr[instr->line()] = instructions::runningaddr;
	line2encoding[instr->line()] = instr->encoding();
	line2len[instr->line()] = instr->len();
	instructions::runningaddr += instr->len();
    }

    void assignaddr
    /*
     * Assign a byte (not word!) address to an instruction.
     */
    (
        instruction*	instr,		// pointer to instruction
	bool		target		// is this a target of a branch?
    )
    {
	if (line2addr.count(instr->line()))				// line already in map?
	{
	    assert(instr->encoding() == line2encoding[instr->line()]);	// encoding match?
	    assert(instr->len()      == line2len[instr->line()]);	// instruction length match?
	}
	else if (instructions::forcealign)				// this line has a label, so we must force a word alignment
	{
	    line2addr[instr->line()] = instr->line() * 8;		// this is a byte address
	    line2encoding[instr->line()] = instr->encoding();
	    line2len[instr->line()]      = instr->len();
	    instructions::forcealign = 0;
	}
	else if (line2addr.count(instr->line() - 1)) 			// is the previous line in the map?
	{
	    line2addr[instr->line()]     = line2addr[instr->line() - 1] + line2len[instr->line() - 1];
	    line2encoding[instr->line()] = instr->encoding();
	    line2len[instr->line()]      = instr->len();
	}
	else								// new line
	{
	    line2addr[instr->line()] = instr->line() * 8;		// this is a byte address
	    line2encoding[instr->line()] = instr->encoding();
	    line2len[instr->line()]      = instr->len();
	}

	if (target)							// is this the target of a branch
	{
	    if (line2addr[instr->line()] % 8)				// is the target address word aligned?
	    {
		cout << "Instruction at line # " << instr->line() << " is the target of a branch but has a byte offset of " << (line2addr[instr->line()] % 8) << endl;
		assert(false);
	    }
	}
    }

    void dumpheader
    (
    )
    {
	cout << "  instr #";
	cout << " |    line #";
	cout << " |                   instruction ";
	cout << " |  address";
	cout << " | encoding ";
	cout << endl;

	cout << "----------";
	cout << "+-----------";
	cout << "+--------------------------------";
	cout << "+----------";
	cout << "+---------";
	cout << endl;
    }

    void dumpheaderop
    (
    )
    {
	cout << "  instr #";
	cout << " |    line #";
	cout << " |             instruction ";
	cout << " |  address";
	cout << " | encoding ";
	cout << " |     op # ";
	cout << " |               operation ";
	cout << " | dispatch ";
	cout << " |    ready ";
	cout << " |    issue ";
	cout << " | complete ";
	cout << endl;

	cout << "----------";
	cout << "+-----------";
	cout << "+--------------------------";
	cout << "+----------";
	cout << "+-----------";
	cout << "+-----------";
	cout << "+--------------------------";
	cout << "+-----------";
	cout << "+-----------";
	cout << "+-----------";
	cout << "+----------";
	cout << endl;
    }

    void dump
    (
        u32		i,
        instruction* 	instr
    )
    {
	cout << setw( 9) << i;
	cout << " | " << setw( 9) << instr->line();
	cout << " | " << setw(24) << instr->dasm();
	cout << " | " << setfill('0') << setw( 8) << hex << line2addr[instr->line()] << dec << setfill(' ');
	if (instr->len() == 4) cout << " | "     << setfill('0') << setw(8) << hex << instr->encoding() << dec << setfill(' ');
	if (instr->len() == 2) cout << " |     " << setfill('0') << setw(4) << hex << instr->encoding() << dec << setfill(' '); 
	cout << endl;
    }

    void dump
    (
        vector<instruction*>& T
    )
    {
	dumpheader();

	for (u32 i=0; i<T.size(); i++)
	{
	    dump(i, T[i]);
	}
    }

    bool process
    (
        instruction* 	instr,
	u32 		line
    )
    {
	if (instructions::labeling)
	{
	    instr->line() = line;
	    labeladdr(instr);
	    delete instr;
	    return false;
	}
	instr->line() = line;				// save instruction line number in source file
	assignaddr(instr, instructions::target);	// assign an address to this instruction
	instr->fixit();					// fix displacement in branches
	instructions::target = instr->execute();	// execute the instructions, remember if a branch is being taken
	trace.push_back(instr);				// save instruction to trace
	if (tracing)					// run-time tracing
	{
	    if (0 == instructions::count) dumpheaderop();
	    dump(instructions::count, instr);
	}
	instr->ops();					// process the internal operations of this instruction
	instructions::count++;				// increment instruction counter
	return instructions::target;			// return true if a branch is taken
    }

    namespace units
    {
	vector<unit>	BRUs(params::micro::nBRUs);	// branch units
	vector<unit>	FXUs(params::micro::nFXUs);	// fixed-point units
	vector<unit>	FPUs(params::micro::nFPUs);	// floating-point units
	vector<unit>	LDUs(params::micro::nLDUs);	// load units
	vector<unit>	STUs(params::micro::nSTUs);	// store units
    }; // namespace units

    namespace operations
    {
	vector<u64>		MEMready(params::MEM::N);	// ready cycle for memory locations
	vector<u64>		REGready(params::micro::nregs); // ready cycle for microarchitected registers
	u64 			count;				// operation count
	u64 			nextdispatch;			// next operation dispatch cycle
	u64			maxcycle;			// maximum observed completion cycle

	bool process
	(
	    operation* op
	)
	{
	    op->process(nextdispatch);			// process this operation
	    count++;					// update operation count
	    maxcycle = max(maxcycle, op->complete());	// update maximum observed completion time
	}
    } // namespace operations
} // namespace 8600
