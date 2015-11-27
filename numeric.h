#ifndef __NUMERIC_H__
#define __NUMERIC_H__

#define RND_MAX 10000

#ifndef __BOOL_T__
#define __BOOL_T__
typedef enum{TRUE=1,FALSE=0}bool_t;
#endif



float add(float,float);
float subs(float,float);
float product(float,float);
float division(float,float,bool_t*);
float exponent(float,int,bool_t*);
long unsigned factorial(long unsigned,bool_t*);
long unsigned binomial(int,int,bool_t*);
float random(float,float,bool_t*);

#endif
