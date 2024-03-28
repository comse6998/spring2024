class bb : public Fijk
{
	private:
	string	_label;
	bool	_taken;

    public:
	bb(u08 j, u08 k, string L) : Fijk(0xB, 0, j, k) { _label = L; }

	bool execute()
	{
	    stringstream ss;

	    if (PROC[me()].X(_j).i() - PROC[me()].X(_k).i() < 0)
	    {
	        _taken = true;

	        u32 targetline = PROC[me()].label2line[_label];
	        u32 targetaddr = PROC[me()].line2addr[targetline];

	        ss << setfill('0') << setw(8) << hex << targetaddr << " " << dec << setfill(' ');
	    }
	    else _taken = false;

	    _trace = ss.str();
	    return _taken;
	}

	string mnemonic() const
	{
	    return "bb";
	}

	bool ops()
	{
	    operations::process<operations::cmp>(params::micro::CMPFLAGS, _j, _k, 0);
	    operations::process<operations::bb>(_i, params::micro::CMPFLAGS, PROC[me()].line2addr[_line], _taken, _label);
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
	    assert(sourceaddr >= targetaddr);
	    _i = (sourceaddr/8) - (targetaddr/8); // Finally calculate and assert _i
	    assert(_i < 16);
	}
};
