class xkj : public Fjk
{
    public:
	xkj(u08 j, u08 k) : Fjk(0x10, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).u() = _k;
	    return false;
	}

	bool ops()
	{
	    operations::process<operations::xKi>(_j, 0, 0, _k);
	    return false;
	}

	string mnemonic() const
	{
	    return "xkj";
	}
};
