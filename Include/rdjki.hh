class rdjki : public Fijk
{
    public:
	rdjki(u08 i, u08 j, u08 k) : Fijk(0xD, i, j, k) {}

	bool execute()
	{
	    int64_t addr = (PROC.X(_j).i() + PROC.X(_k).i()) & 0xfffff;	// Compute displacement
	    if (addr < PROC.FL().u()*256 )		    // Is displacement within bounds of field length
	    {
		// Good
		addr += PROC.RA().u()*256;		    // Add reference address
		assert(addr >= 0);			    // Address should be nonnegative
		assert(addr < params::MEM::N);              // Check against hardware limit
		PROC.X(_i) = MEM[addr];                     // Read data
	    }
	    else
	    {
		// Bad
		PROC._XA = PROC.XA().u();
		PROC.cond()(2) = true;
		assert(false);
	    }
	    return false;
	}

	string mnemonic() const
	{
	    return "rdjki";
	}
};
