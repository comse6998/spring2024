class jmpz : public FjK
{
    private:
	string	_label;
	bool	_taken;

    public:
	jmpz(u08 j, string L) : FjK(0x34, j, 0) { _label = L; }
	jmpz() : FjK(0x34, 0, 0) {}

	bool execute()
	{
            stringstream ss;
	    if (0 == PROC[me()].X(_j).u()) {
                _taken = true;
                u32 targetline = PROC[me()].label2line[_label];
                u32 targetaddr = PROC[me()].line2addr[targetline];

                ss << setfill('0') << setw(8) << hex << targetaddr << " "
                    << dec << setfill(' ');
            }
            else _taken = false;

            _trace = ss.str();
	    return _taken;
	}

	string mnemonic() const
	{
	    return "jmpz";
	}

	bool ops()
	{
	    operations::process<operations::cmpz>(params::micro::CMPFLAGS, _j, 0, 0);
	    operations::process<operations::jmpz>(_K, params::micro::CMPFLAGS, PROC[me()].line2addr[_line], _taken, _label);
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
};
