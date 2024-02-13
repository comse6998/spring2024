class rdjk : public Fjk
{
    public:
	rdjk(u08 j, u08 k) : Fjk(0x25, j, k) {}

	bool execute()
	{
	    PROC.X(_j).u() = PROC.X(_k).u();
            return false;
	}

	string mnemonic() const
	{
	    return "rdjk";
	}
};
