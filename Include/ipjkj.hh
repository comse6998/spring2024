class ipjkj : public Fjk
{
    public:
	ipjkj(u08 j, u08 k) : Fjk(0x0D, j, k) {}

	bool execute()
	{
	    PROC.X(_j).i() = PROC.X(_j).i() * PROC.X(_k).i();
	    return false;
	}

	string mnemonic() const
	{
	    return "ipjkj";
	}
};
