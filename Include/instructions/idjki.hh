class idjki : public Fijk
{
    public:
	idjki(u08 i, u08 j, u08 k) : Fijk(0x7, i, j, k) {}
	idjki() : Fijk(0x7, 0, 0, 0) {}

	bool execute()
	{
	    stringstream ss;
            ss << setfill('0') << setw(16) << hex << PROC[me()].X(_j).i();
            ss << " " << setw(16) << PROC[me()].X(_k).i();
            PROC[me()].X(_i).i() = PROC[me()].X(_j).i() - PROC[me()].X(_k).i();
            ss << " " << setw(16) << PROC[me()].X(_i).i() << dec << setfill(' ');
            _trace = ss.str();
	    return false;
	}

	string mnemonic() const
	{
	    return "idjki";
	}

        bool ops()
        {
            operations::process<operations::idjki>(_i, _j, _k, 0);
            return false;
        }

        bool match(u08 F)
        {
            if (0x7 == F) return true;
            return false;
        }

        void decode(u32 code)
        {
            assert(code < 65536);        // 16-bit instruction
            assert(match(code >> 12));   // we are in the right instruction
            _k = code  & 0xf;            // extract the k field
            _j = (code >> 4) & 0xf;      // extract the j field
            _i = (code >> 8) & 0xf;      // extract the j field
        }

	vector<operations::operation*> crack()
	{
	    vector<operations::operation*>	ops;
	    ops.push_back(new operations::idjki(_i, _j, _k, 0));
	    return ops;
	}
};
