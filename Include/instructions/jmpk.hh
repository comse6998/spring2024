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
	    return _taken;
	}
	
	string mnemonic() const
	{
	    return "jmpk";
	}

	bool ops()
	{
	    _label = "";
	    operations::process<operations::jmpk>(_k, _j, PROC[me()].line2addr[_line], _taken, _label);
	    return false;
	}
};
