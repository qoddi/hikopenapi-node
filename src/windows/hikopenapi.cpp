#define HTTPUTILLIB_EXPORTS
#include <node_api.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <memory.h>
#include <list>
#include <tchar.h>
#include <HttpUtillib.h>
#include <HttpHeader.h>

#pragma comment(lib, "HttpUtillib.lib");


using namespace std;
///////////////////// 动态调用时需要指定的内容 (已废弃)///////////////////////
const char *funName_HttpPost = "HttpPost";
const char *funName_HttpGet = "HttpGet";
const char *funName_HttpPut = "HttpPut";
const char *funName_HttpDelete = "HttpDelete";
const char *funName_InitialBasicHeader = "initialBasicHeader";

//HTTPUTILLIB_API string STDCALL HttpPost(string url, map<string, string> headers, string body, string appKey, string appSecret, int timeout, list<string> signHeaderPrefixList);
typedef string(CALLBACK *PFUNHttpPost)(string, map<string, string>, string, string, string, int, list<string>);

///////////////////// 动态调用时需要指定的内容 ///////////////////////

///////////////////// UTF-8转本地编码以及本地编码转UTF-8 ///////////////////////

// 宽字符字符串转多字节字符串
inline std::string _W2A_(const wchar_t *pwszText)
{
    if (pwszText == NULL || wcslen(pwszText) == 0)
    {
        return std::string();
    }
    int iSizeInBytes = WideCharToMultiByte(CP_ACP, 0, pwszText, -1, NULL, 0, NULL, NULL);
    char *pMultiByte = new (std::nothrow) char[iSizeInBytes];
    if (pMultiByte == NULL)
    {
        return std::string();
    }

    memset(pMultiByte, 0, iSizeInBytes);
    WideCharToMultiByte(CP_ACP, 0, pwszText, -1, pMultiByte, iSizeInBytes, NULL, NULL);

    std::string strResult = std::string(pMultiByte);
    delete[] pMultiByte;
    pMultiByte = NULL;
    return strResult;
}

// UTF-8字符串转宽字符字符串
inline std::wstring _U82W_(const char *pszText)
{
    if (pszText == NULL || strlen(pszText) == 0)
    {
        return std::wstring();
    }
    int iSizeInChars = MultiByteToWideChar(CP_UTF8, 0, pszText, -1, NULL, 0);
    wchar_t *pWideChar = new (std::nothrow) wchar_t[iSizeInChars];
    if (pWideChar == NULL)
    {
        return std::wstring();
    }

    wmemset(pWideChar, 0, iSizeInChars);
    MultiByteToWideChar(CP_UTF8, 0, pszText, -1, pWideChar, iSizeInChars);

    std::wstring strResult = std::wstring(pWideChar);
    delete[] pWideChar;
    pWideChar = NULL;
    return strResult;
}

// UTF-8字符串转多字节字符串
inline std::string _U82A_(const char *pszText)
{
    return _W2A_(_U82W_(pszText).c_str());
}

// 多字节字符串转宽字符字符串
inline std::wstring _A2W_(const char *pszText)
{
    if (pszText == NULL || strlen(pszText) == 0)
    {
        return std::wstring();
    }
    int iSizeInChars = MultiByteToWideChar(CP_ACP, 0, pszText, -1, NULL, 0);
    wchar_t *pWideChar = new (std::nothrow) wchar_t[iSizeInChars];
    if (pWideChar == NULL)
    {
        return std::wstring();
    }

    wmemset(pWideChar, 0, iSizeInChars);
    MultiByteToWideChar(CP_ACP, 0, pszText, -1, pWideChar, iSizeInChars);

    std::wstring strResult = std::wstring(pWideChar);
    delete[] pWideChar;
    pWideChar = NULL;
    return strResult;
}

