class jmpk : public Fjk
{
    private:
        bool    _taken;
        string  _label;

    public:
        jmpk(u08 j, u08 k) : Fjk(0x3C, j, k) {}
        jmpk() : Fjk(0x3C, 0, 0) {}

        bool execute()
        {
            _taken = true;
	    stringstream ss;

	    line_t targetline = PROC[me()].label2line[label_t(_file, _label)];
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
            operations::process<operations::jmpk>(_k, _j, PROC[me()].line2addr[line_t(_file, _line)], _taken, label_t(_file, _label));
            return false;
        }

        bool match(u08 F)
        {
            return _F == F;
        }

        void decode(u32 code)
        {
            assert(code < 65536);       // 16-bit instruction
            assert(match(code >> 8));   // we are in the right instruction
            _j = (code >> 4) & 0xf;     // extract j
            _k = code & 0xf;            // extract k
        }

        vector<operations::operation*> crack()
        {
                vector<operations::operation*>  ops;
                ops.push_back(new operations::jmpk(_k, _j));
                return ops;
        }
};
