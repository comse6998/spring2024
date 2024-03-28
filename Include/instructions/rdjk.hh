class rdjk : public Fjk
{
    public:
	rdjk(u08 j, u08 k) : Fjk(0x25, j, k) {}

	bool execute()
	{
		stringstream ss;
		ss << setfill('0') << setw(16) << hex << PROC[me()].X(_k).i(); // combine the source data to the ss
	    PROC[me()].X(_j).i() = PROC[me()].X(_k).i();
		ss << " " << setw(16) << PROC[me()].X(_j).i() << dec << setfill(' ');
		_trace = ss.str();
        return false;
	}

	string mnemonic() const
	{
	    return "rdjk";
	}

	bool ops()
        {
            operations::process<operations::rdw>(_j, _k, 0);
            return false;
        }
	
	bool match(u08 F)
        {
            if (0x25 == F) return true;
            return false;
        }

	void decode(u32 code)
        {
            assert(code < 65536);       // 16-bit instruction
            assert(match(code >> 8));   // we are in the right instruction
            _k = code  & 0xf;           // extract the k field
            _j = (code >> 4) & 0xf;     // extract the j field
        }
};
