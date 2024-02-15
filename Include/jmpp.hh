class jmpp : public FjK
{
    public:
	jmpp(u08 j, u32 K) : FjK(0x36, j, K) {}

	bool execute()
	{
	    if (0 < PROC.X(_j).i()) return true;
            else return false;
	}

	string mnemonic() const
	{
	    return "jmpp";
	}
};
