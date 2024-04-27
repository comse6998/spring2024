class bb : public Fijk
{
	private:
	string	_label;
	bool	_taken;

    public:
	bb(u08 j, u08 k, string L) : Fijk(0xB, 0, j, k) { _label = L; }
	bb() : Fijk(0xB, 0, 0, 0) {}

	bool execute()
	{
	    stringstream ss;

	    if (PROC[me()].X(_j).i() - PROC[me()].X(_k).i() < 0)
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
	    return "bb";
	}

	bool ops()
	{
	    operations::process<operations::cmp>(params::micro::CMPFLAGS, _j, _k, 0);
	    operations::process<operations::bb>(_i, params::micro::CMPFLAGS, PROC[me()].line2addr[line_t(_file, _line)], _taken, label_t(_file, _label));
	    return false;
	}

	bool match(u08 F)
	{
		return _F == F;
	}

	void decode(u32 code)
        {
            assert(code < 65536);        // 16-bit instruction
            assert(match(code >> 12));   // we are in the right instruction
            _k = code  & 0xf;            // extract the k field
            _j = (code >> 4) & 0xf;      // extract the j field
            _i = (code >> 8) & 0xf;      // extract the j field
        }
	
	void fixit()
	{
	    assert(PROC[me()].label2line.count(label_t(_file, _label)));
	    line_t targetline = PROC[me()].label2line[label_t(_file, _label)];
	    assert(PROC[me()].line2addr.count(targetline));
	    u32 targetaddr = PROC[me()].line2addr[targetline];
	    assert(PROC[me()].line2addr.count(line_t(_file, _line)));
	    u32 sourceaddr = PROC[me()].line2addr[line_t(_file, _line)];
	    assert(sourceaddr >= targetaddr);
	    _i = (sourceaddr/8) - (targetaddr/8); // Finally calculate and assert _i
	    assert(_i < 16);
	}

	vector<operations::operation*> crack()
	{
	    vector<operations::operation*>	ops;
	    ops.push_back(new operations::cmp(params::micro::CMPFLAGS, _j, _k, 0));
	    ops.push_back(new operations::bb(_i, params::micro::CMPFLAGS));
	    return ops;
	}
};
