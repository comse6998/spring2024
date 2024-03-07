#include<iostream>
#include<iomanip>
#include<CDC8600.hh>
#include<ISA.hh>
#ifdef _OPENMP
#include<omp.h>
#endif

namespace CDC8600
{
    vector<word> 	MEM(params::MEM::N);
    uint32_t		FreeMEM;
    vector<Processor>	PROC(params::Proc::N);

    u32	me()
    {
#ifdef _OPENMP
	return omp_get_thread_num();
#else
	return 0;
#endif
    }

    u32 nump()
    {
#ifdef _OPENMP
	return omp_get_num_threads();
#else
	return 1;
#endif
    }

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

    namespace L1
    {
	cache::cache
	(
	) : _sets(params::L1::nsets)
	{
	}

	void cache::reset()
	{
	    for (u32 i=0; i < params::L1::nsets; i++) _sets[i].reset();
	}

	cacheset::cacheset
	(
	) : _valid(params::L1::nways), _tag(params::L1::nways), _used(params::L1::nways)
	{
	}

	void cacheset::reset()
	{
	    for (u32 i=0; i < params::L1::nways; i++) _valid[i] = false;
	}

	bool cache::loadhit
	(
	    u32 addr,
	    u64 cycle
	)
	{
	    u32 lineaddr = addr / params::L1::linesize;
	    cacheset &candidate = _sets[lineaddr % params::L1::nsets];
	    for (u32 i=0; i < params::L1::nways; i++)
		if (candidate.valid(i) && (candidate.tag(i) == lineaddr)) { candidate.used(i) = cycle; return true; } // hit

	    // must allocate on a load miss
	    for (u32 i=0; i < params::L1::nways; i++)
	    {
		if (!candidate.valid(i))
		{
		    // invalid entry, can use this one
		    candidate.validate(i);
		    candidate.tag(i) = lineaddr;
		    candidate.used(i) = cycle;
		    return false;
		}
	    }

	    // no invalid entry in this set, must find the LRU
	    u32 lruentry = 0;
	    u64 lrutime = UINT64_MAX;
	    for (u32 i=0; i < params::L1::nways; i++)
	    {
		if (candidate.used(i) < lrutime)
		{
		    lrutime = candidate.used(i);
		    lruentry = i;
		}
	    }
	    candidate.tag(lruentry) = lineaddr;
	    candidate.used(lruentry) = cycle;
	    return false;
	}
	
	bool cache::storehit
	(
	    u32 addr,
	    u64 cycle
	)
	{
	    u32 lineaddr = addr / params::L1::linesize;
	    cacheset &candidate = _sets[lineaddr % params::L1::nsets];
	    for (u32 i=0; i < params::L1::nways; i++)
		if (candidate.valid(i) && (candidate.tag(i) == lineaddr)) { candidate.used(i) = cycle; return true; } // hit
    	    // do not allocate on store miss
	    return false;
	}
    }

    void reset
    (
    )
    {
	for (u32 i = 0; i < params::MEM::N; i++) MEM[i].u() = 0;			// Zero the memory
	for (u32 i = 0; i < params::MEM::N; i++) operations::MEMready[i] = 0;		// Zero the ready time for all`memory locations
	FreeMEM = 4*8192;								// Heap starts in page 4
	for (u32 i = 0; i < params::Proc::N; i++) PROC[i].reset(i);			// Reset the processors
    }

    void Processor::reset
    (
     	u32 id
    )
    {
	REGready.resize(params::micro::nregs); for (u32 i = 0; i < params::micro::nregs; i++) REGready[i] = 0;	// Zero the ready time for all microarchitected registers
	_XA = 4 + id;												// User context for PROC[0] is in frame 4
	FL() = (u64)(29 * 8192 / 256);										// User data memory is 29 pages
	RA() = (u64)( 3 * 8192 / 256);										// User data memory begins in page 3
	instr_count = 0;											// Instruction count starts at 0
	instr_target = true;											// First instruction is target of a branch
	instr_forcealign = 0;											// Force instruction address to align to word boundary
	op_count = 0;												// Operation count starts at 0
	op_lastdispatch = 0;											// Cycle of last dispatch = 0
	op_nextdispatch = 0;											// Start dispatching operations at cycle 0
	dispatched = 0;												// Dispatched operations in cycle = 0
	op_maxcycle = 0;											// Maximum completion time observed
	for (u32 i=0; i<trace.size(); i++) delete trace[i];							// Delete all previous instructions
	trace.clear();												// Clear the trace
	line2addr.clear();											// Clear line -> address map
	line2encoding.clear();											// Clear line -> encoding map
	line2len.clear();											// Clear line -> len map
	BRUs.resize(params::micro::nBRUs); for (u32 i=0; i < params::micro::nBRUs; i++) BRUs[i].clear();	// Clear usage of BRUs
	FXUs.resize(params::micro::nFXUs); for (u32 i=0; i < params::micro::nFXUs; i++) FXUs[i].clear();	// Clear usage of FXUs
	FPUs.resize(params::micro::nFPUs); for (u32 i=0; i < params::micro::nFPUs; i++) FPUs[i].clear();	// Clear usage of FPUs
	LDUs.resize(params::micro::nLDUs); for (u32 i=0; i < params::micro::nLDUs; i++) LDUs[i].clear();	// Clear usage of LDUs
	STUs.resize(params::micro::nSTUs); for (u32 i=0; i < params::micro::nSTUs; i++) STUs[i].clear();	// Clear usage of STUs
	L1D.reset();												// Reset the L1 data cache (all entries invalid)
	labeling = false;											// Not in labeling mode
	Pready.resize(params::micro::pregs); for (u32 i=0; i<params::micro::pregs; i++) Pready[i] = 0;		// Start with all physical registers ready at T=0
	Pused.resize(params::micro::pregs);  for (u32 i=0; i<params::micro::pregs; i++) Pused[i] = 0;		// Start with all physical registers used at T=0
	mapper.clear(); for (u32 i=0; i<params::micro::nregs; i++) mapper[i] = i;				// Star tiwth identity mapping
	pnext = 0;												// Start with physical register 0
	pfree.clear(); for (u32 i=params::micro::nregs; i < params::micro::pregs; i++) pfree.insert(i);		// Initial set of free physical registers
	assert(params::micro::pregs > params::micro::nregs);							// Make sure there are more physical regisers than architected registers
	niap.clear();												// Clear next instruction address predictor
    }

