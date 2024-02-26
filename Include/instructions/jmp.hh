class jmp : public FjK
{
    private:
	string	_label;

    public:
	jmp(string L) : FjK(0x30, 0, 0) { _label = L; }

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

	bool ops()
	{
	    process(new operations::jmp());
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
