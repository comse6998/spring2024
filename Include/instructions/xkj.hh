class xkj : public Fjk
{
    public:
	xkj(u08 j, u08 k) : Fjk(0x10, j, k) {}
	xkj() : Fjk(0x10, 0, 0) {}

	bool execute()
	{
	    PROC[me()].X(_j).u() = _k;
	    return false;
	}

	bool ops()
	{
	    operations::process<operations::xKi>(_j, 0, 0, _k);
	    return false;
	}

	string mnemonic() const
	{
	    return "xkj";
	}

        bool match(u08 F)
        {
            if (0x10 == F) return true;
            return false;
        }

        void decode(u32 code)
        {
            assert(code < 65536);       // 16-bit instruction
            assert(match(code >> 8));   // we are in the right instruction
            _k = code  & 0xf;           // extract the k field
            _j = (code >> 4) & 0xf;     // extract the j field
        }

	vector<operations::operation*> crack()
	{
	    vector<operations::operation*>	ops;
	    ops.push_back(new operations::xKi(_j, 0, 0, _k));
	    return ops;
	}
};
