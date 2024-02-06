class jmp : public FjK
{
    public:
	jmp(u32 K) : FjK(0x30, 0, K) {}

	bool execute()
	{
	    return true;
	}

	string mnemonic() const
	{
	    return "jmp";
	}

	string dasm() const
	{
	    return mnemonic() + "(" + to_string(_K) + ")";
	}
};
