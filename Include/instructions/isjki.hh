class isjki : public Fijk
{
    public:
	isjki(u08 i, u08 j, u08 k) : Fijk(0x6, i, j, k) {}

	bool execute()
	{
	    PROC.X(_i).i() = PROC.X(_j).i() + PROC.X(_k).i();
	    return false;
	}

	string mnemonic() const
	{
	    return "isjki";
	}

	bool ops()
	{
	    process(new operations::isjki(_i, _j, _k));
	    return false;
	}
};
