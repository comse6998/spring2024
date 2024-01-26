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
	    if (0 == PROC.X(Xj).u()) return true;
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
		cout << "X(Xj)" << PROC.X(Xj).u() << endl;
		cout << "X(Xk)" << PROC.X(Xk).u() << endl;
	    assert(Xi < 16);
		assert(Xj < 16);
		assert(Xk < 16);
		assert(PROC.X(Xj).u()+PROC.X(Xk).u() >= 256 * 32); // In legal memory region
		assert(PROC.X(Xj).u()+PROC.X(Xk).u() < FreeMEM);   // In legal memory region
		PROC.X(Xi).u() = MEM[PROC.X(Xj).u()+PROC.X(Xk).u()].u();
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
   } // namespace instructions
} // namespace CDC8600
