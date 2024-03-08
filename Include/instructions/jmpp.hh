class jmpp : public FjK
{
    private:
	string	_label;
	bool	_taken;

    public:
	jmpp(u08 j, string L) : FjK(0x34, j, 0) { _label = L; }

	bool execute()
	{
	    if (0 < PROC[me()].X(_j).i()) _taken = true;
            else _taken = false;
	    return _taken;
	}

	string mnemonic() const
	{
	    return "jmpp";
	}

	bool ops()
	{
	    operations::process<operations::cmpz>(params::micro::CMPFLAGS, _j, 0, 0);
	    operations::process<operations::jmpp>(_K, params::micro::CMPFLAGS, PROC[me()].line2addr[_line], _taken, _label);
	    return false;
	}

	void fixit()
	{
	    assert(PROC[me()].label2line.count(_label));
	    u32 targetline = PROC[me()].label2line[_label];
	    assert(PROC[me()].line2addr.count(targetline));
	    u32 targetaddr = PROC[me()].line2addr[targetline];
	    assert(PROC[me()].line2addr.count(_line));
	    u32 sourceaddr = PROC[me()].line2addr[_line];
	    _K = ((targetaddr/8) - (sourceaddr/8)) & 0xfffff;
	}
};
