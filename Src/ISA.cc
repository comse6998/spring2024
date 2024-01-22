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
	    if (PROC.X(Xj).u() == 0) return true;
	    else return false;
	}
	
        void xkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
	    PROC.X(Xj).u() = k;
	}

        void rdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    PROC.X(Xi) = MEM[PROC.X(Xj).i() + PROC.X(Xk).i()];
	}

        void sdjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    MEM[PROC.X(Xj).i() + PROC.X(Xk).i()] = PROC.X(Xi);
	}

        void isjki
	(
	    uint8_t Xi, 
	    uint8_t Xj, 
	    uint8_t Xk
	)
	{
	    PROC.X(Xi).i() = PROC.X(Xj).i() + PROC.X(Xk).i();
	}
	
        void idjkj
	(
	    uint8_t Xj, 
	    uint8_t k
	)
	{
	    PROC.X(Xj).i() = PROC.X(Xj).i() - k;
	}
   } // namespace instructions
} // namespace CDC8600