// 宽字符字符串转UTF-8字符串
inline std::string _W2U8_(const wchar_t *pwszText)
{
    if (pwszText == NULL || wcslen(pwszText) == 0)
    {
        return std::string();
    }
    int iSizeInBytes = WideCharToMultiByte(CP_UTF8, 0, pwszText, -1, NULL, 0, NULL, NULL);
    char *pUTF8 = new (std::nothrow) char[iSizeInBytes];
    if (pUTF8 == NULL)
    {
        return std::string();
    }

    memset(pUTF8, 0, iSizeInBytes);
    WideCharToMultiByte(CP_UTF8, 0, pwszText, -1, pUTF8, iSizeInBytes, NULL, NULL);

    std::string strResult = std::string(pUTF8);
    delete[] pUTF8;
    pUTF8 = NULL;
    return strResult;
}

// 多字节字符串转UTF-8字符串
inline std::string _A2U8_(const char *pszText)
{
    return _W2U8_(_A2W_(pszText).c_str());
}

#define _U82A(lpu8Text) (const_cast<char *>(_U82A_(static_cast<const char *>(lpu8Text)).c_str()))
#define _A2U8(lpszText) (const_cast<char *>(_A2U8_(static_cast<const char *>(lpszText)).c_str()))
///////////////////// UTF-8转本地编码以及本地编码转UTF-8 ///////////////////////

///////////////////// 认证库内置部分函数 ///////////////////////
static char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int pos(char c)
{
    char *p;
    for (p = base64; *p; p++)
        if (*p == c)
            return p - base64;
    return -1;
}

int base64_encode(const void *data, int size, char **str)
{
    char *s, *p;
    int i;
    int c;
    const unsigned char *q;

    s = (char *)malloc(size * 4 / 3 + 4);
    p = s;
    if (p == NULL)
        return -1;
    q = (const unsigned char *)data;
    i = 0;
    for (i = 0; i < size;)
    {
        c = q[i++];
        c *= 256;
        if (i < size)
            c += q[i];
        i++;
        c *= 256;
        if (i < size)
            c += q[i];
        i++;
        p[0] = base64[(c & 0x00fc0000) >> 18];
        p[1] = base64[(c & 0x0003f000) >> 12];
        p[2] = base64[(c & 0x00000fc0) >> 6];
        p[3] = base64[(c & 0x0000003f) >> 0];
        if (i > size)
            p[3] = '=';
        if (i > size + 1)
            p[2] = '=';
        p += 4;
    }
    *p = 0;
    *str = s;
    //if (s == NULL)
    //    return -1;
    return strlen(s);
}

int base64_decode(const char *str, void *data)
{
    const char *p;
    unsigned char *q;
    int c;
    int x;
    int done = 0;
    q = (unsigned char *)data;
    for (p = str; *p && !done; p += 4)
    {
        x = pos(p[0]);
        if (x >= 0)
            c = x;
        else
        {
            done = 3;
            break;
        }
        c *= 64;

        x = pos(p[1]);
        if (x >= 0)
            c += x;
        else
            return -1;
        c *= 64;

        if (p[2] == '=')
            done++;
        else
        {
            x = pos(p[2]);
            if (x >= 0)
                c += x;
            else
                return -1;
        }
        c *= 64;

        if (p[3] == '=')
            done++;
        else
        {
            if (done)
                return -1;
            x = pos(p[3]);
            if (x >= 0)
                c += x;
            else
                return -1;
        }
        if (done < 3)
            *q++ = (c & 0x00ff0000) >> 16;

        if (done < 2)
            *q++ = (c & 0x0000ff00) >> 8;
        if (done < 1)
            *q++ = (c & 0x000000ff) >> 0;
    }
    return q - (unsigned char *)data;
}

void SplitString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

std::string UTF8ToGB(const char *str)
{
    std::string result;
    WCHAR *strSrc;
    TCHAR *szRes;

    //�����ʱ�����Ĵ�С
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //�����ʱ�����Ĵ�С
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new TCHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[] strSrc;
    delete[] szRes;

    return result;
}

