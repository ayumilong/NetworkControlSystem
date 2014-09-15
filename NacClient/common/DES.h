#ifndef CDES_H_CAESAR__DEF  
#define CDES_H_CAESAR__DEF  
  
/* Brief CDES��˵��  
 * 
 * ������DES��3DES�㷨��  
 */  
class CDES    
{  
public:  
    CDES();  
    virtual ~CDES();  
  
    //���ܽ���  
    enum      
    {  
        ENCRYPT = 0,    // ����  
        DECRYPT     // ����  
    };  
  
    //DES�㷨��ģʽ  
    enum  
    {  
        ECB = 0,  // ECBģʽ  
        CBC       // CBCģʽ  
    };  
  
    typedef bool    (*PSubKey)[16][48];  
  
    //Pad����ģʽ  
    enum  
    {  
        PAD_ISO_1 = 0,  // ISO_1��䣺���ݳ��Ȳ���8���صı�������0x00���㣬���Ϊ8���صı�������8��0x00  
        PAD_ISO_2,      // ISO_2��䣺���ݳ��Ȳ���8���صı�������0x80,0x00..���㣬���Ϊ8���صı�������0x80,0x00..0x00  
        PAD_PKCS_7      // PKCS7��䣺���ݳ��ȳ�8����Ϊn,��(8-n)����Ϊ8�ı���  
    };  
  
/* Brief ����8λ����  
 * 
 * Description�� ����Э��Լ���ǰ�����ݽ������ 
 * @param nType :���ͣ�PAD���� 
 * @param In :���ݴ�ָ�� 
 * @param Out :��������ָ�� 
 * @param datalen :���ݵĳ��� 
 * @param padlen :(in,out)���buffer�ĳ��ȣ�����ĳ��� 
 * @return true--�ɹ���false--ʧ�ܣ�  
 */  
    static bool RunPad(int nType,const unsigned char* In,  
        unsigned datalen,unsigned char* Out,unsigned& padlen);  
  
/* Brief ִ��DES�㷨���ı��ӽ��� 
 * 
 * Description ��ִ��DES�㷨���ı��ӽ��� 
 * @param bType : ���ͣ�����ENCRYPT������DECRYPT 
 * @param bMode : ģʽ��ECB,CBC 
 * @param In :�����ܴ�ָ�� 
 * @param Out :�������ָ�� 
 * @param datalen :�����ܴ��ĳ��ȣ�ͬʱOut�Ļ�������СӦ���ڻ��ߵ���datalen 
 * @param Key :��Կ(��Ϊ8λ,16λ,24λ)֧��3��Կ 
 * @param keylen : ��Կ���ȣ����24λ���ֽ����Զ��ü� 
 * @return true--�ɹ���false--ʧ�ܣ�  
 */  
    static bool RunDes(bool bType,bool bMode,const unsigned char* In,  
         unsigned char* Out,unsigned datalen,const unsigned char* Key,unsigned keylen);  
  
protected:  
    //���㲢�������Կ��SubKey������  
    static void SetSubKey(PSubKey pSubKey, const unsigned char Key[8]);  
      
    //DES��Ԫ����  
    static void DES(unsigned char Out[8], const unsigned char In[8],  
         const PSubKey pSubKey, bool Type);  
};  
  
#endif//CDES_H_CAESAR__DEF  