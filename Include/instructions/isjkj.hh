class isjkj : public Fjk
{
    public:
	isjkj(u08 j, u08 k) : Fjk(0x12, j, k) {}

	bool execute()
	{
	    PROC.X(_j).i() = PROC.X(_j).i() + _k;
	    return false;
	}

	string mnemonic() const
	{
	    return "isjkj";
	}

	bool ops()
	{
	    process(new operations::isjkj(_j, _k));
	    return false;
	}
};
