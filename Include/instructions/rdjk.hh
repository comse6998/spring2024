class rdjk : public Fjk
{
    private:
        u32 _addr;

    public:
        rdjk(u08 j, u08 k) : Fjk(0x25, j, k) {}
        rdjk() : Fjk(0x25, 0, 0) {}

        bool execute()
        {
            u64 addr = (PROC[me()].X(_k).u()) & 0xfffff; // save the address
            stringstream ss;
                addr += PROC[me()].RA().u()*256;		    // Add reference address
                PROC[me()].X(_j) = MEM[addr];               // Read data
                _addr = addr;				    // Save read address
                ss << setfill('0') << setw(16) << hex << addr; // combine the source data to the ss
                ss << " " << setw(16) << PROC[me()].X(_j).i() << " "<< dec << setfill(' ');
            
            _trace = ss.str();
            return false;
        }

        string mnemonic() const
        {
            return "rdjk";
        }

        bool ops()
        {
            operations::process<operations::agen>(params::micro::Xs, _k, 0, 0);
            operations::process<operations::rdw>(_j, params::micro::Xs, _addr);
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
