#ifndef RSA_H
#define RSA_H
class RSA
{
private:
	unsigned long long p,q,n,e,d,f;
public:
	unsigned long long encry(const unsigned long long in);//加密
	unsigned long long decry(const unsigned long long in);//解密
    void generatekey();//生成公私钥对
    unsigned long long getN();
	void setN(const unsigned long long n);
	unsigned long long getE();
	void setE(const unsigned long long e);
	unsigned long long getD();
	void setD(const unsigned long long d);
private:
	unsigned long long gcd(const unsigned long long p,const unsigned long long q);//求两个数的最大公约数
	unsigned long long euclid(const unsigned long long p,const unsigned long long q);//扩展的欧几里德算法求乘法逆元
	unsigned long long mulmod(const unsigned long long a,const unsigned long long b,const unsigned long long n);//模乘
	unsigned long long powmod(const unsigned long long base,unsigned long long pow,const unsigned long long n);//模幂
};
#endif
