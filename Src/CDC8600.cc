#include<iostream>
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

    reg<12> Processor::FL
    (
    )
    {
	assert(false);
    }

    reg<12> Processor::RA
    (
    )
    {
	assert(false);
    }

    reg<8> Processor::XA
    (
    )
    {
	assert(false);
    }

    reg<8> Processor::cond
    (
    )
    {
	assert(false);
    }

    template<int n> 
    u64	reg<n>::u()
    {
	assert(false);
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

    void reset
    (
    )
    {
	for (uint32_t i = 0; i < params::MEM::N; i++) MEM[i].u() = 0;
	FreeMEM = 256*32;
	PROC._XA = 0;
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
} // namespace 8600
