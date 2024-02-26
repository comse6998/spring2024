class idzkj : public Fjk
{
    public:
	idzkj(u08 j, u08 k) : Fjk(0x17, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).i() = -PROC[me()].X(_k).i();
	    return false;
	}

	string mnemonic() const
	{
	    return "idzkj";
	}

	bool ops()
	{
	    process(new operations::idzkj(_j, _k));
	    return false;
	}
};
