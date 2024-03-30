#include<iostream>
#include<iomanip>
#include<fstream>
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

    void dump
    (
        vector<instruction*>& 	T,
	const char*		filename
    )
    {
	ofstream	file;
	file.open(filename);

	for (u32 i=0; i<T.size();i++)
	{
	    file << setfill('0') << setw( 8) << hex << PROC[me()].line2addr[T[i]->line()] << dec << setfill(' ');
	    if (T[i]->len() == 4) file << " " << setfill('0') << setw(8) << hex << T[i]->encoding() << dec << setfill(' ');
	    if (T[i]->len() == 2) file << " " << setfill('0') << setw(4) << hex << T[i]->encoding() << dec << setfill(' '); 
	    if (T[i]->trace() != "") file << " " << T[i]->trace();
	    file << endl;
	}

	file.close();
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

	vector<basemapper*> mappers(256);

	void initmappers()
	{
	    for (u32 i=0; i<256; i++) mappers[i] = new basemapper;
	    mappers[0x10] = new mapper<xKi>;
	    mappers[0x12] = new mapper<isjkj>;
	    mappers[0x17] = new mapper<idzkj>;
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

    namespace instructions
    {
	vector<basemaker*> makeinstr(256);

	void initmakers()
	{
	    for (u32 i=0; i<256; i++) makeinstr[i] = new maker<pass>;
	    makeinstr[0x10] = new maker<xkj>;
	    makeinstr[0x12] = new maker<isjkj>;
	    makeinstr[0x17] = new maker<idzkj>;
	}

	vector<u64> decode
	/*
	 * Takes as input a 32-bit halfword containing either one 32-bit instruction or two 16-bit instruction.
	 * Produces a vector of operations that implement those instructions.
	 * Each operation is encoded as a 64-bit value
	 * 		F :	 8-bit function identifier
	 * 		i :	12-bit physical register number (output)
	 * 		j : 	12-bit physical register number (input)
	 * 		k : 	12-bit physical register number (input)
	 * 		K :	20-bit immedidate field
	 */
	(
	    u32 code
	)
	{
	    vector<u64> ops;

	    u08 F = code >> 24;				// extract first byte
	    instruction* instr = makeinstr[F]->make();	// make corresponding instruction
	    vector<operations::operation*> cracked;	// operations from an instruction
	    switch (instr->len())
	    {
		case 2:			// two 16-bit instructions
		    // decode first instruction
		    instr->decode(code >> 16);
		    cracked = instr->crack();
		    for (u32 i=0; i<cracked.size(); i++) ops.push_back(cracked[i]->encode());
		    // decode second instruction
		    code = code & 0xffff;
		    F = code >> 8;
		    instr = makeinstr[F]->make();
		    assert(2 == instr->len());
		    instr->decode(code);
		    cracked = instr->crack();
		    for (u32 i=0; i<cracked.size(); i++) ops.push_back(cracked[i]->encode());
		    break;
		case 4:			// one 32-bit instruction
		    // decode single instruction
		    instr->decode(code);
		    cracked = instr->crack();
		    for (u32 i=0; i<cracked.size(); i++) ops.push_back(cracked[i]->encode());
		    break;
		default:		// should not happen
		    assert(false);
	    }

	    return ops;
	}
    } // namespace instructions

    namespace pipeline
    {
	IFstage	IF;
	ICstage	IC[2];
	RMstage	RM;
	ODstage OD[2];
	IQstage	IQ[2];
	OIstage	OI[2];
	BRstage	BR[2];
	FXstage FX[2];
	FPstage FP[2];
	LDstage	LD[2];
	STstage	ST[2];
	CQstage CQ[2];
	COstage	CO;

	void reset()
	{
	    IF.reset();
	    IC[0].reset(); IC[1].reset();
	    RM.reset();
	    OD[0].reset(); OD[1].reset();
	    IQ[0].reset(); IQ[1].reset();
	    OI[0].reset(); OI[1].reset();
	    BR[0].reset(); BR[1].reset();
	    FX[0].reset(); FX[1].reset();
	    FP[0].reset(); FP[1].reset();
	    LD[0].reset(); LD[1].reset();
	    ST[0].reset(); ST[1].reset();
	    CQ[0].reset(); CQ[1].reset();
	    CO.reset();
	}

	void IFstage::init
	(
	    const char* filename
	)
	{
	    fetchgroups.clear();
	    fstream ifs;
	    ifs.open(filename, ios::in);

	    u32 addr;			// instruction address
	    u32 instr;			// instruction encoding
	    u64 instrword = 0;		// instruction word (fetch group)
	    u32 lastaddr = 0;		// address of last instruction (detect a new group)
	    u32 hihalf = 0;		// high half of instruction word
	    u32 lohalf = 0;		// low half of instruction word
	    bool firstword = true;	// first fetch word
	    while (ifs >> hex >> addr)
	    {
		// addr is the instruction address
		// instr is the instruction encoding
		ifs >> hex >> instr;
		if ((addr/8) != (lastaddr/8))
		{
		    // new instruction word
		    instrword = hihalf;
		    instrword = (instrword << 32) | lohalf;
		    if (!firstword) { fetchgroups.push_back(instrword); }
		    instrword = 0; hihalf = 0; lohalf = 0; firstword = false;
		}
		lastaddr = addr;
		switch (addr % 8)
		{
		    case 0: hihalf = instr; 			break;
		    case 2: hihalf = (hihalf << 16) | instr; 	break;
		    case 4: lohalf = instr; 			break;
		    case 6: lohalf = (lohalf << 16) | instr;	break;
		    default: assert(false);
		}
		string line; getline(ifs, line);
	    }
	    // include the last instruction word
	    instrword = hihalf;
	    instrword = (instrword << 32) | lohalf;
	    if (!firstword) { fetchgroups.push_back(instrword); }

	    ifs.close();

	    // dump the fetch history
	    for (u32 i=0; i<fetchgroups.size(); i++)
	    {
	        cout << setfill('0') << setw(16) << hex << fetchgroups[i] << dec << setfill(' ') << endl;
	    }

	    // initialize fetch count
	    fetchcount = 0;
	}

	void copy(u32 N, u64 u, bitvector& v, u32 first)
	{
	    assert(first + N <= v.size());
	    for (u32 i=0; i<N; i++)
	    {
		v[first + i] = u & 0x1;
		u = u >> 1;
	    }
	}

	void copy(u32 N, const bitvector& u, u32 ufirst, bitvector& v, u32 vfirst)
	{
	    assert(ufirst + N <= u.size());
	    assert(vfirst + N <= v.size());
	    for (u32 i=0; i<N; i++)
	    {
		v[vfirst + i] = u[ufirst + i];
	    }
	}

	void copy(u32 N, const bitvector& u, u32 first, u32& v)
	{
	    assert(first + N <= u.size());
	    v = 0;
	    for (u32 i=0; i<N; i++)
	    {
		v = v + ((u[first + i] & 0x1) << i);
	    }
	}

	void IFstage::tick()
	{
	    if (txdone)
	    {
		copy(32, fetchgroups[fetchcount] >> 32, out, 0);
		copy(16, fetchcount, out, 32);
		copy(32, fetchgroups[fetchcount] & 0xffffffff, out, 48);
		copy(16, fetchcount, out, 80);
		txdone = false;
		txready = true;
		fetchcount++;
	    }
	}

	void ICstage::tick()
	{
	    if (rxdone)
	    {
		u32 code; copy(32, in, 0, code);
		vector<u64> ops = instructions::decode(code);
		for (u32 i=0; i<ops.size(); i++)
		{
		    bitvector	op = out; for (u32 i=0; i < op.size(); i++) op[i] = false;
		    copy(16, in, 32, op, 64);
		    copy(64, ops[i], op, 0);
		    opsq.push_back(op);
		}
	        rxdone = false; rxready = true;
	    }

	    if (opsq.size())
	    {
		copy(80, opsq[0], 0, out, 0);
		opsq.erase(opsq.begin());
		txready = true; txdone = false;
	    }
	    else
	    {
		for (u32 i=0; i < out.size(); i++) out[i] = false;
		txready = true; txdone = false;
	    }
	}

	void RMstage::tick() 	
	{
	   if (txdone && rxdone)
	   {
	      for (u32 i=0; i<192; i++) out[i] = false;
	      for (u32 i=0; i<80; i++) out[i+ 0] = in[i+ 0];
	      for (u32 i=0; i<80; i++) out[i+96] = in[i+80];
	   }

	   for (u32 i=0; i<192; i++) out[i] = false;
	   if (txdone && rxdone)
	   {
	       copy(16, in, 64, out, 80);	// pass through the fetch group from IC[0]
	       copy(16, in,144, out,176);	// pass through the fetch group from IC[1]
	       copy(20, in,  0, out,  0);	// pass through K field from IC[0]
	       copy(20, in, 80, out, 96);	// pass through K field from IC[1]
	       copy( 8, in, 56, out, 56);	// pass through F field from IC[0]
	       copy( 8, in,136, out,152);	// pass through F field from IC[1]

	       u32 fg[2];
	       u32 F[2];
	       u32 ireg[2];
	       u32 jreg[2];
	       u32 kreg[2];

	       copy(16, in, 64, fg[0]);
	       copy(16, in,144, fg[1]);
	       copy( 8, in, 56, F[0]);
	       copy( 8, in,136, F[1]);
	       copy(12, in, 44, ireg[0]);
	       copy(12, in,124, ireg[1]);
	       copy(12, in, 32, jreg[0]);
	       copy(12, in,112, jreg[1]);
	       copy(12, in, 20, kreg[0]);
	       copy(12, in,100, kreg[1]);

	       operations::mappers[F[0]]->map(ireg[0], jreg[0], kreg[0]);	// architected -> physical register mapping
	       copy(12, ireg[0], out, 44);	// pass physical i register from IC[0]
	       copy(12, jreg[0], out, 32);	// pass physical j register from IC[0]
	       copy(12, kreg[0], out, 20);	// pass physical k register from IC[0]

	       copy(16, opcount, out, 64);	// pass operation count from IC[0]
	       opcount++;

	       copy(12, ireg[1], out,140);	// pass physical i register from IC[1] 
	       copy(12, jreg[1], out,128);	// pass physical j register from IC[1] 
	       copy(12, kreg[1], out,116);	// pass physical k register from IC[1] 

	       copy(16, opcount, out,160);	// pass operation count from IC[1]
	       opcount++;
	   }
	   rxdone = false; rxready = true;
	   txready = true; txdone = false;
	}

	void COstage::tick()
	{
	    if (rxdone)
	    {
		rxready = true;
		rxdone = false;
		txready = false;
		txdone = true;
	    }
	}

	bool IFstage::busy()
	{
	    return (fetchcount < fetchgroups.size());
	}

	bool busy()
	{
	    if (IF.busy())    return true;
	    if (IC[0].busy()) return true; 
	    if (IC[1].busy()) return true;
	    if (RM.busy())    return true;
	    if (OD[0].busy()) return true; 
	    if (OD[1].busy()) return true;
	    if (IQ[0].busy()) return true; 
	    if (IQ[1].busy()) return true;
	    if (OI[0].busy()) return true; 
	    if (OI[1].busy()) return true;
	    if (BR[0].busy()) return true; 
	    if (BR[1].busy()) return true;
	    if (FX[0].busy()) return true; 
	    if (FX[1].busy()) return true;
	    if (FP[0].busy()) return true; 
	    if (FP[1].busy()) return true;
	    if (LD[0].busy()) return true; 
	    if (LD[1].busy()) return true;
	    if (ST[0].busy()) return true; 
	    if (ST[1].busy()) return true;
	    if (CQ[0].busy()) return true; 
	    if (CQ[1].busy()) return true;
	    if (CO.busy())    return true;
	    return false;
	}

	void tick()
	{
	    IF.tick();
	    IC[0].tick(); IC[1].tick();
	    RM.tick();
	    OD[0].tick(); OD[1].tick();
	    IQ[0].tick(); IQ[1].tick();
	    OI[0].tick(); OI[1].tick();
	    BR[0].tick(); BR[1].tick();
	    FX[0].tick(); FX[1].tick();
	    FP[0].tick(); FP[1].tick();
	    LD[0].tick(); LD[1].tick();
	    ST[0].tick(); ST[1].tick();
	    CQ[0].tick(); CQ[1].tick();
	    CO.tick();
	}

	stage<96,96>& OIstage::target()
	{
	    return FX[_ix];
	}

	void transfer()
	{
	    transfer(96, CQ[0],  0, CO   ,  0);
	    transfer(96, CQ[1],  0, CO   , 96);
	    transfer(96, FX[0],  0, CQ[0],  0);
	    transfer(96, FX[1],  0, CQ[1],  0);
	    transfer(96, OI[0],  0, OI[0].target(), 0);
	    transfer(96, OI[1],  0, OI[1].target(), 0);
	    transfer(96, IQ[0],  0, OI[0],  0);
	    transfer(96, IQ[1],  0, OI[1],  0);
	    transfer(96, OD[0],  0, IQ[0],  0);
	    transfer(96, OD[1],  0, IQ[1],  0);
	    transfer(96, RM   ,  0, OD[0],  0);
	    transfer(96, RM   , 96, OD[1],  0);
	    transfer(80, IC[0],  0, RM   ,  0);
	    transfer(80, IC[1],  0, RM   , 80);
	    transfer(48, IF   ,  0, IC[0],  0);
	    transfer(48, IF   , 48, IC[1],  0);
	}

	void dump
	(
	    const bitvector& v
	)
	{
	    u32 acc = 0;
	    for (i32 i=v.size() - 1; i >= 0; i--)
	    {
		acc = (acc << 1) | ((u08)v[i] & 0x1);
		if (0 == (i % 8))
		{
		    cout << setfill('0') << setw(2) << hex << acc << dec << setfill(' ');
		    acc = 0;
		}
	    }
	}

	void run
	(
	    const char* filename
	)
	{
	    IF.init(filename);
	    RM.init();
	    OI[0].init(0); OI[1].init(1);

	    cout << "   cycle | "
		 << "                      IF | "
		 << "               IC[0] | "
		 << "                                              RM | "
		 << "                   FX[0] | "
		 << "                   CQ[0]"
		 << endl;

	    cout << "---------+-"
		 << "-------------------------+-"
		 << "---------------------+-"
		 << "-------------------------------------------------+-"
		 << "-------------------------+-"
		 << "------------------------"
		 << endl;

	    for (u32 cycle = 0; busy(); cycle++)
	    {
		tick();
		transfer();
		cout << setw(8) << cycle << " | ";
		dump(IF.out)   ; cout << " | ";
		dump(IC[0].out); cout << " | ";
		dump(RM.out)   ; cout << " | ";
		dump(FX[0].out); cout << " | ";
		dump(CQ[0].out);
		cout << endl;
	    }
	}
    } // namespace pipeline
} // namespace 8600
