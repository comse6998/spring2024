class jmpk0 : public Fjk
{
    public:
	jmpk0(u08 j, u08 k) : Fjk(0x3C, j, k) {}
	jmpk0() : Fjk(0x3C, 0, 0) {}

	bool execute()
	{
	    return true;
	}
	
	string mnemonic() const
	{
	    return "jmpk0";
	}
};
