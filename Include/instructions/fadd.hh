class fadd : public Fijk
{
    public:
        fadd(u08 i, u08 j, u08 k) : Fijk(0x8, i, j, k)
    {
    }

        bool execute()
        {
            stringstream ss;
            ss << setfill('0') << setw(16) << hex << PROC[me()].X(_j).i() <<
                " " << setw(16) << PROC[me()].X(_k).i();
            PROC[me()].X(_i).f() = PROC[me()].X(_j).f() + PROC[me()].X(_k).f();
            ss << " " << setw(16) << PROC[me()].X(_i).i() << dec << setfill(' ');
            _trace = ss.str();
            return false;
        }

        string mnemonic() const
        {
            return "fadd";
        }

        bool ops()
        {
            operations::process<operations::fadd>(_i, _j, _k, 0);
            return false;
        }

        bool match(u08 F)
        {
            return _F == F;
        }

        void decode(u32 code)
        {
            assert(code < 65536);       // 16-bit instruction
            assert(match(code >> 12));  // we are in the right instruction
            _i = (code >> 8) & 0xf;     // extract i
            _j = (code >> 4) & 0xf;     // extract j
            _k = code  & 0xf;           // extract k
        }
};
