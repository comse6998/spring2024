#ifndef _ISA_HH_
#define _ISA_HH_

#include <CDC8600.hh>

// 1. Branch Facility
#define LABEL(L)                                                                                                       \
    L : {                                                                                                              \
    }

#define jmp(L)			if (process(new  instructions::jmp   (0), __LINE__))      goto L;

#define jmpa(L)			if (process(new  instructions::jmpa  (0), __LINE__))      goto L;

#define jmprng(Xj,L)		if (process(new instructions::jmprng (Xj,  0), __LINE__)) goto L;

#define jmpnrng(Xj,L)		if (process(new instructions::jmpnrng(Xj,  0), __LINE__)) goto L;

#define jmpz(Xj,L)		if (process(new instructions::jmpz   (Xj,  0), __LINE__)) goto L;

#define jmpnz(Xj,L)		if (process(new instructions::jmpnz  (Xj,  0), __LINE__)) goto L;

#define jmpp(Xj,L)		if (process(new instructions::jmpp   (Xj,  0), __LINE__)) goto L;

#define jmpn(Xj,L)		if (process(new instructions::jmpn   (Xj,  0), __LINE__)) goto L;

#define bb(Xj,Xk,L)		if (process(new instructions::bb     (Xj, Xk), __LINE__)) goto L;

#define call(Xj,func)		if (process(new instructions::call   (Xj), __LINE__))     func ();

#define calla(Xj,func)		if (process(new instructions::calla  (Xj), __LINE__))     func ();

#define callp(Xj,Xk)		if (process(new instructions::callpi (Xj, Xk), __LINE__));

#define jmpk(Xj,k)		if (process(new instructions::jmpk   (Xj, k), __LINE__))  return;

#define rdjki(Xi, Xj, Xk)	process(new instructions::rdjki(Xi, Xj, Xk), __LINE__);

#define rdjk(Xj, Xk)	process(new instructions::rdjk(Xj, Xk), __LINE__);

#define sdjki(Xi, Xj, Xk)	process(new instructions::sdjki(Xi, Xj, Xk), __LINE__);

#define rdKj(Xj, K)		process(new instructions::rdKj(Xj, K), __LINE__);

#define xkj(Xj, k)		process(new instructions::xkj(Xj, k), __LINE__);

#define isjki(Xi, Xj, Xk)	process(new instructions::isjki(Xi, Xj, Xk), __LINE__);

#define idjkj(Xj, k)		process(new instructions::idjkj(Xj, k), __LINE__);

#define idzkj(Xj, Xk)		process(new instructions::idzkj(Xj, Xk), __LINE__);

#define isjkj(Xj, Xk)		process(new instructions::isjkj(Xj, Xk), __LINE__);

#define ipjkj(Xj, Xk)		process(new instructions::ipjkj(Xj, Xk), __LINE__);

#define lpjkj(Xj, Xk)		process(new instructions::lpjkj(Xj, Xk), __LINE__);

#define fmul(Xi, Xj, Xk)	process(new instructions::fmul(Xi, Xj, Xk), __LINE__);

#define fadd(Xi, Xj, Xk)	process(new instructions::fadd(Xi, Xj, Xk), __LINE__);

#define fsub(Xi, Xj, Xk)	process(new instructions::fsub(Xi, Xj, Xk), __LINE__);

#endif // _ISA_HH_
