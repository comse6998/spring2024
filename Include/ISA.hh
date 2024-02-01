#ifndef _ISA_HH_
#define _ISA_HH_

#include <CDC8600.hh>

// 1. Branch Facility
#define LABEL(L)                                                                                                       \
    L : {                                                                                                              \
    }

#define jmp(L)			goto L;

#define jmpz(Xj,L)		if (instructions::jmpz(Xj)) goto L; //this is how we execute the program

#define jmpp(Xj,L)		if (instructions::jmpp(Xj)) goto L;

#define jmpk(Xj,k)		return; // just return whatever call it

#define xkj(Xj, k)		instructions::xkj(Xj, k);

#define rdjki(Xi, Xj, Xk)	instructions::rdjki(Xi, Xj, Xk);

#define sdjki(Xi, Xj, Xk)	instructions::sdjki(Xi, Xj, Xk);

#define isjki(Xi, Xj, Xk)	instructions::isjki(Xi, Xj, Xk);

#define idjkj(Xj, k)		instructions::idjkj(Xj, k);

#define idzkj(Xj, Xk)		instructions::idzkj(Xj, Xk);

#define isjkj(Xj, Xk)		instructions::isjkj(Xj, Xk);

#define ipjkj(Xj, Xk)		instructions::ipjkj(Xj, Xk);

#define rdKj(Xj, K)		instructions::rdKj(Xj, K);

#endif // _ISA_HH_
