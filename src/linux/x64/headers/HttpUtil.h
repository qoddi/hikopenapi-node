#ifndef HIK_HTTPUTIL_H
#define HIK_HTTPUTIL_H

#if (defined(WIN32) || defined(WIN64))
	#ifdef HTTPUTIL_EXPORTS
		#define HTTPUTIL_API extern "C" _declspec(dllexport)
	#else
		#define HTTPUTIL_API extern "C" _declspec(dllimport)
	#endif
#else  // LINUX
	#define HTTPUTIL_API extern "C"
#endif

namespace httpUtil
{
	// 状态码定义: 
	// 0 表示成功
	// 10000~20000 表示具体的错误码. 
	// 小于10000大于0 表示HTTP状态码（如404），HTTP状态码请自行查阅HTTP状态码定义
	const int  HTTPUTIL_ERR_SUCCESS = 0;                                   // 成功
	const int  HTTPUTIL_ERR_BASE = 10000;                                  // 错误码基准
	const int  HTTPUTIL_ERR_PARAM = (HTTPUTIL_ERR_BASE + 1);               // 参数错误，如传入空指针、超时时间不对
	const int  HTTPUTIL_ERR_ALLOC_SESSION = (HTTPUTIL_ERR_BASE + 2);       // HTTP请求分配session失败
	const int  HTTPUTIL_ERR_SET_HEADER_FIELD = (HTTPUTIL_ERR_BASE + 3);    // 头域设置失败
	const int  HTTPUTIL_ERR_SEND_REQUEST = (HTTPUTIL_ERR_BASE + 4);        // HTTP请求失败
	const int  HTTPUTIL_ERR_ALLOC_MEM = (HTTPUTIL_ERR_BASE + 5);           // 分配内存错误
	const int  HTTPUTIL_ERR_GET_REDIRECT_URL = (HTTPUTIL_ERR_BASE + 6);    // 获取重定向Url失败
	const int  HTTPUTIL_ERR_OTHER = (HTTPUTIL_ERR_BASE + 10000);           // 其它错误

	// HTTP POST
	// url: url
	// body: body
	// appkey: API网关提供的APPKey
	// secret: API网关提供的APPSecret
	// timeout: 秒级超时时间（内部分别使用此超时时间设置建立连接、请求发送、等待回复超时时间）
	// dataLen: 响应数据长度，如果确定返回的内容是字符串，可以指定NULL
	// return: HTTP请求响应，如果是二进制，请结合dataLen获取数据。失败返回NULL，可使用HTTPUTIL_GetLastStatus获取状态码。成功需使用HTTPUTIL_Free释放内存
	HTTPUTIL_API char* HTTPUTIL_Post(const char* url, const char* body, const char* appkey, const char* secret, int timeout, int* dataLen);

	// HTTP GET
	// url: url
	// appkey: API网关提供的APPKey
	// secret: API网关提供的APPSecret
	// timeout: 秒级超时时间（内部分别使用此超时时间设置建立连接、请求发送、等待回复超时时间）
	// dataLen: 响应数据长度，如果确定返回的内容是字符串，可以指定NULL
	// return: HTTP请求响应，如果是二进制，请结合dataLen获取数据。失败返回NULL，可使用HTTPUTIL_GetLastStatus获取状态码。成功需使用HTTPUTIL_Free释放内存
	HTTPUTIL_API char* HTTPUTIL_Get(const char* url, const char* appkey, const char* secret, int timeout, int* dataLen);

	// 释放内存
	// buffer: pHTTPUTIL_Get和HTTPUTIL_Post返回的非空指针
	HTTPUTIL_API void HTTPUTIL_Free(char* buffer);

	// 获取状态码，必须在调HTTPUTIL_Post或HTTPUTIL_Get所在线程中调此接口
	// return: 状态码, 详见HTTPUTIL_ERR_*或自行查阅HTTP状态码
	HTTPUTIL_API int HTTPUTIL_GetLastStatus();
}
#endif