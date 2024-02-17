class idjki : public Fijk
{
    public:
	idjki(u08 i, u08 j, u08 k) : Fijk(0x7, i, j, k) {}

	bool execute()
	{
	    PROC.X(_i).i() = PROC.X(_j).i() - PROC.X(_k).i();
	}

	string mnemonic() const
	{
	    return "idjki";
	}
};
