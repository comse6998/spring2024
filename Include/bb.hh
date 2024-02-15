class bb : public Fijk
{
    public:
	bb(u08 j, u08 k) : Fijk(0xB, 0, j, k) {}

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