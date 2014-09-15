/**********************************************
 * Filename:MyLog.h
 * 
 *
 **********************************************/
#ifndef MYLOG_H
#define MYLOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define LOG_BUF_LEN_MAX		1024
#define LOG_PATH_LEN_MAX	256
/* ��־�����ȼ� */   
typedef enum
{
	LEVEL_DEBUG = 0,	//����,�������,��ϵͳʵ�����й�����һ�㶼�ǲ������
	LEVEL_INFO	= 1,	//��Ϣ,����ϵͳ�ĵ�ǰ״̬�������û���
	LEVEL_WARN	= 2,	//����,��ʱ����һЩ�޸��ԵĹ���,ϵͳ�ܹ��ָ�,���Լ���������ȥ
	LEVEL_ERROR = 3,	//����,����һ���޷��޸��Ĵ���
	LEVEL_FATAL = 4		//����
} LEVEL_TYPE;
//��ͬ�ȼ���Ӧ���ַ���
const char LOG_ERROR[][8] = {
    "DEBUG",
	" INFO",
	" WARN",
	"ERROR",
	"FATAL"
};
class CMyLog
{
public:
	CMyLog();
	~CMyLog();
	int		InitLogCfg(LEVEL_TYPE level, const char* logFileName, unsigned long m_LogFileMaxSize);	//��ʼ����־����
	void	WriteLog(LEVEL_TYPE level, const char* format,...); //��־��ӡ�ӿ�,�����ɱ�
	void	WriteData(const char* format,...);							//ֱ��д�ַ���
	void	WriteData_Ttime(const char* format,...);				//д��־���Զ���дʱ��

protected:	
    void PrintLog(const char* szMsg); // ��ӡ�����Ϣ
	void GetDateString(char* szDate); // �������,ʵ��ʡ��
	void GetTimeString(char* szTime); // ���ʱ��,ʵ��ʡ��
private:
	FILE *m_file;							//��־�ļ����
	LEVEL_TYPE m_iLevel;					//��־����
	char m_logFilePath[LOG_PATH_LEN_MAX];	//��־�ļ���
	unsigned long m_LogFileMaxSize;			//��־�ļ�����С
#ifdef WIN32
	CRITICAL_SECTION m_cs;					//�������������
#else
#endif
};

#endif
