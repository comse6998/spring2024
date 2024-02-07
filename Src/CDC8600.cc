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
	for (uint32_t i = 0; i < params::MEM::N; i++) MEM[i].u() = 0;	// Zero the memory
	FreeMEM = 4*8192;						// Heap starts in page 4
	PROC._XA = 4;							// User context for PROC[0] is in frame 4
	PROC.FL() = (u64)(29 * 8192 / 256);				// User data memory is 29 pages
	PROC.RA() = (u64)( 3 * 8192 / 256);				// User data memory begins in page 3
	instructions::count = 0;					// Instruction count starts at 0
	instructions::target = true;					// First instruction is target of a branch
	for (u32 i=0; i<trace.size(); i++) delete trace[i];		// Delete all previous instructions
	trace.clear();							// Clear the trace
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

    // call5 Call5(void(*f)())
    // {
    // return call5(f);
    // }

    template class reg<12>;
    template class reg<8>;
    template class reg<4>;
    template class reg<1>;
    template class reg<20>;

    vector<instruction*> trace;				// instruction trace

    namespace instructions
    {
	u32	count;
	bool 	target;
    } // namespace instructions

    bool process
    (
        instruction* 	instr,
	u32 		line
    )
    {
	instr->line() = line;				// save instruction line number in source file
	instructions::target = instr->execute();	// execute the instructions, remember if a branch is being taken
	trace.push_back(instr);				// save instruction to trace	
	instructions::count++;				// increment instruction counter
	return instructions::target;			// return true if a branch is taken
    }

    void dump
    (
        vector<instruction*>& T
    )
    {
	cout << "  instr #";
	cout << " |    line #";
	cout << " |                   instruction ";
	cout << " | encoding ";
	cout << endl;

	cout << "----------";
	cout << "+-----------";
	cout << "+--------------------------------";
	cout << "+---------";
	cout << endl;

	for (u32 i=0; i<T.size(); i++)
	{
	    cout << setw( 9) << i;
	    cout << " | " << setw( 9) << T[i]->line();
	    cout << " | " << setw(30) << T[i]->dasm();
	    if (T[i]->len() == 4) cout << " | "     << setfill('0') << setw(8) << hex << T[i]->encoding() << dec << setfill(' ');
	    if (T[i]->len() == 2) cout << " |     " << setfill('0') << setw(4) << hex << T[i]->encoding() << dec << setfill(' '); 
	    cout << endl;
	}
    }
} // namespace 8600
