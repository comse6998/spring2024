#include<CDC8600.hh>
#include<assert.h>

namespace CDC8600
{
   namespace instructions
   {   
        bool jmpz
	(
	    uint8_t Xj
	)
	{
	    assert(Xj < 16);
	    if (0 == PROC.X(Xj).u()) return true;
	    else return false;
	}

	bool jmpp
	(
	    uint8_t Xj
	)
	{	
		assert(Xj < 16);
		if (PROC.X(Xj).i() > 0) return true;
	    else return false;
	}
	
        void xkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
	    assert(Xj < 16);
	    assert(k < 1024*1024);
	    PROC.X(Xj) = k;
	}

        void rdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    assert(Xj < 16);
	    assert(Xk < 16);
		assert(Xi < 16);
	    if ( (PROC.X(Xj).u() + PROC.X(Xk).u()) < PROC.FL().u()*256)
	    {
		// Good
		uint32_t addr = PROC.RA().u()*256 + PROC.X(Xj).u() + PROC.X(Xk).u();	// Architected address
		assert(addr < params::MEM::N);		// Check against hardware limit
		PROC.X(Xi) = MEM[addr];
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }
	}

        void sdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    assert(Xj < 16);
	    assert(Xk < 16);
		assert(Xi < 16);
	    if ( (PROC.X(Xj).u() + PROC.X(Xk).u()) < PROC.FL().u()*256)
	    {
		// Good
		uint32_t addr = PROC.RA().u()*256 + PROC.X(Xj).u() + PROC.X(Xk).u();	// Architected address
		assert(addr < params::MEM::N);		// Check against hardware limit
		MEM[addr] = PROC.X(Xi);
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }
	}

        void isjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    assert(Xj < 16);
	    assert(Xk < 16);
		assert(Xi < 16);
		PROC.X(Xi) = PROC.X(Xj).u() + PROC.X(Xk).u();
	}
	
        void idjkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
	    assert(Xj < 16);
		assert(k < 8);
		PROC.X(Xj) = PROC.X(Xj).u() - k;
	}

	void idzkj
	(
	   uint8_t Xj,
	   uint8_t Xk
	)
	{
	    assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xj) = 0 - PROC.X(Xk).u();
	}

	void isjkj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
	    assert(Xj < 16);
		assert(k < 8);
		PROC.X(Xj) = PROC.X(Xj).u() + k;
	
	}

	void ipjkj
	(
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
	    assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xj) = PROC.X(Xj).u() * PROC.X(Xk).u();
	}

	void rdKj
	(
	    uint8_t Xj,
	    uint32_t K
	)
	{
	    assert(Xj < 16);
	    assert(K < 1024*1024);

	    if (K < PROC.FL().u()*256)
	    {
		// Good
		uint32_t addr = PROC.RA().u()*256 + K;	// Architected address
		assert(addr < params::MEM::N);		// Check against hardware limit
		PROC.X(Xj) = MEM[addr];
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }
	}

   } // namespace instructions
} // namespace CDC8600
