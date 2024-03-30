class ipjkj : public Fjk
{
    public:
	ipjkj(u08 j, u08 k) : Fjk(0x0D, j, k) {}
	ipjkj() : Fjk(0x0D, 0, 0) {}

	bool execute()
	{
	    stringstream ss;
	    ss << setfill('0') << setw(16) << hex << PROC[me()].X(_j).i() <<
                " " << setw(16) << PROC[me()].X(_k).i();
	    PROC[me()].X(_j).i() = PROC[me()].X(_j).i() * PROC[me()].X(_k).i();
	    ss << " " << setw(16) << PROC[me()].X(_j).i() << dec << setfill(' ');
	    _trace = ss.str();
	    return false;
	}

	string mnemonic() const
	{
	    return "ipjkj";
	}

	bool ops()
	{
	    operations::process<operations::ipjkj>(_j, _j, _k, 0);
	    return false;
	}

        bool match(u08 F)
        {
	    return _F == F;
        }

        void decode(u32 code)
        {
	    assert(code < 65536);       // 16-bit instruction
	    assert(match(code >> 8));  // we are in the right instruction
	    _j = (code >> 4) & 0xf;     // extract j
	    _k = code  & 0xf;           // extract k
        }
};
