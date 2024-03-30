class isjkj : public Fjk
{
    public:
	isjkj(u08 j, u08 k) : Fjk(0x12, j, k) {}
	isjkj() : Fjk(0x12, 0, 0) {}

	bool execute()
	{
	    stringstream ss;
	    ss << setfill('0') << setw(16) << hex << PROC[me()].X(_j).i() <<
                " " << setw(1) << u32(_k);
	    PROC[me()].X(_j).i() = PROC[me()].X(_j).i() + _k;
	    ss << " " << setw(16) << PROC[me()].X(_j).i() << dec << setfill(' ');
	    _trace = ss.str();
	    return false;
	}

	string mnemonic() const
	{
	    return "isjkj";
	}

	bool ops()
	{
	    operations::process<operations::isjkj>(_j,_j, _k, 0);
	    return false;
	}

        bool match(u08 F)
        {
	    return _F == F;
        }

        void decode(u32 code)
        {
	    assert(code < 65536);       // 16-bit instruction
	    assert(match(code >> 8));   // we are in the right instruction
	    _j = (code >> 4) & 0xf;     // extract j
	    _k = code  & 0xf;           // extract k
        }

	vector<operations::operation*> crack()
	{
	    vector<operations::operation*>	ops;
	    ops.push_back(new operations::isjkj(_j, _j, _k, 0));
	    return ops;
	}
};