void string2tchar(std::string &src, TCHAR *buf)
{
#ifdef UNICODE
    _stprintf_s(buf, MAX_PATH, _T("%S"), src.c_str()); //%S宽字符
#else
    _stprintf_s(buf, MAX_PATH, _T("%s"), src.c_str()); //%s单字符
#endif
}

///////////////////// 认证库内置部分函数 ///////////////////////

///////////////////// 自有函数 ///////////////////////

//cpp获取上一个错误代码
void getLastError(string action)
{
    DWORD err = GetLastError();
    printf("[getLastError]action [%s],error code:[%d]\n", action, err);
}

//真实调用的post方法
std::string artemisPost(std::string dllPath, std::string url, std::map<string, string> headers, std::string body, std::string appKey, std::string appSecret, std::double_t timeout)
{

    list<string> signHeaderPrefixList;
    cout << "[artemisPost]request url:" << url << "\n"
         << endl;
    cout << "[artemisPost]request body:" << body << "\n"
         << endl;
    string szUtf8Body = _A2U8(body.c_str());
    string szResponse = HttpPost(url, headers, szUtf8Body, appKey, appSecret, int(timeout), signHeaderPrefixList);
    string szLocal = _U82A(szResponse.c_str());
    cout << "[artemisPost]request response:" << szLocal << "\n"
         << endl;
    char* base64EncodeBuffer = NULL;
	int iRet = base64_encode(szLocal.c_str(), strlen(szLocal.c_str()), &base64EncodeBuffer);
    cout<<base64EncodeBuffer<<endl;
    string szLocalBase64=base64EncodeBuffer;
    return  "{\"status\":true,\"error\":0,\"data\":\"" + szLocalBase64+ "\"}";
}

//真实调用的get方法
std::string artemisGet(std::string dllPath, std::string url, std::map<string, string> headers, std::string appKey, std::string appSecret, std::double_t timeout)
{

    printf("hello");
    list<string> signHeaderPrefixList;
    cout << "[artemisGet]request url:" << url << "\n"
         << endl;
    string szResponse = HttpGet(url, headers, appKey, appSecret, int(timeout), signHeaderPrefixList);
    string szLocal = _U82A(szResponse.c_str());
    cout << "[artemisGet]request response:" << szLocal << "\n"
         << endl;
    char* base64EncodeBuffer = NULL;
	int iRet = base64_encode(szLocal.c_str(), strlen(szLocal.c_str()), &base64EncodeBuffer);
    cout<<base64EncodeBuffer<<endl;
    string szLocalBase64=base64EncodeBuffer;
    return  "{\"status\":true,\"error\":0,\"data\":\"" + szLocalBase64+ "\"}";
}

//真实调用的put方法
std::string artemisPut(std::string dllPath, std::string url, std::map<string, string> headers, std::string body, std::string appKey, std::string appSecret, std::double_t timeout)
{

    list<string> signHeaderPrefixList;
    cout << "[artemisPut]request url:" << url << "\n"
         << endl;
    cout << "[artemisPut]request body:" << body << "\n"
         << endl;
    string szUtf8Body = _A2U8(body.c_str());
    const char *bodyP = body.c_str();
    string szResponse = HttpPut(url, headers, bodyP, strlen(bodyP), appKey, appSecret, int(timeout), signHeaderPrefixList);
    string szLocal = _U82A(szResponse.c_str());
    cout << "[artemisPut]request response:" << szLocal << "\n"
         << endl;
    char* base64EncodeBuffer = NULL;
	int iRet = base64_encode(szLocal.c_str(), strlen(szLocal.c_str()), &base64EncodeBuffer);
    cout<<base64EncodeBuffer<<endl;
    string szLocalBase64=base64EncodeBuffer;
    return  "{\"status\":true,\"error\":0,\"data\":\"" + szLocalBase64+ "\"}";
}

