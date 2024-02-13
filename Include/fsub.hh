class fsub : public Fijk
{
    public:
	fsub(u08 i, u08 j, u08 k) : Fijk(0x9, i, j, k) {}

	bool execute()
	{
	    PROC.X(_i).f() = PROC.X(_j).f() - PROC.X(_k).f();
	    return false;
	}

	string mnemonic() const
	{
	    return "fsub";
	}
};
