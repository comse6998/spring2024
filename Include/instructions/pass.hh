class pass : public Fjk
{
    public:
	pass() : Fjk(0xF, 0, 0) {}

	bool execute()
	{
	    return false;
	}

	bool ops()
	{
	    return false;
	}

	string mnemonic() const
	{
	    return "pass";
	}

	string dasm() const
	{
	    return mnemonic() + "()";
	}

};
