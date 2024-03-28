class jmpn : public FjK
{
    private:
	string	_label;
	bool	_taken;

    public:
	jmpn(u08 j, string L) : FjK(0x37, j, 0) { _label = L; }

	bool execute()
	{
	    stringstream ss;
	    
	    if (0 > PROC[me()].X(_j).i())
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
	    return "jmpn";
	}
	
	bool ops()
	{
	    operations::process<operations::cmpz>(params::micro::CMPFLAGS, _j, 0, 0);
	    operations::process<operations::jmpn>(_K, params::micro::CMPFLAGS, PROC[me()].line2addr[_line], _taken, _label);
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
