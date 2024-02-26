#ifndef _TYPES_HH_
#define _TYPES_HH_

using namespace std;

namespace CDC8600
{
    typedef uint64_t		u64;
    typedef uint32_t		u32;
    typedef uint8_t		u08;
    typedef int64_t		i64;
    typedef int32_t		i32;
    typedef double		f64;
    typedef complex<double>	c128;

    namespace units
    {
	class unit
	{
	    private:
		set<u64>	_busy;	// the cycles this unit is busy

	    public:
		unit() { }
		void clear()               { _busy.clear(); }			// clear the unit usage
		bool busy(u64 cycle) const { return _busy.count(cycle); }	// is the unit busy at that cycle?
		void claim(u64 cycle)      { _busy.insert(cycle); }		// claim the unit for that cycle
		bool isfree(u64 begin, u64 end)					// is the unit free for cycles [begin, end)?
		{
		    for (u64 i = begin; i < end; i++) if (busy(i)) return false;
		    return true;
		}
	};

	extern vector<unit>	LDUs;	// load units
	extern vector<unit>	STUs;	// store units
	extern vector<unit>	FXUs;	// fixed-point units
	extern vector<unit>	FPUs;	// floating-point units
	extern vector<unit>	BRUs;	// branch units
    } // namespace units

    namespace L1
    {
	class cacheset
	{
	    private:
		vector<bool>	_valid;
		vector<u32>	_tag;
		vector<u64>	_used;

	    public:
		cacheset();
		void reset();
		bool valid(u32 i) { return _valid[i]; }
		u32& tag(u32 i) { return _tag[i]; }
		u64& used(u32 i) { return _used[i]; }
		void invalidate(u32 i) { _valid[i] = false; }
		void validate(u32 i) { _valid[i] = true; }
	};

	class cache
	{
	    private:
		vector<cacheset>	_sets;

	    public:
		cache();
		void reset();
		bool hit(u32 addr);
		bool loadhit(u32 addr, u64 cycle);
		bool storehit(u32 addr, u64 cycle);
	};
    }
} // namespace CDC8600

#endif // _TYPES_HH
