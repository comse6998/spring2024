class rdjk : public Fjk
{
    public:
	rdjk(u08 j, u08 k) : Fjk(0x25, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).u() = PROC[me()].X(_k).u();
            return false;
	}

	string mnemonic() const
	{
	    return "rdjk";
	}
};
