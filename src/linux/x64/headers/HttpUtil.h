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
	// ״̬�붨��: 
	// 0 ��ʾ�ɹ�
	// 10000~20000 ��ʾ����Ĵ�����. 
	// С��10000����0 ��ʾHTTP״̬�루��404����HTTP״̬�������в���HTTP״̬�붨��
	const int  HTTPUTIL_ERR_SUCCESS = 0;                                   // �ɹ�
	const int  HTTPUTIL_ERR_BASE = 10000;                                  // �������׼
	const int  HTTPUTIL_ERR_PARAM = (HTTPUTIL_ERR_BASE + 1);               // ���������紫���ָ�롢��ʱʱ�䲻��
	const int  HTTPUTIL_ERR_ALLOC_SESSION = (HTTPUTIL_ERR_BASE + 2);       // HTTP�������sessionʧ��
	const int  HTTPUTIL_ERR_SET_HEADER_FIELD = (HTTPUTIL_ERR_BASE + 3);    // ͷ������ʧ��
	const int  HTTPUTIL_ERR_SEND_REQUEST = (HTTPUTIL_ERR_BASE + 4);        // HTTP����ʧ��
	const int  HTTPUTIL_ERR_ALLOC_MEM = (HTTPUTIL_ERR_BASE + 5);           // �����ڴ����
	const int  HTTPUTIL_ERR_GET_REDIRECT_URL = (HTTPUTIL_ERR_BASE + 6);    // ��ȡ�ض���Urlʧ��
	const int  HTTPUTIL_ERR_OTHER = (HTTPUTIL_ERR_BASE + 10000);           // ��������

	// HTTP POST
	// url: url
	// body: body
	// appkey: API�����ṩ��APPKey
	// secret: API�����ṩ��APPSecret
	// timeout: �뼶��ʱʱ�䣨�ڲ��ֱ�ʹ�ô˳�ʱʱ�����ý������ӡ������͡��ȴ��ظ���ʱʱ�䣩
	// dataLen: ��Ӧ���ݳ��ȣ����ȷ�����ص��������ַ���������ָ��NULL
	// return: HTTP������Ӧ������Ƕ����ƣ�����dataLen��ȡ���ݡ�ʧ�ܷ���NULL����ʹ��HTTPUTIL_GetLastStatus��ȡ״̬�롣�ɹ���ʹ��HTTPUTIL_Free�ͷ��ڴ�
	HTTPUTIL_API char* HTTPUTIL_Post(const char* url, const char* body, const char* appkey, const char* secret, int timeout, int* dataLen);

	// HTTP GET
	// url: url
	// appkey: API�����ṩ��APPKey
	// secret: API�����ṩ��APPSecret
	// timeout: �뼶��ʱʱ�䣨�ڲ��ֱ�ʹ�ô˳�ʱʱ�����ý������ӡ������͡��ȴ��ظ���ʱʱ�䣩
	// dataLen: ��Ӧ���ݳ��ȣ����ȷ�����ص��������ַ���������ָ��NULL
	// return: HTTP������Ӧ������Ƕ����ƣ�����dataLen��ȡ���ݡ�ʧ�ܷ���NULL����ʹ��HTTPUTIL_GetLastStatus��ȡ״̬�롣�ɹ���ʹ��HTTPUTIL_Free�ͷ��ڴ�
	HTTPUTIL_API char* HTTPUTIL_Get(const char* url, const char* appkey, const char* secret, int timeout, int* dataLen);

	// �ͷ��ڴ�
	// buffer: pHTTPUTIL_Get��HTTPUTIL_Post���صķǿ�ָ��
	HTTPUTIL_API void HTTPUTIL_Free(char* buffer);

	// ��ȡ״̬�룬�����ڵ�HTTPUTIL_Post��HTTPUTIL_Get�����߳��е��˽ӿ�
	// return: ״̬��, ���HTTPUTIL_ERR_*�����в���HTTP״̬��
	HTTPUTIL_API int HTTPUTIL_GetLastStatus();
}
#endif