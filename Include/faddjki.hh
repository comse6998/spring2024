class faddjki : public Fijk {

    public:
	faddjki(u08 i, u08 j, u08 k) : Fijk(0x8, i, j, k) {}

    bool execute()
    {
        PROC.X(_i).f() = PROC.X(_j).f() + PROC.X(_k).f();
    }

    string mnemonic() const
    {
        return "faddjki";
    }
};