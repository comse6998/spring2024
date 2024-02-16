class xkj : public Fjk
{
    public:
	xkj(u08 j, u08 k) : Fjk(0x10, j, k) {}

	bool execute()
	{
	    PROC.X(_j).u() = _k;
	    return false;
	}

	string mnemonic() const
	{
	    return "xkj";
	}
};
