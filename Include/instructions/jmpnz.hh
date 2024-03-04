class jmpnz : public FjK
{
    private:
	string	_label;
    
    public:
	jmpnz(u08 j, string L) : FjK(0x35, j, 0) { _label = L; }

	bool execute()
	{
	    if (0 != PROC[me()].X(_j).u()) return true;
            else return false;
	}

	string mnemonic() const
	{
	    return "jmpnz";
	}

        bool ops()
	{
	    operations::process<operations::cmpz>(params::micro::CMPFLAGS, _j, 0, 0);
	    operations::process<operations::jmpnz>(_K, params::micro::CMPFLAGS);
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
