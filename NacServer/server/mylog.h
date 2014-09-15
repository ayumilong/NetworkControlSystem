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
/* 日志报警等级 */   
typedef enum
{
	LEVEL_DEBUG = 0,	//调试,级别最低,在系统实际运行过程中一般都是不输出的
	LEVEL_INFO	= 1,	//信息,反馈系统的当前状态给最终用户的
	LEVEL_WARN	= 2,	//警告,这时进行一些修复性的工作,系统能够恢复,可以继续运行下去
	LEVEL_ERROR = 3,	//错误,导致一个无法修复的错误
	LEVEL_FATAL = 4		//致命
} LEVEL_TYPE;
//不同等级对应的字符串
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
	int		InitLogCfg(LEVEL_TYPE level, const char* logFileName, unsigned long m_LogFileMaxSize);	//初始化日志配置
	void	WriteLog(LEVEL_TYPE level, const char* format,...); //日志打印接口,参数可变
	void	WriteData(const char* format,...);							//直接写字符串
	void	WriteData_Ttime(const char* format,...);				//写日志，自动填写时间

protected:	
    void PrintLog(const char* szMsg); // 打印输出信息
	void GetDateString(char* szDate); // 获得日期,实现省略
	void GetTimeString(char* szTime); // 获得时间,实现省略
private:
	FILE *m_file;							//日志文件句柄
	LEVEL_TYPE m_iLevel;					//日志级别
	char m_logFilePath[LOG_PATH_LEN_MAX];	//日志文件名
	unsigned long m_LogFileMaxSize;			//日志文件最大大小
#ifdef WIN32
	CRITICAL_SECTION m_cs;					//函数的重入控制
#else
#endif
};

#endif
