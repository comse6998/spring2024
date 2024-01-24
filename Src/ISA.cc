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
	    assert(false);
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
	    assert(false);
	}

        void sdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    assert(false);
	}

        void isjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    assert(false);
	}
	
        void idjkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
	    assert(false);
	}

	void idzkj
	(
	   uint8_t Xj,
	   uint8_t Xk
	)
	{
	    assert(false);
	}

	void isjkj
	(
	    uint8_t Xj,
	    uint8_t k
	)
	{
	    assert(false);
	}

	void ipjkj
	(
	    uint8_t Xj,
	    uint8_t Xk
	)
	{
	    assert(false);
	}
   } // namespace instructions
} // namespace CDC8600
