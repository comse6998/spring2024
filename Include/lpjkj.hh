class lpjkj : public Fjk
{
    public:
	lpjkj(u08 j, u08 k) : Fjk(0x1, j, k) {}

	bool execute()
	{
	    PROC.X(_j).i() = PROC.X(_j).i() & PROC.X(_k).i();
	}

	string mnemonic() const
	{
	    return "lpjkj";
	}
};
