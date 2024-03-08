class idjkj : public Fjk
{
    public:
	idjkj(u08 j, u08 k) : Fjk(0x13, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).i() = PROC[me()].X(_j).i() - _k;
	    return false;
	}

	string mnemonic() const
	{
	    return "idjkj";
	}

	bool ops()
	{
	    operations::process<operations::idjkj>(_j, _j, 0, _k);
	    return false;
	}
};