//真实调用的delete方法
std::string artemisDelete(std::string dllPath, std::string url, std::map<string, string> headers, std::string appKey, std::string appSecret, std::double_t timeout)
{

    list<string> signHeaderPrefixList;
    cout << "[artemisDelete]request url:" << url << "\n"
         << endl;
    string szResponse = HttpDelete(url, headers, appKey, appSecret, int(timeout), signHeaderPrefixList);
    string szLocal = _U82A(szResponse.c_str());
    cout << "[artemisDelete]request response:" << szLocal << "\n"
         << endl;
    char* base64EncodeBuffer = NULL;
	int iRet = base64_encode(szLocal.c_str(), strlen(szLocal.c_str()), &base64EncodeBuffer);
    cout<<base64EncodeBuffer<<endl;
    string szLocalBase64=base64EncodeBuffer;
    return  "{\"status\":true,\"error\":0,\"data\":\"" + szLocalBase64+ "\"}";
}

//根据程序demo提供的方法进行测试调用的函数
int artemisPostDemo()
{
    // Step1：根据实际综合安防管理平台情况设置IP地址、端口号、APPKey和APPSecret
    Unit_query_t query;
    query.appKey = "25747611";                // ������APPKey
    query.appSecret = "zt4sP15zdqKjtBYVypkl"; // ������APPSecret
    query.artemisIp = "44.136.134.5";         // ƽ̨IP��ַ
    query.artemisPort = 443;                  // HTTP��HTTPSЭ��˿ڣ���Ҫ����ʵ��ʹ��HTTP����HTTPSЭ������䣬�˴�ʹ��HTTPSЭ��

    // Step2：组装POST请求URL（以HTTPS协议为例）
    std::stringstream ss;
    ss << "https://" << query.artemisIp << ":" << query.artemisPort << "/artemis/api/video/v1/cameras/previewURLs";
    std::string szUrl = ss.str();

    // Step3：根据期望的Response内容类型组装请求头
    map<string, string> headers;
    headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_ACCEPT, "application/json"));
    headers.insert(std::make_pair(HttpHeader::HTTP_HEADER_CONTENT_TYPE, "application/json;charset=UTF-8"));

    // Step4：请求超时时间与自定义参与签名参数列表
    int iTimeOut = 15;
    list<string> signHeaderPrefixList;

    // Step5：组装body(此处直接组装字符串，也可以使用json来组装，报文较复杂时，建议使用json来组装)
    string szCameraIndexCode = "65d2e1d5bad64443967811519ed06d37";
    string szBody = "{\"cameraIndexCode\": \"" + szCameraIndexCode + "\",\"streamType\":0,\"protocol\":\"rtsp\",\"transmode\":0,\"expand\":\"transcode=0\"}";
    cout << szBody << endl;

    // Step6：对body转成UTF-8编码
    string szUtf8Body = _A2U8(szBody.c_str());

    // Step7：发起POST请求
    std::string szResponse = HttpPost(szUrl, headers, szBody, query.appKey, query.appSecret, iTimeOut, signHeaderPrefixList);
    // 或使用HttpPost的另一种形式：
    //std::string szResponse = HttpPost(szUrl, headers, szBody.c_str(), query.appKey, query.appSecret, iTimeOut, signHeaderPrefixList);

    // Step8：将响应转成本地编码（中文操作系统下认为是GBK）
    string szLocal = _U82A(szResponse.c_str());

    // Step9：解析json报文（此处直接打印出来）
    std::cout << szLocal << std::endl;

    return 0;
}

