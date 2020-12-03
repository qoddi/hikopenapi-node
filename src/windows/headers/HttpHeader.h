#pragma once

/**
 * HTTPͷ����
 */
namespace HttpHeader 
{
    //����Header Accept
    const string HTTP_HEADER_ACCEPT = "accept";
    //����Body����MD5 Header
    const string HTTP_HEADER_CONTENT_MD5 = "content-md5";
    //����Header Content-Type
    const string HTTP_HEADER_CONTENT_TYPE = "content-type";
    //����Header UserAgent
    const string HTTP_HEADER_USER_AGENT = "user-agent";
    //����Header Date
    const string HTTP_HEADER_DATE = "date";
}

/**
 * ����HTTP Content-Type����
 */
namespace ContentType 
{
    //������Content-Type
    const string CONTENT_TYPE_FORM = "application/x-www-form-urlencoded;charset=UTF-8";
    // ������Content-Type
    const string CONTENT_TYPE_STREAM = "application/octet-stream;charset=UTF-8";
    //JSON����Content-Type
    const string CONTENT_TYPE_JSON = "application/json;charset=UTF-8";
    //XML����Content-Type
    const string CONTENT_TYPE_XML = "application/xml;charset=UTF-8";
    //�ı�����Content-Type
    const string CONTENT_TYPE_TEXT = "application/text;charset=UTF-8";
}

/**
* ���ò���
*/
typedef struct Unit_query_s
{
	string userName;               //�û������Խ�CAS��ʱ����Ҫ��
	string artemisIp;              //API ����ip��ַ�����
	int artemisPort;               //API ����Port�����
	string appKey;                 //API ���ص�appKey�����
	string appSecret;              //API ���ص�appSecret�����
	string unitCode;               //����֯���
	string treeNode;               //��֯�ڵ���
	string cameraIndexCode;         //��ص���
	int start;						// ��ҳ���ҿ�ʼ ��0��ʼ
	int limit;						// ��ҳ����ҳ��
	string order;
	string sortby;
	string body;                     //POST

	Unit_query_s()
	{
		artemisPort = 0;
		start = 0;
		limit = 1000;
		order = "name";
	}

}Unit_query_t;
