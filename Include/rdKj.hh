class rdjK : public FjK
{
    public:
	rdjK(u08 j, u32 K) : FjK(0x24, j, K) {}

	bool execute()
	{
	    if (_K < PROC.FL().u()*256)			// Check that displacement is within bounds of field length
	    {
		// Good
		uint32_t addr = PROC.RA().u()*256 + _K;	// Architected address
		assert(addr < params::MEM::N);		// Check against hardware limit
		PROC.X(_j) = MEM[addr];			// Read data
	    }
	    else
	    {
		// Bad
		PROC.cond()(2) = true;
		PROC._XA = PROC.XA().u();
		assert(false);
	    }
	    return false;
	}

	string mnemonic() const
	{
	    return "rdjK";
	}
};