/*   动态调用demo,由于函数名无法加载，已经废弃

std::string artemisPost(std::string dllPath, std::string url, std::map<string, string> headers, std::string body, std::string appKey, std::string appSecret, std::double_t timeout)
{
    fstream fs("log.txt", ios::out | ios::trunc);

    stringstream ss;
    //ss << UTF8ToGB(dllPath.c_str()) << "\\lib\\windows1\\Identify.dll";
    ss << UTF8ToGB(dllPath.c_str()) << "\\lib\\windows\\HttpUtillib.dll";
    printf("[LoadLibraryEx]dll full path: %s\n", ss.str().c_str());
    //HMODULE hDLL = LoadLibraryA(ss.str().c_str());
    //由于包含子依赖文件，所以改用LoadLibraryEx加载库，也可以采用
    HMODULE hDLL = LoadLibraryEx(_T(ss.str().c_str()), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (hDLL == NULL)
    {
        fs << "dll load failed" << endl;
        printf("[LoadLibraryEx]dll load failed\n");
        getLastError("LoadLibraryEx");

        return "";
    }
    else
    {
        printf("[LoadLibraryEx]dll load success\n");
    }
    list<string> signHeaderPrefixList;

    PFUNHttpPost fp1 = (PFUNHttpPost)GetProcAddress(hDLL, funName_HttpPost);

    if (fp1 == NULL)
    {

        fs << "fp load failed" << endl;

        printf("[GetProcAddress]fp load failed\n");
        getLastError("GetProcAddress");
        return "";
    }
    string szResponse = ""; // fp1(url, headers, body, appKey, appSecret, timeout, signHeaderPrefixList);

    FreeLibrary(hDLL);
    return szResponse;
}
*/

///////////////////// 自有函数 ///////////////////////

namespace openapi
{

