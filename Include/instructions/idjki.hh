class idjki : public Fijk
{
    public:
	idjki(u08 i, u08 j, u08 k) : Fijk(0x7, i, j, k) {}
	idjki() : Fijk(0x7, 0, 0, 0) {}

	bool execute()
	{
	    PROC[me()].X(_i).i() = PROC[me()].X(_j).i() - PROC[me()].X(_k).i();
	    return false;
	}

	string mnemonic() const
	{
	    return "idjki";
	}
};
