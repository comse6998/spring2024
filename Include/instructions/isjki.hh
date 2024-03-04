class isjki : public Fijk
{
    public:
	isjki(u08 i, u08 j, u08 k) : Fijk(0x6, i, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_i).i() = PROC[me()].X(_j).i() + PROC[me()].X(_k).i();
	    return false;
	}

	string mnemonic() const
	{
	    return "isjki";
	}

	bool ops()
	{
	    operations::process<operations::isjki>(_i, _j, _k, 0);
	    return false;
	}
};
