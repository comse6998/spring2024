class jmpk : public Fjk
{
    private:
	bool	_taken;
	string	_label;

    public:
	jmpk(u08 j, u08 k) : Fjk(0x3C, j, k) {}

	bool execute()
	{
	    _taken = true;
		stringstream ss;
		u32 targetline = PROC[me()].label2line[_label];
		u32 targetaddr = PROC[me()].line2addr[targetline];

		ss << setfill('0') << setw(8) << hex << targetaddr << " "
		   << dec << setfill(' ');
		_trace = ss.str();
	    return _taken;
	}
	
	string mnemonic() const
	{
	    return "jmpk";
	}

	string dasm() const
	{
	    return mnemonic() + "(" + to_string(_k) + ")";
	}
	bool ops()
	{
	    _label = "";
	    operations::process<operations::jmpk>(_k, _j, PROC[me()].line2addr[_line], _taken, _label);
	    return false;
	}

	void decode(u32 code)
	{
		assert(match(code >> 24));    // we are in the right instruction
		_j = (code >> 20) & 0xf;      // extract j
		_k = code & 0xfffff;          // extract K
	}
};
