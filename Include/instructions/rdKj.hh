class rdjK : public FjK
{
    public:
	rdjK(u08 j, u32 K) : FjK(0x24, j, K) {}

	bool execute()
	{
	    if (_K < PROC[me()].FL().u()*256)			// Check that displacement is within bounds of field length
	    {
		// Good
		uint32_t addr = PROC[me()].RA().u()*256 + _K;	// Architected address
		assert(addr < params::MEM::N);		// Check against hardware limit
		PROC[me()].X(_j) = MEM[addr];			// Read data
	    }
	    else
	    {
		// Bad
		PROC[me()].cond()(2) = true;
		PROC[me()]._XA = PROC[me()].XA().u();
		assert(false);
	    }
	    return false;
	}

	string mnemonic() const
	{
	    return "rdjK";
	}
};
