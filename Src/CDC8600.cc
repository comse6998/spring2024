#include<CDC8600.hh>
#include<ISA.hh>

namespace CDC8600
{
    vector<word> 	MEM(params::MEM::N);
    uint32_t		FreeMEM;
    Processor		PROC;

    void reset
    (
    )
    {
	for (uint32_t i = 0; i < params::MEM::N; i++) MEM[i] = 0;
	FreeMEM = 256*32;
	PROC.XA = 0;
    }

    void *memalloc
    (
        u64	N
    )
    {
	void *addr = &(MEM[FreeMEM]);
	FreeMEM += N;
    }

    call0 Call(void (*f)())
    {
	return call0(f);
    }
} // namespace 8600
