class jmpnz : public FjK
{
    private:
	string	_label;
	bool	_taken;
    
    public:
	jmpnz(u08 j, string L) : FjK(0x35, j, 0) { _label = L; }
	jmpnz() : FjK(0x35, 0, 0) {}

	bool execute()
	{
	    stringstream ss;

	    if (0 != PROC[me()].X(_j).u())
	    {

		_taken = true;

		line_t targetline = PROC[me()].label2line[label_t(_file, _label)];
        	u32 targetaddr = PROC[me()].line2addr[targetline];

		ss << setfill('0') << setw(8) << hex << targetaddr << " " << dec << setfill(' ');
	    }
	    else _taken = false;

	    _trace = ss.str();

	    return _taken;
	}

	string mnemonic() const
	{
	    return "jmpnz";
	}

        bool ops()
	{
	    operations::process<operations::cmpz>(params::micro::CMPFLAGS, _j, 0, 0);
	    operations::process<operations::jmpnz>(_K, params::micro::CMPFLAGS, _addr, _taken, label_t(_file, _label));
	    return false;
	}

	void fixit()
	{
	    assert(PROC[me()].label2line.count(label_t(_file, _label)));
	    line_t targetline = PROC[me()].label2line[label_t(_file, _label)];
	    assert(PROC[me()].line2addr.count(targetline));
	    u32 targetaddr = PROC[me()].line2addr[targetline];
	    assert(PROC[me()].line2addr.count(line_t(_file, _line)));
	    u32 sourceaddr = PROC[me()].line2addr[line_t(_file, _line)];
	    _K = ((targetaddr/8) - (sourceaddr/8)) & 0xfffff;
	}

	bool match(u08 F)
	{
	return _F == F;
	}

	void decode(u32 code)
	{
	assert(match(code >> 24));    // we are in the right instruction
	_j = (code >> 20) & 0xf;      // extract j
	_K = code & 0xfffff;          // extract K
	}

	vector<operations::operation*> crack()
	{
	    vector<operations::operation*>	ops;
	    ops.push_back(new operations::cmpz(params::micro::CMPFLAGS, _j, 0, 0));
	    ops.push_back(new operations::jmpnz(_K, params::micro::CMPFLAGS));
	    return ops;
	}
};