    u32 Processor::pfind
    (
    )
    {
	u32 idx = 0;
	u64 lrutime = UINT64_MAX;
	for (u32 i : pfree)	// look for the earliest available physical register
	    if (Pused[i] < lrutime) { idx = i; lrutime = Pused[i]; }
	assert(lrutime < UINT64_MAX);
	return idx;
    }

    void *memalloc
    (
        u64	N
    )
    {
	void *addr = &(MEM[FreeMEM]);
	FreeMEM += N;
	assert(FreeMEM <= params::MEM::N);
	return addr;
    }

    void memfree
    (
        void* addr,
	u64	N
    )
    {
	if (((word*)addr + N) == &(MEM[FreeMEM]))
	{
	    FreeMEM -= N;
	}
	return;
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

    void addlabel
    (
        string label,
	u32    line
    )
    {
	if (PROC[me()].label2line.count(label)) return;
	PROC[me()].label2line[label] = line;
    }

    template<typename T> void label
    (
        T (*f)()
    )
    {
	PROC[me()].line2addr.clear();
	PROC[me()].line2encoding.clear();
	PROC[me()].line2len.clear();
	PROC[me()].label2line.clear();
	PROC[me()].labeling = true;
	PROC[me()].runningaddr = 0;
	f();
	PROC[me()].labeling = false;
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
	if (0 == PROC[me()].runningaddr) PROC[me()].runningaddr = instr->line() * 8;
	if (PROC[me()].line2addr.count(instr->line())) return;

	if (PROC[me()].runningaddr % instr->len())
	{
		cout << "Instruction at line # " << instr->line() 
		     << " has length " << instr->len() << " bytes, but has a byte offset of "
		     << (PROC[me()].runningaddr % 8) << endl;
		assert(false);
	}
	PROC[me()].line2addr[instr->line()] = PROC[me()].runningaddr;
	PROC[me()].line2encoding[instr->line()] = instr->encoding();
	PROC[me()].line2len[instr->line()] = instr->len();
	PROC[me()].runningaddr += instr->len();
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
	if (PROC[me()].line2addr.count(instr->line()))				// line already in map?
	{
	    assert(instr->encoding() == PROC[me()].line2encoding[instr->line()]);	// encoding match?
	    assert(instr->len()      == PROC[me()].line2len[instr->line()]);		// instruction length match?
	}
	else if (PROC[me()].instr_forcealign)						// this line has a label, so we must force a word alignment
	{
	    PROC[me()].line2addr[instr->line()] = instr->line() * 8;			// this is a byte address
	    PROC[me()].line2encoding[instr->line()] = instr->encoding();
	    PROC[me()].line2len[instr->line()]      = instr->len();
	    PROC[me()].instr_forcealign = 0;
	}
	else if (PROC[me()].line2addr.count(instr->line() - 1)) 			// is the previous line in the map?
	{
	    PROC[me()].line2addr[instr->line()]     = PROC[me()].line2addr[instr->line() - 1] + PROC[me()].line2len[instr->line() - 1];
	    PROC[me()].line2encoding[instr->line()] = instr->encoding();
	    PROC[me()].line2len[instr->line()]      = instr->len();
	}
	else									// new line
	{
	    PROC[me()].line2addr[instr->line()] = instr->line() * 8;			// this is a byte address
	    PROC[me()].line2encoding[instr->line()] = instr->encoding();
	    PROC[me()].line2len[instr->line()]      = instr->len();
	}

	if (target)								// is this the target of a branch
	{
	    if (PROC[me()].line2addr[instr->line()] % 8)				// is the target address word aligned?
	    {
		cout << "Instruction at line # " << instr->line() << " is the target of a branch but has a byte offset of " << (PROC[me()].line2addr[instr->line()] % 8) << endl;
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
	cout << " | " << setfill('0') << setw( 8) << hex << PROC[me()].line2addr[instr->line()] << dec << setfill(' ');
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
	if (PROC[me()].labeling)
	{
	    instr->line() = line;
	    labeladdr(instr);
	    delete instr;
	    return false;
	}
	instr->line() = line;								// save instruction line number in source file
	assignaddr(instr, PROC[me()].instr_target);					// assign an address to this instruction
	instr->fixit();									// fix displacement in branches
	PROC[me()].instr_target = instr->execute();					// execute the instructions, remember if a branch is being taken
	PROC[me()].trace.push_back(instr);						// save instruction to trace
	if (tracing)									// run-time tracing
	{
	    if (0 == PROC[me()].instr_count) dumpheaderop();
	    dump(PROC[me()].instr_count, instr);
	}
	instr->ops();									// process the internal operations of this instruction
	PROC[me()].instr_count++;							// increment instruction counter
	return PROC[me()].instr_target;							// return true if a branch is taken
    }

    namespace operations
    {
	vector<u64>		MEMready(params::MEM::N);				// ready cycle for memory locations

	void process
	(
	    operation* op
	)
	{
	    if (PROC[me()].op_nextdispatch > PROC[me()].op_lastdispatch)		// are we starting a new cycle for dispatch
	    {
		PROC[me()].dispatched = 0;						// reset counter of dispatched operations
	    }
	    if (PROC[me()].dispatched >= params::micro::maxdispatch)			// did we reach the dispatch per cycle limit?
	    {
		PROC[me()].op_nextdispatch += 1;					// Move to nxt dispatch cycle
		PROC[me()].dispatched = 0;						// Reset counter
	    }
	    PROC[me()].dispatched += 1;							// update number of operations dispatched in this cycle
	    PROC[me()].op_lastdispatch = PROC[me()].op_nextdispatch;			// remember cycle of last dispatch
	    op->process(PROC[me()].op_nextdispatch);					// process this operation
	    PROC[me()].op_count++;							// update operation count
	    PROC[me()].op_maxcycle = max(PROC[me()].op_maxcycle, op->complete());	// update maximum observed completion time
	}

	template<>
	void process<rdw>
	(
	    u08	j, 	// target register
	    u08	k,	// address register
	    u32	addr	// compute address
	)
	{
	    u08 tgtreg = PROC[me()].pfind();								// find next target physical register
	    PROC[me()].pfree.erase(tgtreg);								// target physical register comes out of the free set
	    PROC[me()].pfree.insert(PROC[me()].mapper[j]);						// old physical register goes back to the free set
	    PROC[me()].mapper[j] = tgtreg;								// new mapping of target logical register to target physical register
	    process(new rdw(PROC[me()].mapper[j], PROC[me()].mapper[k], addr));				// process new operation
	}

	template<>
	void process<stw>
	(
	    u08	j, 	// source register
	    u08	k,	// address register
	    u32	addr	// compute address
	)
	{
	    process(new stw(PROC[me()].mapper[j], PROC[me()].mapper[k], addr));				// process new operation
	}
    } // namespace operations

    bool prediction
    (
        u32	addr,	// branch address
	bool	taken,	// branch taken
	string	label	// brach label
    )
    {
	bool	hit = false;					// Branch prediction hit flag

	addr = addr / 8;					// byte address -> word address

	if (taken) 						// branch taken
	{
	    if (PROC[me()].niap.count(addr))			// next instruction address predictor has a match
	    {
		if (PROC[me()].niap[addr] == PROC[me()].line2addr[PROC[me()].label2line[label]])	// do we have a match for the target?
		{
		    hit = true;					// count as a hit
		}
		else						// no match of target address
		{
		    hit = false;				// count as a miss
		    PROC[me()].niap[addr] = PROC[me()].line2addr[PROC[me()].label2line[label]];	// update the entry in the next instruction address predictor
		}
	    }
	    else						// no match of branch address
	    {
		hit = false;					// count as a miss
		PROC[me()].niap[addr] = PROC[me()].line2addr[PROC[me()].label2line[label]];	// create an entry in the next instruction address predictor
	    }
	}
	else							// branch not taken
	{
	    if (PROC[me()].niap.count(addr))			// next instruction address predictor has a match
	    {
		hit = false;					// this counts as a miss
		PROC[me()].niap.erase(addr);			// remove entry from next instruction address predictor
	    }
	    else						// no match in next instruction address predictor
	    {
		hit = true;					// this is a hit!
	    }
	}

	return hit;
    }
} // namespace 8600
