class fmuljk : public Fjk
{
    public:
	fmuljk(u08 j, u08 k) : Fjk(0xC, j, k) {}

	bool execute()
	{
	    PROC.X(_j).f() = PROC.X(_j).f() * PROC.X(_k).f();
	}

	string mnemonic() const
	{
	    return "fmuljk";
	}
};
