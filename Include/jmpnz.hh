class jmpnz : public FjK
{
    public:
	jmpnz(u08 j, u32 K) : FjK(0x35, j, K) {}

	bool execute()
	{
	    if (0 != PROC.X(_j).u()) return true;
            else return false;
	}

	string mnemonic() const
	{
	    return "jmpnz";
	}
};
