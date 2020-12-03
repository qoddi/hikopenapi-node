#pragma once
#include <string>
#include <map>
#include <list>
using std::string;
using std::map;
using std::list;

#ifdef HTTPUTILLIB_EXPORTS
#define HTTPUTILLIB_API extern __declspec(dllexport)
#else
#define HTTPUTILLIB_API __declspec(dllimport)
#endif

#define STDCALL __stdcall

	/**
	* HTTP GET
	*
	* @param url                  http://host+path+query
	* @param headers              Httpͷ
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param timeout              ��ʱʱ�䣨�룩
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ���ý��
	*/
	
	HTTPUTILLIB_API string STDCALL HttpGet(string url, map<string, string> headers, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http POST �ַ���
	*
	* @param url                  http://host+path+query
	* @param headers              Httpͷ
	* @param body                 �ַ���������
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param timeout              ��ʱʱ�䣨�룩
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ���ý��
	*/
	HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, string body, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http POST �ֽ�����
	* 
	* @param url                  http://host+path+query
	* @param headers              Httpͷ
	* @param body                 �ֽ�����
	* @param bodySize             �ֽ�����Ĵ�С
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param timeout              ��ʱʱ�䣨�룩
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ���ý��
	*/
	HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, const char *body, unsigned int bodySize, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http POST ����
	*
	* @param url                  http://host+path+query
	* @param headers              Httpͷ
	* @param bodys                ��������
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param timeout              ��ʱʱ�䣨�룩
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ���ý��
	*/
	HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, map<string, string> bodys, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http PUT
	*
	* @param url                  http://host+path+query
	* @param headers              Httpͷ
	* @param body                 ���ϴ�����
	* @param bodySize             ���ݴ�С
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param timeout              ��ʱʱ�䣨�룩
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ���ý��
	*/
	HTTPUTILLIB_API string STDCALL HttpPut(string url, map<string, string> headers, const char *body, unsigned int bodySize, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* Http DELETE
	*
	* @param url                  http://host+path+query
	* @param headers              Httpͷ
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param timeout              ��ʱʱ�䣨�룩
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ���ý��
	*/
	HTTPUTILLIB_API string STDCALL HttpDelete(string url, map<string, string> headers, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);

	/**
	* ��ʼ������Header
	*
	* @param headers              Httpͷ
	* @param appKey               APP KEY
	* @param appSecret            APP��Կ
	* @param method               Http����
	* @param requestAddress       http://host+path+query
	* @param formParam            ��������
	* @param signHeaderPrefixList �Զ������ǩ��Headerǰ׺
	* @return ����Header
	*/
	