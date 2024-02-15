#include<CDC8600.hh>
#include<assert.h>
#include<stdio.h>

namespace CDC8600
{
   namespace instructions
   {   
<<<<<<< HEAD
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
		return (PROC.X(Xj).i()>0) ? true : false;
	}
	
        void xkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
	    assert(Xj < 16);
	    assert(k < 16);
	    PROC.X(Xj).u() = k % 32; 
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
	    assert(PROC.X(Xj).u()+PROC.X(Xk).u() < 1024*1024);

	    if (PROC.X(Xj).u()+PROC.X(Xk).u() < PROC.FL().u()*256)
	    {
		// Good
		uint32_t addr = PROC.RA().u()*256 + PROC.X(Xj).u()+PROC.X(Xk).u();	// Architected address
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
		assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);
	    assert(PROC.X(Xj).u()+PROC.X(Xk).u() < 1024*1024);

	    if (PROC.X(Xj).u()+PROC.X(Xk).u() < PROC.FL().u()*256)
	    {
		// Good
		uint32_t addr = PROC.RA().u()*256 + PROC.X(Xj).u()+PROC.X(Xk).u();	// Architected address
		assert(addr < params::MEM::N);		// Check against hardware limit
		MEM[addr] = PROC.X(Xi);
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
	    }
	    //assert(false);
	}

        void isjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
		assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);

		int64_t tmp = PROC.X(Xj).u() + PROC.X(Xk).u();
		
		PROC.X(Xi).u() = tmp;
	    //assert(false);
	}
	
        void idjkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
		assert(Xj < 16);
		int64_t tmp = (int64_t)PROC.X(Xj).u() - ((int8_t)(k<<4))/16;
		//printf("%ld %d %ld\n", PROC.X(Xj).u(),((int8_t)(k<<4))/16,tmp);
		PROC.X(Xj).u() = tmp;
	    //assert(false);
	}

	void idzkj
	(
	   uint8_t Xj,
	   uint8_t Xk
	)
	{
		assert(Xj < 16);
		assert(Xk < 16);
		int64_t tmp = 0 - (int64_t)PROC.X(Xk).u();
		PROC.X(Xj).u() = tmp;
	    //assert(false);
	}

	void isjkj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
		assert(Xj < 16);
		int8_t tmp = ((int8_t)(k<<4))/16;
		int64_t tmp2 = PROC.X(Xj).u()+tmp;
		PROC.X(Xj).u() = tmp2;
	    //assert(false);
	}

	void ipjkj
	(
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
		assert(Xj < 16);
		assert(Xk < 16);
		PROC.X(Xj).u() = (((int64_t)(PROC.X(Xj).u()<<16)/65536) * ((int64_t)(PROC.X(Xk).u()<<16)/65536));
		//PROC.X(Xj).u() = (PROC.X(Xj).u()) * (PROC.X(Xk).u());

	    //assert(false);
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

=======
>>>>>>> upstream/main
   } // namespace instructions
} // namespace CDC8600
