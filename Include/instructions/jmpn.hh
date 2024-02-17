class jmpn : public FjK
{
    public:
	jmpn(u08 j, u32 K) : FjK(0x37, j, K) {}

	bool execute()
	{
	    if (0 > PROC.X(_j).i()) return true;
            else return false;
	}

	string mnemonic() const
	{
	    return "jmpn";
	}
};
