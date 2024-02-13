class bb : public Fijk
{
    public:
	bb(u08 j, u08 k, u32 K) : Fijk(0x23, j, k, K) {}

	bool execute()
	{
	    if (PROC.X(_j).i() - PROC.X(_k).i() < 0) return true;
            else return false;
	}

	string mnemonic() const
	{
	    return "bb";
	}
};
