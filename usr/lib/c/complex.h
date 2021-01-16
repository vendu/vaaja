#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#define complex         _Complex
#define _Complex_I      const float _Complex
#define imaginary       _Imaginary
#define _Imaginary_I    const float _Imaginary

extern double           cacos(double complex dc);
extern double           casin(double complex dc);
extern double           catan(double complex dc);
extern double           cacosh(double complex dc);
extern double           casinh(double complex dc);
extern double           catanh(double complex dc);
extern double           ccos(double complex dc);
extern double           csin(double complex dc);
extern double           ctan(double complex dc);
extern double           ccosh(double complex dc);
extern double           csinh(double complex dc);
extern double           ctanh(double complex dc);
extern double           cexp(double complex dc);
extern double           clog(double complex dc);
extern double           cpow(double complex dc, double dp);
extern double           csqrt(double complex dc);
extern double           cfabs(double complex dc);

extern float            cacosf(float complex fc);
extern float            casinf(float complex fc);
extern float            catanf(float complex fc);
extern float            cacoshf(float complex fc);
extern float            casinhf(float complex fc);
extern float            catanhf(float complex fc);
extern float            ccosf(float complex fc);
extern float            csinf(float complex fc);
extern float            ctanf(float complex fc);
extern float            ccoshf(float complex fc);
extern float            csinhf(float complex fc);
extern float            ctanhf(float complex fc);
extern float            cexpf(float complex fc);
extern float            clogf(float complex fc);
extern float            cpowf(float complex fc, float fp);
extern float            csqrtf(float complex fc);
extern float            cfabsf(float complex fc);

#endif /* __COMPLEX_H__ */
