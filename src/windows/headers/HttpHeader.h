#pragma once

/**
 * HTTP头常量
 */
namespace HttpHeader 
{
    //请求Header Accept
    const string HTTP_HEADER_ACCEPT = "accept";
    //请求Body内容MD5 Header
    const string HTTP_HEADER_CONTENT_MD5 = "content-md5";
    //请求Header Content-Type
    const string HTTP_HEADER_CONTENT_TYPE = "content-type";
    //请求Header UserAgent
    const string HTTP_HEADER_USER_AGENT = "user-agent";
    //请求Header Date
    const string HTTP_HEADER_DATE = "date";
}

/**
 * 常用HTTP Content-Type常量
 */
namespace ContentType 
{
    //表单类型Content-Type
    const string CONTENT_TYPE_FORM = "application/x-www-form-urlencoded;charset=UTF-8";
    // 流类型Content-Type
    const string CONTENT_TYPE_STREAM = "application/octet-stream;charset=UTF-8";
    //JSON类型Content-Type
    const string CONTENT_TYPE_JSON = "application/json;charset=UTF-8";
    //XML类型Content-Type
    const string CONTENT_TYPE_XML = "application/xml;charset=UTF-8";
    //文本类型Content-Type
    const string CONTENT_TYPE_TEXT = "application/text;charset=UTF-8";
}

/**
* 常用参数
*/
typedef struct Unit_query_s
{
	string userName;               //用户名，对接CAS的时候需要传
	string artemisIp;              //API 网关ip地址（必填）
	int artemisPort;               //API 网关Port（必填）
	string appKey;                 //API 网关的appKey（必填）
	string appSecret;              //API 网关的appSecret（必填）
	string unitCode;               //父组织编号
	string treeNode;               //组织节点编号
	string cameraIndexCode;         //监控点编号
	int start;						// 分页查找开始 从0开始
	int limit;						// 分页查找页数
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
