class fsub : public Fijk
{
  public:
    fsub(u08 i, u08 j, u08 k) : Fijk(0x9, i, j, k)
    {
    }

    bool execute()
    {
        PROC[me()].X(_i).f() = PROC[me()].X(_j).f() - PROC[me()].X(_k).f();
        return false;
    }

    string mnemonic() const
    {
        return "fsub";
    }

    bool ops()
    {
        process(new operations::fsub(_i, _j, _k));
        return false;
    }
};
