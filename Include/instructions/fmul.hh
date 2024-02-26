class fmul : public Fijk
{
  public:
    fmul(u08 i, u08 j, u08 k) : Fijk(0xA, i, j, k)
    {
    }

    bool execute()
    {
        PROC[me()].X(_i).f() = PROC[me()].X(_j).f() * PROC[me()].X(_k).f();
        return false;
    }

    string mnemonic() const
    {
        return "fmul";
    }

    bool ops()
    {
        process(new operations::fmul(_i, _j, _k));
        return false;
    }
};
