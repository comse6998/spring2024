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
	    assert(k < 16);
	    PROC.X(Xj).u() = k;
	}

        void rdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{   
		assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);

		if (PROC.X(Xj).i()+PROC.X(Xk).i() < PROC.FL().u()*256) {
			// good
			uint32_t addr = PROC.RA().u()*256 + PROC.X(Xj).i()+PROC.X(Xk).i();	// Architected address
			assert(addr < params::MEM::N);		// Check against hardware limit
			PROC.X(Xi) = MEM[addr];
		} else {
			// bad	
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
		assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);

		if (PROC.X(Xj).i()+PROC.X(Xk).i() < PROC.FL().u()*256) {
			// good
			uint32_t addr = PROC.RA().u()*256 + PROC.X(Xj).i()+PROC.X(Xk).i();	// Architected address
			assert(addr < params::MEM::N);		// Check against hardware limit
			MEM[addr] = PROC.X(Xi);
		} else {
			// bad	
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
		// cout << "isjki, Xi: " << unsigned(Xi) << "Xj: " << unsigned(Xj) << "Xk: " << unsigned(Xk) << endl;
	    assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);
		// cout << "X(Xi)" << PROC.X(Xi).u() << endl;
		// cout << "X(Xj)" << PROC.X(Xj).u() << endl;
		// cout << "X(Xk)" << PROC.X(Xk).i() << endl;
		PROC.X(Xi).i() = PROC.X(Xj).i() + PROC.X(Xk).i();
		// cout << "X(Xi)" << PROC.X(Xi).u() << endl;
		// cout << "X(Xj)" << PROC.X(Xj).u() << endl;
		// cout << "X(Xk)" << PROC.X(Xk).i() << endl;
		
	}
	
        void idjkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
		assert(Xj < 16);
		assert(k < 16);
	    PROC.X(Xj).u() = PROC.X(Xj).u() - k;
	}

	void idzkj
	(
	   uint8_t Xj,
	   uint8_t Xk
	)
	{
		assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xj).i() = 0-PROC.X(Xk).u();
	}

	void isjkj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
		assert(Xj < 16);
		assert(k < 16);
	    PROC.X(Xj).i() = PROC.X(Xj).i() + k;
	}

	void ipjkj
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
