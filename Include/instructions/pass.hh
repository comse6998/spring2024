class pass : public Fjk
{
    public:
	pass() : Fjk(0x0F, 0, 0) {}

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

	bool match(u08 F)
	{
	    if (0x0F == F) return true;
	    return false;
	}

	void decode(u32 code)
	{
            assert(code < 65536);       // 16-bit instruction
            // assert(match(code >> 8));   // we are in the right instruction
            _k = code  & 0xf;           // extract the k field
            _j = (code >> 4) & 0xf;     // extract the j field
        }
};
