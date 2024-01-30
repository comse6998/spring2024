#include<CDC8600.hh>
#include<assert.h>

namespace CDC8600
{
   namespace instructions
   {
        bool jmpz // Jump to P + K if (Xj) equal to 0.
	(
	    uint8_t Xj
	)
	{
	    assert(Xj < 16);
	    if (0 == PROC.X(Xj).u()) return true;
	    else return false;
	}

	bool jmpp // Jump to P + K if (Xj) is Positive
	(
	    uint8_t Xj
	)
	{
		// note P is the current program address and K is a sign extended increment
	    assert(Xj < 16);
	    if (0 < PROC.X(Xj).i()) return true;
	    else return false;
	}

        void xkj // Transmit k to Xj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
	    assert(Xj < 16);
	    assert(k < 16);
	    PROC.X(Xj).u() = k;
	}

        void rdjki // Read data at address (Xj)+(Xk) from MEM to Xi
	(
	    uint8_t Xi,
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
	    assert(Xi < 16);
	    assert(Xj < 16);
		assert(Xk < 16);

		int64_t offset = PROC.X(Xj).u() + PROC.X(Xk).u();
		if (offset < PROC.FL().u()*256)
	    {
		// Good
		uint64_t addr = PROC.RA().u()*256 + offset;
		assert(addr < params::MEM::N);
		PROC.X(Xi) = MEM[addr];
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }
	}

        void sdjki // Store the data in (Xi) to MEM[Xk+Xj]
	(
	    uint8_t Xi,
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
	    assert(Xi < 16);
	    assert(Xj < 16);
		assert(Xk < 16);

		int64_t offset = PROC.X(Xj).u() + PROC.X(Xk).u();
		assert(PROC.X(Xj).i() + PROC.X(Xk).i() == offset);

		if (offset < PROC.FL().u()*256)
	    {
		uint64_t addr = PROC.RA().u()*256 + offset;
		assert(addr < params::MEM::N);
		MEM[addr] = PROC.X(Xi);
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }

	}

        void isjki // Integer sum of (Xj) plus (Xk) to Xi
	(
	    uint8_t Xi,
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
	    assert(Xi < 16);
	    assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xi).i() = PROC.X(Xj).i() + PROC.X(Xk).i();
	}

        void idjkj // Integer difference of (Xj) minus k to Xj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
	    assert(Xj < 16);
	    assert(k < 16);
		PROC.X(Xj).i() = PROC.X(Xj).i() - k;
	}

	void idzkj // Integer difference of zero minus (Xk) to Xj
	(
	   uint8_t Xj,
	   uint8_t Xk
	)
	{
	    assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xj).i() = -PROC.X(Xk).i();
	}

	void isjkj // Integer sum of (Xj) plus k to Xj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
	    assert(Xj < 16);
	    assert(k < 16);
		PROC.X(Xj).i() = PROC.X(Xj).i() + k;
	}

	void ipjkj // Integer product of (Xj) times (Xk) to Xj
	(
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
	    assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xj).i() = PROC.X(Xj).i() * PROC.X(Xk).i();
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
		uint32_t addr = PROC.RA().u()*256 + K;
		assert(addr < params::MEM::N);
		PROC.X(Xj) = MEM[addr];
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }
	}

   }
}