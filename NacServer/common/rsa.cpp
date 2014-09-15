#include "rsa.h"
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

unsigned long long RSA::gcd(const unsigned long long p,const unsigned long long q)
{
	unsigned long long a=p>q?p:q;
	unsigned long long b=p<q?p:q;
	unsigned long long t;
	if(p==q)
	{
		return p;
	}
	else
	{
		while(b)
		{
			a=a%b;
			t=a;
			a=b;
			b=t;
		}
		return a;
	}
}

unsigned long long RSA::euclid(const unsigned long long p,const unsigned long long q)
{
	unsigned long long max=0xffffffffffffffff-q;
	unsigned long long i=1;
	while(1)
	{
		if(((i*q)+1)%p==0)
		{
			return ((i*q)+1)/p;
		}
		i++;
		unsigned long long temp=(i+1)*q;
		if(temp>max)
		{
			return 0;
		}
	}
	return 0;
}

unsigned long long RSA::mulmod(const unsigned long long a,const unsigned long long b,const unsigned long long n)
{
	return (a%n)*(b%n)%n;
}

unsigned long long RSA::powmod(const unsigned long long base,unsigned long long pow,const unsigned long long n)
{
	unsigned long long a=base,b=pow,c=1;
	while(b)
	{
		while(!(b&1))
		{
			b>>=1;
		    a=mulmod(a,a,n);
		}
		b--;
		c=mulmod(a,c,n);
	}
	return c;
}

void RSA::generatekey()
{
	srand((unsigned int)time(NULL));
	p=263;
	q=75629;
	n=p*q;
	f=(p-1)*(q-1);
	do
	{
		e=rand();
		e|=1;
	}while(gcd(e,f)!=1);
//	printf("f=%I64d\n",f);
//	printf("e=%I64d\n",e);
	d=euclid(e,f);
//  printf("d=%I64d\n",d);
}

unsigned long long RSA::getN()
{
	return n;
}

void RSA::setN(const unsigned long long n)
{
	this->n=n;
}

unsigned long long RSA::getE()
{
	return e;
}

void RSA::setE(const unsigned long long e)
{
	this->e=e;
}

unsigned long long RSA::getD()
{
	return d;
}

void RSA::setD(const unsigned long long d)
{
	this->d=d;
}

unsigned long long RSA::encry(const unsigned long long in)
{
	return powmod(in,e,n);
}

unsigned long long RSA::decry(const unsigned long long in)
{
	return powmod(in,d,n);
}