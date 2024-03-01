class sdjki : public Fijk
{
    private:
	u32 _addr;

    public:
	sdjki(u08 i, u08 j, u08 k) : Fijk(0xF, i, j, k) {}

	bool execute()
	{
	    u64 addr = (PROC[me()].X(_j).i() + PROC[me()].X(_k).i()) & 0xfffff;	// Compute displacement
	    if (addr < PROC[me()].FL().u()*256 )		    // Is displacement within bounds of field length
	    {
		// Good
		addr += PROC[me()].RA().u()*256;		    // Add reference address
		assert(addr >= 0);			    // Address should be nonnegative
		assert(addr < params::MEM::N);              // Check against hardware limit
		MEM[addr] = PROC[me()].X(_i);                     // Store data
		_addr = addr;				    // Save store address
	    }
	    else
	    {
		// Bad
		PROC[me()]._XA = PROC[me()].XA().u();
		PROC[me()].cond()(2) = true;
		assert(false);
	    }
	    return false;
	}

	string mnemonic() const
	{
	    return "sdjki";
	}

	bool ops()
	{
	    process(new operations::agen(params::micro::Xs, _j, _k));
	    process(new operations::stw(_i, params::micro::Xs, _addr));
	    return false;
	}
};