    napi_value SaySomething(napi_env env, napi_callback_info info)
    {
        napi_value result;
        napi_status status;
        //从外部读取的参数个数
        napi_value argv[1];
        //参数个数
        size_t argc = sizeof(argv) / sizeof(napi_value);
        napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
        //创建一个dll地址对象，从外部读取这个值
        char dllPath[2048] = "";
        size_t data_size_n = 0;
        if (napi_ok != napi_get_value_string_utf8(env, argv[0], dllPath, sizeof(dllPath) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        printf(dllPath);

        //创建一个返回值对象，包装进result,返回出去
        std::string response = "good bye"; //genSK(std::string(dllPath));
        status = napi_create_string_utf8(env, (char *)response.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok)
            return nullptr;
        return result;
    }

    napi_value SayHello(napi_env env, napi_callback_info info)
    {
        printf("Hello\n");
        return NULL;
    }

    //导出方法post
    napi_value ArtemisHttpPost(napi_env env, napi_callback_info info)
    {
        napi_value result;
        napi_status status;
        //从外部读取的参数个数8个参数
        napi_value argv[8];
        //参数个数
        size_t argc = sizeof(argv) / sizeof(napi_value);
        napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

        size_t data_size_n = 0;
        //第一个参数，dll地址对象，从外部读取这个值
        char dllPath[2048] = "";
        //第二个参数，请求的url
        char url[2048] = "";
        //第三个参数，请求头
        map<string, string> headers;
        //第四个参数，请求体，string
        char body[2048] = "";
        //第五个参数，appKey
        char appKey[2048] = "";
        //第六个参数, appSecret
        char appSecret[2048] = "";
        //第七个参数,timeout
        double timeout;
        //第八个参数 签名附属参数列表，一般无需指定

        if (napi_ok != napi_get_value_string_utf8(env, argv[0], dllPath, sizeof(dllPath) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[1], url, sizeof(url) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        //headers对象保存到obj
        napi_value obj;
        obj = argv[2];

        //获取所有的对象列表
        napi_value propertyList;
        uint32_t propertyListLength = 0;
        status = napi_get_property_names(env, obj, &propertyList);              //获取所有参数列表
        status = napi_get_array_length(env, propertyList, &propertyListLength); //计算参数列表长度
        printf("[ArtemisHttpPost]propertyListlength:%d\n", propertyListLength);
        for (uint32_t n = 0; n < propertyListLength; ++n)
        {
            napi_value listKey, listValue;
            char listKeyString[2048], listValueString[2048] = "";
            //遍历参数列表，获取key，再通过key获取value，并存入map中
            status = napi_get_element(env, propertyList, n, &listKey);
            napi_get_value_string_utf8(env, listKey, listKeyString, sizeof(listKeyString) / sizeof(char), &data_size_n);
            napi_get_named_property(env, obj, listKeyString, &listValue);
            napi_get_value_string_utf8(env, listValue, listValueString, sizeof(listValueString) / sizeof(char), &data_size_n);
            printf("[ArtemisHttpPost][http headers] %d %s---%s \n", n, listKeyString, listValueString);
            headers.insert(std::make_pair<string, string>(listKeyString, listValueString));
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[3], body, sizeof(body) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        if (napi_ok != napi_get_value_string_utf8(env, argv[4], appKey, sizeof(appKey) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        if (napi_ok != napi_get_value_string_utf8(env, argv[5], appSecret, sizeof(appSecret) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_double(env, argv[6], &timeout))
        {
            return nullptr;
        }

        printf("dllPath:[%s],url:[%s],body:[%s],appKey:[%s],appSecret:[%s],timeout:[%d]\n", dllPath, url, body, appKey, appSecret, int(timeout));

        //artemisPostDemo();
        //调用原生cpp方法拿到结果
        std::string artemisRes = artemisPost(std::string(dllPath), std::string(url), std::map<string, string>(headers), std::string(body), std::string(appKey), std::string(appSecret), std::double_t(timeout));
        //创建一个返回值对象，包装进result,返回出去
        std::string response = artemisRes; //genSK(std::string(dllPath));
        status = napi_create_string_utf8(env, (char *)response.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok)
            return nullptr;
        return result;
    }

    //导出方法put
    napi_value ArtemisHttpPut(napi_env env, napi_callback_info info)
    {
        napi_value result;
        napi_status status;
        //从外部读取的参数个数8个参数
        napi_value argv[8];
        //参数个数
        size_t argc = sizeof(argv) / sizeof(napi_value);
        napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

        size_t data_size_n = 0;
        //第一个参数，dll地址对象，从外部读取这个值
        char dllPath[2048] = "";
        //第二个参数，请求的url
        char url[2048] = "";
        //第三个参数，请求头
        map<string, string> headers;
        //第四个参数，请求体，string
        char body[2048] = "";
        //第五个参数，appKey
        char appKey[2048] = "";
        //第六个参数, appSecret
        char appSecret[2048] = "";
        //第七个参数,timeout
        double timeout;
        //第八个参数 签名附属参数列表，一般无需指定

        if (napi_ok != napi_get_value_string_utf8(env, argv[0], dllPath, sizeof(dllPath) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[1], url, sizeof(url) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        //headers对象保存到obj
        napi_value obj;
        obj = argv[2];

        //获取所有的对象列表
        napi_value propertyList;
        uint32_t propertyListLength = 0;
        status = napi_get_property_names(env, obj, &propertyList);              //获取所有参数列表
        status = napi_get_array_length(env, propertyList, &propertyListLength); //计算参数列表长度
        printf("[ArtemisHttpPut]propertyListlength:%d\n", propertyListLength);
        for (uint32_t n = 0; n < propertyListLength; ++n)
        {
            napi_value listKey, listValue;
            char listKeyString[2048], listValueString[2048] = "";
            //遍历参数列表，获取key，再通过key获取value，并存入map中
            status = napi_get_element(env, propertyList, n, &listKey);
            napi_get_value_string_utf8(env, listKey, listKeyString, sizeof(listKeyString) / sizeof(char), &data_size_n);
            napi_get_named_property(env, obj, listKeyString, &listValue);
            napi_get_value_string_utf8(env, listValue, listValueString, sizeof(listValueString) / sizeof(char), &data_size_n);
            printf("[ArtemisHttpPut][http headers] %d %s---%s \n", n, listKeyString, listValueString);
            headers.insert(std::make_pair<string, string>(listKeyString, listValueString));
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[3], body, sizeof(body) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        if (napi_ok != napi_get_value_string_utf8(env, argv[4], appKey, sizeof(appKey) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        if (napi_ok != napi_get_value_string_utf8(env, argv[5], appSecret, sizeof(appSecret) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_double(env, argv[6], &timeout))
        {
            return nullptr;
        }

        printf("dllPath:[%s],url:[%s],body:[%s],appKey:[%s],appSecret:[%s],timeout:[%d]\n", dllPath, url, body, appKey, appSecret, int(timeout));

        //artemisPostDemo();
        //调用原生cpp方法拿到结果
        std::string artemisRes = artemisPut(std::string(dllPath), std::string(url), std::map<string, string>(headers), std::string(body), std::string(appKey), std::string(appSecret), std::double_t(timeout));
        //创建一个返回值对象，包装进result,返回出去
        std::string response = artemisRes; //genSK(std::string(dllPath));
        status = napi_create_string_utf8(env, (char *)response.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok)
            return nullptr;
        return result;
    }

    //导出方法get
    napi_value ArtemisHttpGet(napi_env env, napi_callback_info info)
    {
        napi_value result;
        napi_status status;
        //从外部读取的参数个数7个参数
        napi_value argv[7];
        //参数个数
        size_t argc = sizeof(argv) / sizeof(napi_value);
        napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

        size_t data_size_n = 0;
        //第一个参数，dll地址对象，从外部读取这个值
        char dllPath[2048] = "";
        //第二个参数，请求的url
        char url[2048] = "";
        //第三个参数，请求头
        map<string, string> headers;
        //第四个参数，appKey
        char appKey[2048] = "";
        //第五个参数, appSecret
        char appSecret[2048] = "";
        //第六个参数,timeout
        double timeout;

        //第八个参数 签名附属参数列表，一般无需指定

        if (napi_ok != napi_get_value_string_utf8(env, argv[0], dllPath, sizeof(dllPath) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[1], url, sizeof(url) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        //headers对象保存到obj
        napi_value obj;
        obj = argv[2];

        //获取所有的对象列表
        napi_value propertyList;
        uint32_t propertyListLength = 0;
        status = napi_get_property_names(env, obj, &propertyList);              //获取所有参数列表
        status = napi_get_array_length(env, propertyList, &propertyListLength); //计算参数列表长度
        printf("[ArtemisHttpGet]propertyListlength:%d\n", propertyListLength);
        for (uint32_t n = 0; n < propertyListLength; ++n)
        {
            napi_value listKey, listValue;
            char listKeyString[2048], listValueString[2048] = "";
            //遍历参数列表，获取key，再通过key获取value，并存入map中
            status = napi_get_element(env, propertyList, n, &listKey);
            napi_get_value_string_utf8(env, listKey, listKeyString, sizeof(listKeyString) / sizeof(char), &data_size_n);
            napi_get_named_property(env, obj, listKeyString, &listValue);
            napi_get_value_string_utf8(env, listValue, listValueString, sizeof(listValueString) / sizeof(char), &data_size_n);
            printf("[ArtemisHttpGet][http headers] %d %s---%s \n", n, listKeyString, listValueString);
            headers.insert(std::make_pair<string, string>(listKeyString, listValueString));
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[3], appKey, sizeof(appKey) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        if (napi_ok != napi_get_value_string_utf8(env, argv[4], appSecret, sizeof(appSecret) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_double(env, argv[5], &timeout))
        {
            return nullptr;
        }

        printf("url:[%s],appKey:[%s],appSecret:[%s],timeout:[%d]\n", url, appKey, appSecret, int(timeout));

        //调用原生cpp方法拿到结果
        std::string artemisRes = artemisGet(std::string(dllPath), std::string(url), std::map<string, string>(headers), std::string(appKey), std::string(appSecret), std::double_t(timeout));
        //创建一个返回值对象，包装进result,返回出去
        std::string response = artemisRes;
        status = napi_create_string_utf8(env, (char *)response.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok)
            return nullptr;
        return result;
    }

    //导出方法delete
    napi_value ArtemisHttpDelete(napi_env env, napi_callback_info info)
    {
        napi_value result;
        napi_status status;
        //从外部读取的参数个数7个参数
        napi_value argv[7];
        //参数个数
        size_t argc = sizeof(argv) / sizeof(napi_value);
        napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

        size_t data_size_n = 0;
        //第一个参数，dll地址对象，从外部读取这个值
        char dllPath[2048] = "";
        //第二个参数，请求的url
        char url[2048] = "";
        //第三个参数，请求头
        map<string, string> headers;
        //第四个参数，appKey
        char appKey[2048] = "";
        //第五个参数, appSecret
        char appSecret[2048] = "";
        //第六个参数,timeout
        double timeout;
        //第七个参数 签名附属参数列表，一般无需指定

        if (napi_ok != napi_get_value_string_utf8(env, argv[0], dllPath, sizeof(dllPath) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[1], url, sizeof(url) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        //headers对象保存到obj
        napi_value obj;
        obj = argv[2];

        //获取所有的对象列表
        napi_value propertyList;
        uint32_t propertyListLength = 0;
        status = napi_get_property_names(env, obj, &propertyList);              //获取所有参数列表
        status = napi_get_array_length(env, propertyList, &propertyListLength); //计算参数列表长度
        printf("[ArtemisHttpDelete]propertyListlength:%d\n", propertyListLength);
        for (uint32_t n = 0; n < propertyListLength; ++n)
        {
            napi_value listKey, listValue;
            char listKeyString[2048], listValueString[2048] = "";
            //遍历参数列表，获取key，再通过key获取value，并存入map中
            status = napi_get_element(env, propertyList, n, &listKey);
            napi_get_value_string_utf8(env, listKey, listKeyString, sizeof(listKeyString) / sizeof(char), &data_size_n);
            napi_get_named_property(env, obj, listKeyString, &listValue);
            napi_get_value_string_utf8(env, listValue, listValueString, sizeof(listValueString) / sizeof(char), &data_size_n);
            printf("[ArtemisHttpDelete][http headers] %d %s---%s \n", n, listKeyString, listValueString);
            headers.insert(std::make_pair<string, string>(listKeyString, listValueString));
        }

        if (napi_ok != napi_get_value_string_utf8(env, argv[3], appKey, sizeof(appKey) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }
        if (napi_ok != napi_get_value_string_utf8(env, argv[4], appSecret, sizeof(appSecret) / sizeof(char), &data_size_n))
        {
            return nullptr;
        }

        if (napi_ok != napi_get_value_double(env, argv[5], &timeout))
        {
            return nullptr;
        }

        printf("url:[%s],appKey:[%s],appSecret:[%s],timeout:[%d]\n", url, appKey, appSecret, int(timeout));

        //调用原生cpp方法拿到结果
        std::string artemisRes = artemisDelete(std::string(dllPath), std::string(url), std::map<string, string>(headers), std::string(appKey), std::string(appSecret), std::double_t(timeout));
        //创建一个返回值对象，包装进result,返回出去
        std::string response = artemisRes;
        status = napi_create_string_utf8(env, (char *)response.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok)
            return nullptr;
        return result;
    }

    napi_value init(napi_env env, napi_value exports)
    {

        napi_property_descriptor properties[] = {
            {"sayHello", 0, SayHello, 0, 0, 0, napi_default, 0},
            {"saySomething", 0, SaySomething, 0, 0, 0, napi_default, 0},
            {"artemisHttpPost", 0, ArtemisHttpPost, 0, 0, 0, napi_default, 0},
            {"artemisHttpPut", 0, ArtemisHttpPut, 0, 0, 0, napi_default, 0},
            {"artemisHttpGet", 0, ArtemisHttpGet, 0, 0, 0, napi_default, 0},
            {"artemisHttpDelete", 0, ArtemisHttpDelete, 0, 0, 0, napi_default, 0},

        };
        napi_status status = napi_define_properties(env, exports, sizeof(properties) / sizeof(properties[0]), properties);
        if (napi_ok != status)
        {
            napi_throw_error(env, "INITIALIZE_FAILED", "Failed to initialize");
            return nullptr;
        }
        return exports;
    }

    NAPI_MODULE(hikopenapi, init);
} // namespace openapi
