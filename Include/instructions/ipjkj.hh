class ipjkj : public Fjk
{
    public:
	ipjkj(u08 j, u08 k) : Fjk(0x0D, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).i() = PROC[me()].X(_j).i() * PROC[me()].X(_k).i();
	    return false;
	}

	string mnemonic() const
	{
	    return "ipjkj";
	}

	bool ops()
	{
	    operations::process<operations::ipjkj>(_j, _j, _k, 0);
	    return false;
	}
};
