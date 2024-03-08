class isjkj : public Fjk
{
    public:
	isjkj(u08 j, u08 k) : Fjk(0x12, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).i() = PROC[me()].X(_j).i() + _k;
	    return false;
	}

	string mnemonic() const
	{
	    return "isjkj";
	}

	bool ops()
	{
	    operations::process<operations::isjkj>(_j,_j, _k, 0);
	    return false;
	}
};
