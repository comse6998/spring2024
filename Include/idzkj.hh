class idzkj : public Fjk
{
    public:
	idzkj(u08 j, u08 k) : Fjk(0x17, j, k) {}

	bool execute()
	{
	     PROC.X(_j).i() = -PROC.X(_k).i();
	     return false;
	}

	string mnemonic() const
	{
	    return "idzkj";
	}
};
