class idjkj : public Fjk
{
    public:
	idjkj(u08 j, u08 k) : Fjk(0x13, j, k) {}

	bool execute()
	{
	    stringstream ss;
	    ss << setfill('0') << setw(16) << hex << PROC[me()].X(_j).i() <<
                " " << setw(2) << _k;
	    PROC[me()].X(_j).i() = PROC[me()].X(_j).i() - _k;
            ss << " " << setw(16) << PROC[me()].X(_j).i() << dec <<
                setfill(' ');
	    _trace = ss.str();
	    return false;
	}

	string mnemonic() const
	{
	    return "idjkj";
	}

	bool ops()
	{
	    operations::process<operations::idjkj>(_j, _j, 0, _k);
	    return false;
	}
};
