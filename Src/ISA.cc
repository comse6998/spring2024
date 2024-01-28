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
		// cout << "rdjki, Xi: " << unsigned(Xi) << "Xj: " << unsigned(Xj) << "Xk: " << unsigned(Xk) << endl;
		// cout << "Xi: " << unsigned(Xi) << endl;
		// cout << "Xj: " << unsigned(Xj) << endl;
		// cout << "Xk: " << unsigned(Xk) << endl;
		// cout << "X(Xi)" << PROC.X(Xi).i() << endl;
		// cout << "X(Xj)" << PROC.X(Xj).i() << endl;
		// cout << "X(Xk)" << PROC.X(Xk).i() << endl;
		// cout << "PROC.RA().u()" << PROC.RA().u() << endl;
		// cout << "PROC.FL().u()" << PROC.FL().u() << endl;
	    
		assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);

		// Check upper bound and lower memory bound
		// assert(PROC.X(Xj).u()+PROC.X(Xk).u() >=  PROC.RA().u()*256); // In legal memory region
		// assert(PROC.X(Xj).u()+PROC.X(Xk).u() < PROC.FL().u()*256);   // In legal memory region
		
		PROC.X(Xi).u() = MEM[PROC.X(Xj).i()+PROC.X(Xk).i()].u();
		
		// cout << "X(Xi)" << PROC.X(Xi).i() << endl;
		// cout << "X(Xj)" << PROC.X(Xj).i() << endl;
		// cout << "X(Xk)" << PROC.X(Xk).i() << endl;
	}

        void sdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
		// cout << "sdjki, Xi: " << unsigned(Xi) << "Xj: " << unsigned(Xj) << "Xk: " << unsigned(Xk) << endl;
		// cout << "X(Xi)" << PROC.X(Xi).i() << endl;
		// cout << "X(Xj)" << PROC.X(Xj).i() << endl;
		// cout << "X(Xk)" << PROC.X(Xk).i() << endl;
		assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);

		// Check for upper and lower memory bound: TODO

	    MEM[PROC.X(Xj).i()+PROC.X(Xk).i()].u() = PROC.X(Xi).u();
		// cout << "X(Xi)" << PROC.X(Xi).i() << endl;
		// cout << "X(Xj)" << PROC.X(Xj).i() << endl;
		// cout << "X(Xk)" << PROC.X(Xk).i() << endl;
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
