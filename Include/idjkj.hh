class idjkj : public Fjk
{
    public:
	idjkj(u08 j, u08 k) : Fjk(0x13, j, k) {}

	bool execute()
	{
	    PROC.X(_j).i() = PROC.X(_j).i() - _k;
	    return false;
	}

	string mnemonic() const
	{
	    return "idjkj";
	}
};
