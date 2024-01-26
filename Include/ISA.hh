#ifndef _ISA_HH_
#define _ISA_HH_

#include <CDC8600.hh>

// 1. Branch Facility

// {} is to make sure we don't have weird things happening
#define LABEL(L)                                                                                                       \
    L : {                                                                                                              \
    }

#define jmp(L)			goto L;

#define jmpz(Xj,L)		if (instructions::jmpz(Xj)) goto L;

#define jmpk(Xj,k)		return;

#define xkj(Xj, k)		instructions::xkj(Xj, k);

#define rdjki(Xi, Xj, Xk)	instructions::rdjki(Xi, Xj, Xk);

#define sdjki(Xi, Xj, Xk)	instructions::sdjki(Xi, Xj, Xk);

#define isjki(Xi, Xj, Xk)	instructions::isjki(Xi, Xj, Xk);

#define idjkj(Xj, k)		instructions::idjkj(Xj, k);

#endif // _ISA_HH_
