#ifndef RSA_H
#define RSA_H
class RSA
{
private:
	unsigned long long p,q,n,e,d,f;
public:
	unsigned long long encry(const unsigned long long in);//����
	unsigned long long decry(const unsigned long long in);//����
    void generatekey();//���ɹ�˽Կ��
    unsigned long long getN();
	void setN(const unsigned long long n);
	unsigned long long getE();
	void setE(const unsigned long long e);
	unsigned long long getD();
	void setD(const unsigned long long d);
private:
	unsigned long long gcd(const unsigned long long p,const unsigned long long q);//�������������Լ��
	unsigned long long euclid(const unsigned long long p,const unsigned long long q);//��չ��ŷ������㷨��˷���Ԫ
	unsigned long long mulmod(const unsigned long long a,const unsigned long long b,const unsigned long long n);//ģ��
	unsigned long long powmod(const unsigned long long base,unsigned long long pow,const unsigned long long n);//ģ��
};
#endif
