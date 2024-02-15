// need a return value or the compiler comlains.
// This is probably not the best pratice.

class jmpk0 : public Fjk
{
    public:
	jmpk0(u08 j, u08 k) : Fjk(0x3C, j, k) {}

	bool execute()
	{
	    return true;
	}

	string mnemonic() const
	{
	    return "jmpk0";
	}
};
