class lpjkj : public Fjk
{
    public:
	lpjkj(u08 j, u08 k) : Fjk(0x1, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).u() = PROC[me()].X(_j).u() & PROC[me()].X(_k).u();
	    return false;
	}

	string mnemonic() const
	{
	    return "lpjkj";
	}
};
