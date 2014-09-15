/***************************************************************
* 
* FileName：		MyLog.h
* Function Note：	Program Log implementation.
* 
****************************************************************/
#include <stdio.h>
#include "mylog.h"
//构造函数
CMyLog::CMyLog()
{	
	//初始化变量
	m_file = NULL;
	memset(m_logFilePath, 0, sizeof(m_logFilePath));
#ifdef WIN32
	InitializeCriticalSection(&m_cs);
#else
#endif
}
//析构函数
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
//初始化日志配置
int CMyLog::InitLogCfg(LEVEL_TYPE level, const char* logFilePath,unsigned long logMaxSize)	//初始化日志配置
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
	char logMsg[LOG_BUF_LEN_MAX]; 	//日志信息字符串
	memset(logMsg,0,sizeof(logMsg));

	if (NULL == format)				//输入检查,判断是否有日志信息输入
	{
		return;
	}
	if (logLevel < m_iLevel)		//等级控制,例如选择日志级别LEVEL_ERROR,那么WARN信息不会输出日志
	{
		return;
	}
#ifdef WIN32
	SYSTEMTIME	sysTime;			//系统时间
	GetLocalTime( &sysTime );		//获得系统当前时间
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

	va_list argp;					//内容 
	va_start(argp,format);

#ifdef WIN32
	_vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#else
	vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#endif	
	va_end(argp);
	//输出日志
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
	unsigned long	ul32Pos	= 0;		//日志大小
	if (m_file == NULL)					//日志文件尚未打开
	{
		return;
	}
	fseek( m_file, 0, SEEK_END );
	ul32Pos = ftell( m_file );
	if( ul32Pos >= m_LogFileMaxSize )	//超过日志文件大小
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
//写数据，自动填写当前时间
void CMyLog::WriteData_Ttime(const char* format,...)
{
	char logMsg[LOG_BUF_LEN_MAX]; 	//日志信息字符串
	memset(logMsg,0,sizeof(logMsg));
	
	if (NULL == format)				//输入检查,判断是否有日志信息输入W
	{
		return;
	}

#ifdef WIN32
	SYSTEMTIME	sysTime;			//系统时间
	GetLocalTime( &sysTime );		//获得系统当前时间
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
	
	va_list argp;					//内容 
        va_start(argp,format);
	
#ifdef WIN32
	_vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#else
	vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#endif	
	va_end(argp);
	//输出日志
#ifdef WIN32
	EnterCriticalSection(&m_cs);
	this->PrintLog(logMsg);
	LeaveCriticalSection(&m_cs);
#else
	this->PrintLog(logMsg);
#endif	
	
}

//写数据，自动填写当前时间
void CMyLog::WriteData(const char* format,...)
{
	char logMsg[LOG_BUF_LEN_MAX]; 	//日志信息字符串
	memset(logMsg,0,sizeof(logMsg));
	
	if (NULL == format)				//输入检查,判断是否有日志信息输入
	{
		return;
	}
		
	va_list argp;					//内容 
	va_start(argp,format);
	
#ifdef WIN32
	_vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#else
	vsnprintf(&logMsg[strlen(logMsg)], LOG_BUF_LEN_MAX-strlen(logMsg), format, argp);
#endif	
	va_end(argp);
	//输出日志
#ifdef WIN32
	EnterCriticalSection(&m_cs);
	this->PrintLog(logMsg);
	LeaveCriticalSection(&m_cs);
#else
	this->PrintLog(logMsg);
#endif	
	
}
