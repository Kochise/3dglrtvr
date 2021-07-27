/*

		Fast inline definitions for linear algebra operations

*/

#define X	0
#define Y	1
#define Z	2

typedef double Vector[3];

#define MakeVector(x, y, z, v)          (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecScale(S,a)   (a)[0] *= S ; (a)[1] *= S ; (a)[2] *= S
#define VecNegate(a)    (a)[0]=0-(a)[0];\
                        (a)[1]=0-(a)[1];\
                        (a)[2]=0-(a)[2];
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a)       (sqrt(VecDot(a,a)))
#define VecNorm(a)		{ double sf = 1.0 / VecLen(a); VecScale(sf, a); }
#define VecCopy(a,b)     (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c)    (c)[0]=(a)[0]+(b)[0];\
                         (c)[1]=(a)[1]+(b)[1];\
                         (c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c)    (c)[0]=(a)[0]-(b)[0];\
                         (c)[1]=(a)[1]-(b)[1];\
                         (c)[2]=(a)[2]-(b)[2]
#define VecComb(A,a,B,b,c)      (c)[0]=(A)*(a)[0]+(B)*(b)[0];\
                                (c)[1]=(A)*(a)[1]+(B)*(b)[1];\
                                (c)[2]=(A)*(a)[2]+(B)*(b)[2]
#define VecAddS(A,a,b,c)         (c)[0]=(A)*(a)[0]+(b)[0];\
                                 (c)[1]=(A)*(a)[1]+(b)[1];\
                                 (c)[2]=(A)*(a)[2]+(b)[2]
#define VecCross(a,b,c)  (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1];\
                         (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2];\
                         (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define VecPrint(msg,v)         printf("%s %g %g %g\n", msg,\
                                        (v)[0],(v)[1],(v)[2])
#define VecZero(v)      (v)[0]=0.0;(v)[1]=0.0;v[2]=0.0
