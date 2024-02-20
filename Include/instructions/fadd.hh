class fadd : public Fijk
{
    public:
	fadd(u08 i, u08 j, u08 k) : Fijk(0x8, i, j, k) {}

	bool execute()
	{
	    PROC.X(_i).f() = PROC.X(_j).f() + PROC.X(_k).f();
	    return false;
	}

	string mnemonic() const
	{
	    return "fadd";
	}

	bool ops()
	{
	    process(new operations::fadd(_i, _j, _k));
	    return false;
	}
};
