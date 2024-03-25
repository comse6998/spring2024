class idzkj : public Fjk
{
    public:
        idzkj(u08 j, u08 k) : Fjk(0x17, j, k) {}
	idzkj() : Fjk(0x17, 0, 0) {}

        bool execute()
        {
            stringstream ss;
            ss << setfill('0') << setw(16) << hex << PROC[me()].X(_k).i();
            PROC[me()].X(_j).i() = -PROC[me()].X(_k).i();
	    ss << " " << setw(16) << PROC[me()].X(_j).i() << dec << setfill(' ');
            _trace = ss.str();
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

        bool match(u08 F)
        {
            if (0x17 == F) return true;
            return false;
        }

        void decode(u32 code)
        {
            assert(code < 65536);       // 16-bit instruction
            assert(match(code >> 8));   // we are in the right instruction
            _k = code  & 0xf;           // extract the k field
            _j = (code >> 4) & 0xf;     // extract the j field
        }

	vector<operations::operation*> crack()
	{
	    vector<operations::operation*>	ops;
	    ops.push_back(new operations::idzkj(_j, _j, _k, 0));
	    return ops;
	}
};
