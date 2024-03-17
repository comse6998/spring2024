class idzkj : public Fjk
{
    public:
	idzkj(u08 j, u08 k) : Fjk(0x17, j, k) {}

	bool execute()
	{
	    PROC[me()].X(_j).i() = -PROC[me()].X(_k).i();
	    stringstream ss; ss << setfill('0') << setw(16) << hex << PROC[me()].X(_k).i() << " " << PROC[me()].X(_j).i() << dec << setfill(' '); _trace = ss.str();
	    return false;
	}

	string mnemonic() const
	{
	    return "idzkj";
	}

	bool ops()
	{
	    operations::process<operations::idzkj>(_j, _j, _k, 0);
	    return false;
	}
};
