#ifndef _ISA_HH_
#define _ISA_HH_

#include <CDC8600.hh>

// 1. Branch Facility
#define LABEL(L)                                                                                                       \
    L : {                                                                                                              \
        branching::labels[#L] = __LINE__;                                                                              \
    }

#define b(X)                                                                                                           \
    if (instructions::b::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))    \
        goto X;

#define blr()

#define beq(X)                                                                                                         \
    if (instructions::beq::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))  \
        goto X;
#define bne(X)                                                                                                         \
    if (instructions::bne::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))  \
        goto X;
#define blt(X)                                                                                                         \
    if (instructions::blt::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))  \
        goto X;
#define bgt(X)                                                                                                         \
    if (instructions::bgt::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))  \
        goto X;
#define ble(X)                                                                                                         \
    if (instructions::ble::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))  \
        goto X;
#define bge(X)                                                                                                         \
    if (instructions::bge::execute(branching::labels.count(#X) ? branching::labels[#X] - __LINE__ : 0, #X, __LINE__))  \
        goto X;

#define simti(RT) instructions::simti::execute(RT, __LINE__);
#define simtj(RT) instructions::simtj::execute(RT, __LINE__);
#define pushavf()                                                                                                      \
    {                                                                                                                  \
        STACK().push_back(AV());                                                                                       \
    }
#define popavf()                                                                                                       \
    {                                                                                                                  \
        AV() = STACK().back();                                                                                         \
        STACK().pop_back();                                                                                            \
    }

// 2. Addressing Facility
#define malloc_m(RP, RA) instructions::malloc_m::execute(RP, RA, __LINE__);
#define free_m(RP) instructions::free_m::execute(RP, __LINE__);
#define rss(RD, RS, RA) instructions::rss::execute(RD, RS, RA, __LINE__);

// 3. Load/Store Facility
#define lb(RT, RA, RB) instructions::lb::execute(RT, RA, RB, __LINE__);
#define lh(RT, RA, RB) instructions::lh::execute(RT, RA, RB, __LINE__);
#define lw(RT, RA, RB) instructions::lw::execute(RT, RA, RB, __LINE__);
#define ld(RT, RA, RB) instructions::ld::execute(RT, RA, RB, __LINE__);
#define lfs(RT, RA, RB) instructions::lfs::execute(RT, RA, RB, __LINE__);
#define lfd(RT, RA, RB) instructions::lfd::execute(RT, RA, RB, __LINE__);
#define lfc(RT, RA, RB) instructions::lfc::execute(RT, RA, RB, __LINE__);
#define lfz(RT, RA, RB) instructions::lfz::execute(RT, RA, RB, __LINE__);

#define lib(RT, SI) instructions::lib::execute(RT, SI, __LINE__);
#define lih(RT, SI) instructions::lih::execute(RT, SI, __LINE__);
#define liw(RT, SI) instructions::liw::execute(RT, SI, __LINE__);
#define lid(RT, SI) instructions::lid::execute(RT, SI, __LINE__);

#define lifs(RT, SI) instructions::lifs::execute(RT, SI, __LINE__);
#define lifd(RT, SI) instructions::lifd::execute(RT, SI, __LINE__);
#define lifc(RT, SI) instructions::lifc::execute(RT, SI, __LINE__);
#define lifz(RT, SI) instructions::lifz::execute(RT, SI, __LINE__);

#define stb(RT, RA, RB) instructions::stb::execute(RT, RA, RB, __LINE__);
#define sth(RT, RA, RB) instructions::sth::execute(RT, RA, RB, __LINE__);
#define stw(RT, RA, RB) instructions::stw::execute(RT, RA, RB, __LINE__);
#define stdw(RT, RA, RB) instructions::stdw::execute(RT, RA, RB, __LINE__);
#define stfs(RT, RA, RB) instructions::stfs::execute(RT, RA, RB, __LINE__);
#define stfd(RT, RA, RB) instructions::stfd::execute(RT, RA, RB, __LINE__);
#define stfc(RT, RA, RB) instructions::stfc::execute(RT, RA, RB, __LINE__);
#define stfz(RT, RA, RB) instructions::stfz::execute(RT, RA, RB, __LINE__);

// 4. Fixed-point Facility

// 4.0 Initialization instructions
#define zw(RT) ;

// 4.1. Arithmetic instructions
#define addb(RT, RA, RB) instructions::addb::execute(RT, RA, RB, __LINE__);
#define mulb(RT, RA, RB) instructions::mulb::execute(RT, RA, RB, __LINE__);
#define mulhb(RT, RA, RB) instructions::mulhb::execute(RT, RA, RB, __LINE__);
#define mullb(RT, RA, RB) instructions::mullb::execute(RT, RA, RB, __LINE__);
#define divb(RT, RA, RB) instructions::divb::execute(RT, RA, RB, __LINE__);
#define modb(RT, RA, RB) instructions::modb::execute(RT, RA, RB, __LINE__);
#define subb(RT, RA, RB) instructions::subb::execute(RT, RA, RB, __LINE__);

#define addw(RT, RA, RB) instructions::addw::execute(RT, RA, RB, __LINE__);
#define mulw(RT, RA, RB) instructions::mulw::execute(RT, RA, RB, __LINE__);
#define mulhw(RT, RA, RB) instructions::mulhw::execute(RT, RA, RB, __LINE__);
#define mullw(RT, RA, RB) instructions::mullw::execute(RT, RA, RB, __LINE__);
#define divw(RT, RA, RB) instructions::divw::execute(RT, RA, RB, __LINE__);
#define modw(RT, RA, RB) instructions::modw::execute(RT, RA, RB, __LINE__);
#define subw(RT, RA, RB) instructions::subw::execute(RT, RA, RB, __LINE__);

#define addd(RT, RA, RB) instructions::addd::execute(RT, RA, RB, __LINE__);
#define subd(RT, RA, RB) instructions::subd::execute(RT, RA, RB, __LINE__);
#define muld(RT, RA, RB) instructions::muld::execute(RT, RA, RB, __LINE__);
#define mulhd(RT, RA, RB) instructions::mulhd::execute(RT, RA, RB, __LINE__);
#define mulld(RT, RA, RB) instructions::mulld::execute(RT, RA, RB, __LINE__);
#define divd(RT, RA, RB) instructions::divd::execute(RT, RA, RB, __LINE__);
#define modd(RT, RA, RB) instructions::modd::execute(RT, RA, RB, __LINE__);

#define addib(RT, RA, SI) instructions::addib::execute(RT, RA, SI, __LINE__);
#define subib(RT, RA, SI) instructions::subib::execute(RT, RA, SI, __LINE__);
#define mulib(RT, RA, SI) instructions::mulib::execute(RT, RA, SI, __LINE__);
#define divib(RT, RA, SI) instructions::divib::execute(RT, RA, SI, __LINE__);

#define addih(RT, RA, SI) instructions::addih::execute(RT, RA, SI, __LINE__);
#define subih(RT, RA, SI) instructions::subih::execute(RT, RA, SI, __LINE__);
#define mulhh(RT, RA, RB) instructions::mulhh::execute(RT, RA, RB, __LINE__);
#define mullh(RT, RA, RB) instructions::mullh::execute(RT, RA, RB, __LINE__);
#define mulih(RT, RA, SI) instructions::mulih::execute(RT, RA, SI, __LINE__);
#define divih(RT, RA, SI) instructions::divih::execute(RT, RA, SI, __LINE__);

#define addiw(RT, RA, SI) instructions::addiw::execute(RT, RA, SI, __LINE__);
#define subiw(RT, RA, SI) instructions::subiw::execute(RT, RA, SI, __LINE__);
#define mulhw(RT, RA, RB) instructions::mulhw::execute(RT, RA, RB, __LINE__);
#define mullw(RT, RA, RB) instructions::mullw::execute(RT, RA, RB, __LINE__);
#define muliw(RT, RA, SI) instructions::muliw::execute(RT, RA, SI, __LINE__);
#define diviw(RT, RA, SI) instructions::diviw::execute(RT, RA, SI, __LINE__);

#define addid(RT, RA, SI) instructions::addid::execute(RT, RA, SI, __LINE__);
#define subid(RT, RA, SI) instructions::subid::execute(RT, RA, SI, __LINE__);
#define mulid(RT, RA, SI) instructions::mulid::execute(RT, RA, SI, __LINE__);
#define divid(RT, RA, SI) instructions::divid::execute(RT, RA, SI, __LINE__);

#define addh(RT, RA, RB) instructions::addh::execute(RT, RA, RB, __LINE__);
#define subh(RT, RA, RB) instructions::subh::execute(RT, RA, RB, __LINE__);
#define divh(RT, RA, RB) instructions::divh::execute(RT, RA, RB, __LINE__);
#define modh(RT, RA, RB) instructions::modh::execute(RT, RA, RB, __LINE__);
#define mulh(RT, RA, RB) instructions::mulh::execute(RT, RA, RB, __LINE__);

#define msubb(RT, RA, RB) instructions::msubb::execute(RT, RA, RB, __LINE__);
#define msubh(RT, RA, RB) instructions::msubh::execute(RT, RA, RB, __LINE__);
#define msubw(RT, RA, RB) instructions::msubw::execute(RT, RA, RB, __LINE__);
#define msubd(RT, RA, RB) instructions::msubd::execute(RT, RA, RB, __LINE__);

#define maddw(RT, RA, RB) instructions::maddw::execute(RT, RA, RB, __LINE__);
#define maddh(RT, RA, RB) instructions::maddh::execute(RT, RA, RB, __LINE__);
#define maddb(RT, RA, RB) instructions::maddb::execute(RT, RA, RB, __LINE__);
#define maddd(RT, RA, RB) instructions::maddd::execute(RT, RA, RB, __LINE__);

#define modw(RT, RA, RB) instructions::modw::execute(RT, RA, RB, __LINE__);
#define modh(RT, RA, RB) instructions::modh::execute(RT, RA, RB, __LINE__);
#define modb(RT, RA, RB) instructions::modb::execute(RT, RA, RB, __LINE__);
#define modd(RT, RA, RB) instructions::modd::execute(RT, RA, RB, __LINE__);

#define modiw(RT, RA, RB) instructions::modiw::execute(RT, RA, RB, __LINE__);
#define modih(RT, RA, RB) instructions::modih::execute(RT, RA, RB, __LINE__);
#define modib(RT, RA, RB) instructions::modib::execute(RT, RA, RB, __LINE__);
#define modid(RT, RA, RB) instructions::modid::execute(RT, RA, RB, __LINE__);

// 4.2. Compare instructions

#define cmpfs(RA, RB) instructions::cmpfs::execute(RA, RB, __LINE__);
#define cmpb(RA, RB) instructions::cmpb::execute(RA, RB, __LINE__);
#define cmph(RA, RB) instructions::cmph::execute(RA, RB, __LINE__);
#define cmpw(RA, RB) instructions::cmpw::execute(RA, RB, __LINE__);
#define cmpd(RA, RB) instructions::cmpd::execute(RA, RB, __LINE__);
#define cmpfs(RA, RB) instructions::cmpfs::execute(RA, RB, __LINE__);
#define cmpib(RA, SI) instructions::cmpib::execute(RA, SI, __LINE__);
#define cmpih(RA, SI) instructions::cmpih::execute(RA, SI, __LINE__);
#define cmpiw(RA, SI) instructions::cmpiw::execute(RA, SI, __LINE__);
#define cmpid(RA, SI) instructions::cmpid::execute(RA, SI, __LINE__);
#define cmpifs(RA, SI) instructions::cmpifs::execute(RA, SI, __LINE__);
#define cmpifd(RA, SI) instructions::cmpifd::execute(RA, SI, __LINE__);
#define cmpifc(RA, SI) instructions::cmpifc::execute(RA, SI, __LINE__);
#define cmpifz(RA, SI) instructions::cmpifz::execute(RA, SI, __LINE__);

// 5. Floating-point Facility

// 5.1. Arithmetic instructions
#define addfs(RT, RA, RB) instructions::addfs::execute(RT, RA, RB, __LINE__);
#define mulfs(RT, RA, RB) instructions::mulfs::execute(RT, RA, RB, __LINE__);
#define divfs(RT, RA, RB) instructions::divfs::execute(RT, RA, RB, __LINE__);
#define subfs(RT, RA, RB) instructions::subfs::execute(RT, RA, RB, __LINE__);

#define addfd(RT, RA, RB) instructions::addfd::execute(RT, RA, RB, __LINE__);
#define mulfd(RT, RA, RB) instructions::mulfd::execute(RT, RA, RB, __LINE__);
#define divfd(RT, RA, RB) instructions::divfd::execute(RT, RA, RB, __LINE__);
#define subfd(RT, RA, RB) instructions::subfd::execute(RT, RA, RB, __LINE__);

#define addfc(RT, RA, RB) instructions::addfc::execute(RT, RA, RB, __LINE__);
#define subfc(RT, RA, RB) instructions::subfc::execute(RT, RA, RB, __LINE__);
#define mulfc(RT, RA, RB) instructions::mulfc::execute(RT, RA, RB, __LINE__);
#define divfc(RT, RA, RB) instructions::divfc::execute(RT, RA, RB, __LINE__);

#define conjfc(RT, RA) instructions::conjfc::execute(RT, RA, __LINE__);

#define addfz(RT, RA, RB) instructions::addfz::execute(RT, RA, RB, __LINE__);
#define subfz(RT, RA, RB) instructions::subfz::execute(RT, RA, RB, __LINE__);
#define mulfz(RT, RA, RB) instructions::mulfz::execute(RT, RA, RB, __LINE__);
#define divfz(RT, RA, RB) instructions::divfz::execute(RT, RA, RB, __LINE__);

#define conjfz(RT, RA) instructions::conjfz::execute(RT, RA, __LINE__);

#define addifd(RT, RA, SI) instructions::addifd::execute(RT, RA, SI, __LINE__);
#define subifd(RT, RA, SI) instructions::subifd::execute(RT, RA, SI, __LINE__);
#define mulifd(RT, RA, SI) instructions::mulifd::execute(RT, RA, SI, __LINE__);
#define divifd(RT, RA, SI) instructions::divifd::execute(RT, RA, SI, __LINE__);

#define maddfs(RT, RA, RB) instructions::maddfs::execute(RT, RA, RB, __LINE__);
#define maddfd(RT, RA, RB) instructions::maddfd::execute(RT, RA, RB, __LINE__);
#define maddfc(RT, RA, RB) instructions::maddfc::execute(RT, RA, RB, __LINE__);
#define maddfz(RT, RA, RB) instructions::maddfz::execute(RT, RA, RB, __LINE__);

#define msubfs(RT, RA, RB) instructions::msubfs::execute(RT, RA, RB, __LINE__);
#define msubfd(RT, RA, RB) instructions::msubfd::execute(RT, RA, RB, __LINE__);
#define msubfc(RT, RA, RB) instructions::msubfc::execute(RT, RA, RB, __LINE__);
#define msubfz(RT, RA, RB) instructions::msubfz::execute(RT, RA, RB, __LINE__);

#define addifs(RT, RA, SI) instructions::addifs::execute(RT, RA, SI, __LINE__);
#define subifs(RT, RA, SI) instructions::subifs::execute(RT, RA, SI, __LINE__);
#define mulifs(RT, RA, SI) instructions::mulifs::execute(RT, RA, SI, __LINE__);
#define divifs(RT, RA, SI) instructions::divifs::execute(RT, RA, SI, __LINE__);

// 5.2. Compare instructions
#define cmpfs(RA, RB) instructions::cmpfs::execute(RA, RB, __LINE__);
#define cmpfd(RA, RB) instructions::cmpfd::execute(RA, RB, __LINE__);
#define cmpfc(RA, RB) instructions::cmpfc::execute(RA, RB, __LINE__);
#define cmpfz(RA, RB) instructions::cmpfz::execute(RA, RB, __LINE__);

#endif
