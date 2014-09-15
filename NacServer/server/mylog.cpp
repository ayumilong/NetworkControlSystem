/***************************************************************
* 
* FileName��		MyLog.h
* Function Note��	Program Log implementation.
* 
****************************************************************/
#include <stdio.h>
#include "mylog.h"
//���캯��
CMyLog::CMyLog()
{	
	//��ʼ������
	m_file = NULL;
	memset(m_logFilePath, 0, sizeof(m_logFilePath));
#ifdef WIN32
	InitializeCriticalSection(&m_cs);
#else
#endif
}
//��������
CMyLog::~CMyLog()
{
	if (NULL != m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}
#ifdef WIN32
	DeleteCriticalSection(&m_cs);
#else
#endif
}
//��ʼ����־����
int CMyLog::InitLogCfg(LEVEL_TYPE level, const char* logFilePath,unsigned long logMaxSize)	//��ʼ����־����
{
	char logFileName[LOG_PATH_LEN_MAX];
	struct tm *nowtime;
	time_t long_time;
	time( &long_time );                
	nowtime = localtime( &long_time );

	m_iLevel = level;
	m_LogFileMaxSize = logMaxSize;
	memcpy(m_logFilePath, logFilePath, strlen(logFilePath));

#ifdef WIN32
	sprintf( logFileName, "%s-%04d%02d%02d%02d%02d%02d.csv",
		m_logFilePath, nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday,nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec); 
#else
        sprintf( logFileName, "%s", m_logFilePath);
#endif

	if (logFileName != NULL)
	{
                m_file = fopen( logFileName, "a+" );
		if (m_file == NULL)
		{
			return -1;
		}
	}
	return 1;
}
void CMyLog::WriteLog(LEVEL_TYPE logLevel, const char* format,...)
{
	char logMsg[LOG_BUF_LEN_MAX]; 	//��־��Ϣ�ַ���
	memset(logMsg,0,sizeof(logMsg));

	if (NULL == format)				//������,�ж��Ƿ�����־��Ϣ����
	{
		return;
	}
	if (logLevel < m_iLevel)		//�ȼ�����,����ѡ����־����LEVEL_ERROR,��ôWARN��Ϣ���������־
	{
		return;
	}
#ifdef WIN32
	SYSTEMTIME	sysTime;			//ϵͳʱ��
	GetLocalTime( &sysTime );		//���ϵͳ��ǰʱ��
	sprintf( logMsg, "%04d%02d%02d-%02d:%02d:%02d:%03d [%5s] " ,
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds,LOG_ERROR[logLevel] );
#else
        struct tm *nowtime;
	time_t long_time;
	struct timeval t;
        gettimeofday(&t, NULL);
	time( &long_time );                
	nowtime = localtime( &long_time );
	sprintf( logMsg, "%04d%02d%02d-%02d:%02d:%02d:%d [%5s] ",
		nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday,
		nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec,(t.tv_usec/1000), LOG_ERROR[logLevel]);
#endif

	va_list argp;					//���� 
	va_start(argp,format);

#ifdef WIN32
	_vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#else
	vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#endif	
	va_end(argp);
	//�����־
#ifdef WIN32
	EnterCriticalSection(&m_cs);
	this->PrintLog(logMsg);
	LeaveCriticalSection(&m_cs);
#else
	this->PrintLog(logMsg);
#endif	

}

void CMyLog::PrintLog(const char* szMsg)
{
	unsigned long	ul32Pos	= 0;		//��־��С
	if (m_file == NULL)					//��־�ļ���δ��
	{
		return;
	}
	fseek( m_file, 0, SEEK_END );
	ul32Pos = ftell( m_file );
	if( ul32Pos >= m_LogFileMaxSize )	//������־�ļ���С
	{		
		char logName[256];
		struct tm *nowtime;
		time_t long_time;
		time( &long_time );                
		nowtime = localtime( &long_time );  		
#ifdef WIN32
		sprintf( logName, "%s-%04d%02d%02d%02d%02d%02d.csv",
			m_logFilePath, nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday,nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec); 
#else
                sprintf( logName, "%s",
                        m_logFilePath);
#endif

		m_file = fopen( logName, "w+" );
		if ( m_file == NULL )
		{	
			return;
		}
	}	
	fprintf( m_file, "%s \n", szMsg );
	fflush( m_file );	
}
//д���ݣ��Զ���д��ǰʱ��
void CMyLog::WriteData_Ttime(const char* format,...)
{
	char logMsg[LOG_BUF_LEN_MAX]; 	//��־��Ϣ�ַ���
	memset(logMsg,0,sizeof(logMsg));
	
	if (NULL == format)				//������,�ж��Ƿ�����־��Ϣ����W
	{
		return;
	}

#ifdef WIN32
	SYSTEMTIME	sysTime;			//ϵͳʱ��
	GetLocalTime( &sysTime );		//���ϵͳ��ǰʱ��
	sprintf( logMsg, "%04d%02d%02d-%02d:%02d:%02d:%03d," ,
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
#else
	struct tm *nowtime;
	time_t long_time;
	struct timeval t;
    gettimeofday(&t, NULL);
	time( &long_time );                
	nowtime = localtime( &long_time );
	sprintf( logMsg, "%04d%02d%02d-%02d:%02d:%02d:%d,",
		nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday,
		nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec,(t.tv_usec/1000));
#endif
	
	va_list argp;					//���� 
        va_start(argp,format);
	
#ifdef WIN32
	_vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#else
	vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#endif	
	va_end(argp);
	//�����־
#ifdef WIN32
	EnterCriticalSection(&m_cs);
	this->PrintLog(logMsg);
	LeaveCriticalSection(&m_cs);
#else
	this->PrintLog(logMsg);
#endif	
	
}

//д���ݣ��Զ���д��ǰʱ��
void CMyLog::WriteData(const char* format,...)
{
	char logMsg[LOG_BUF_LEN_MAX]; 	//��־��Ϣ�ַ���
	memset(logMsg,0,sizeof(logMsg));
	
	if (NULL == format)				//������,�ж��Ƿ�����־��Ϣ����
	{
		return;
	}
		
	va_list argp;					//���� 
	va_start(argp,format);
	
#ifdef WIN32
	_vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#else
	vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#endif	
	va_end(argp);
	//�����־
#ifdef WIN32
	EnterCriticalSection(&m_cs);
	this->PrintLog(logMsg);
	LeaveCriticalSection(&m_cs);
#else
	this->PrintLog(logMsg);
#endif	
	
}
