#include <stdio.h>
#include <node_api.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <memory.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <HttpUtil.h>
#include <map>
#include <list>

using namespace std;

#define SO_FILE "libHttpUtil.so"

const char *funName_HttpGet = "HTTPUTIL_Get";
const char *funName_HttpPost = "HTTPUTIL_Post";
const char *funName_GetLastStatus = "HTTPUTIL_GetLastStatus";
const char *funName_Free = "HTTPUTIL_Free";

typedef char *(*pFUNHttpGet)(const char *, const char *, const char *, int, int *);
typedef char *(*pFUNHttpPost)(const char *, const char *, const char *, const char *, int, int *);
typedef int (*pFUNGetLastStatus)();
typedef void (*pFUNFree)(char *);

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

///////////////////// 动态调用时需要指定的内容 ///////////////////////
std::string artemisGet(std::string dllPath, std::string url, std::map<string, string> headers, std::string appKey, std::string appSecret, int timeout)
{
    fstream fs("log.txt", ios::out | ios::trunc);
    void *handle = NULL;
    char *error = NULL;
    stringstream ss;
    //ss2 << dllPath.c_str() << "/lib/linux/x64";
    ss << "libHttpUtil.so";
    cout << "[dlopen]dll full path: " << ss.str().c_str() << endl;
    cout << "[artemisGet]request url:" << url << "\n"
         << endl;

    handle = dlopen(ss.str().c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle)
    {
        fs << "dll load failed\n"
           << endl;
        printf("[dlopen]dll load failed\n");
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30000}";
    }
    else
    {
        printf("[dlopen]dll load success\n");
    }

    pFUNHttpGet pf = (pFUNHttpGet)dlsym(handle, funName_HttpGet);
    if ((error = dlerror()) != NULL)
    {
        fs << "[dlsym]load fp pFUNHttpGet failed\n"
           << endl;
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30001}";
    }
    else
    {

        printf("[dlsym]fp pFUNHttpPost load success\n");
    }
    pFUNGetLastStatus pf2 = (pFUNGetLastStatus)dlsym(handle, funName_GetLastStatus);
    if ((error = dlerror()) != NULL)
    {
        fs << "[dlsym]load fp2 pFUNGetLastStatus failed\n"
           << endl;
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30001}";
    }
    else
    {
        printf("[dlsym]fp2 pFUNGetLastStatus load success\n");
    }
    pFUNFree pf3 = (pFUNFree)dlsym(handle, funName_Free);
    if ((error = dlerror()) != NULL)
    {
        fs << "[dlsym]load fp3 pFUNFree failed\n"
           << endl;
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30001}";
    }
    else
    {
        printf("[dlsym]fp3 pFUNFree load success\n");
    }

    int dataLength = 0;
    cout << "url:" << url << ",appKey:" << appKey << ",appSecret:" << appSecret << endl;
    char *rsp = pf(url.c_str(), appKey.c_str(), appSecret.c_str(), timeout, &dataLength);
    string response = "";
    if (NULL == rsp)
    {
        int status = pf2();
        if (status < httpUtil::HTTPUTIL_ERR_BASE)
        {
            //http statud code
            cout << "[ERROR]http status code:" << status << "\n"
                 << endl;
            stringstream ss;
            ss << "{\"status\":false,\"error\":" << status << "}";
            response = ss.str();
        }
        else
        {
            //error code
            cout << "[ERROR]openapi error code:" << status << "\n"
                 << endl;
            stringstream ss;
            ss << "{\"status\":false,\"error\":" << status << "}";
            response = ss.str();
        }
    }
    else
    {
        string szLocal = string(rsp, dataLength);
        char *base64EncodeBuffer = NULL;
        int iRet = base64_encode(szLocal.c_str(), strlen(szLocal.c_str()), &base64EncodeBuffer);
        cout << base64EncodeBuffer << endl;
        string szLocalBase64 = base64EncodeBuffer;
        response = "{\"status\":true,\"error\":0,\"data\":\"" + szLocalBase64 + "\"}";
    }
    cout << "[artemisPost]request response:" << response << "\n"
         << endl;

    //release memory
    pf3(rsp);
    dlclose(handle);
    return response;
}

std::string artemisPost(std::string dllPath, std::string url, std::map<string, string> headers, std::string body, std::string appKey, std::string appSecret, int timeout)
{
    fstream fs("log.txt", ios::out | ios::trunc);
    void *handle = NULL;
    char *error = NULL;
    stringstream ss;
    stringstream ss2;
    //ss << dllPath.c_str() << "libHttpUtil.so";
    ss << "libHttpUtil.so";
    cout << "[dlopen]dll full path: " << ss.str().c_str() << endl;
    cout << "[artemisPost]request url:" << url << "\n"
         << endl;
    cout << "[artemisPost]request body:" << body << "\n"
         << endl;
    handle = dlopen(ss.str().c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!handle)
    {
        fs << "dll load failed" << endl;
        printf("[dlopen]dll load failed\n");
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30000}";
    }
    else
    {
        printf("[dlopen]dll load success\n");
    }

    pFUNHttpPost pf = (pFUNHttpPost)dlsym(handle, funName_HttpPost);
    if ((error = dlerror()) != NULL)
    {
        fs << "[dlsym]load fp pFUNHttpPost failed\n"
           << endl;
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30001}";
    }
    else
    {

        printf("[dlsym]fp pFUNHttpPost load success\n");
    }
    pFUNGetLastStatus pf2 = (pFUNGetLastStatus)dlsym(handle, funName_GetLastStatus);
    if ((error = dlerror()) != NULL)
    {
        fs << "[dlsym]load fp2 pFUNGetLastStatus failed\n"
           << endl;
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30001}";
    }
    else
    {
        printf("[dlsym]fp2 pFUNGetLastStatus load success\n");
    }
    pFUNFree pf3 = (pFUNFree)dlsym(handle, funName_Free);
    if ((error = dlerror()) != NULL)
    {
        fs << "[dlsym]load fp3 pFUNFree failed\n"
           << endl;
        fprintf(stderr, dlerror());
        return "{\"status\":false,\"error\":30001}";
    }
    else
    {
        printf("[dlsym]fp3 pFUNFree load success\n");
    }

    int dataLength = 0;
    cout << "url:" << url << ",body:" << body << ",appKey:" << appKey << ",appSecret:" << appSecret << endl;
    char *rsp = pf(url.c_str(), body.c_str(), appKey.c_str(), appSecret.c_str(), timeout, &dataLength);
    string response = "";
    if (NULL == rsp)
    {
        int status = pf2();
        if (status < httpUtil::HTTPUTIL_ERR_BASE)
        {
            //http statud code
            cout << "[ERROR]http status code:" << status << "\n"
                 << endl;
            stringstream ss;
            ss << "{\"status\":false,\"error\":" << status << "}";
            response = ss.str();
        }
        else
        {
            //error code
            cout << "[ERROR]openapi error code:" << status << "\n"
                 << endl;
            stringstream ss;
            ss << "{\"status\":false,\"error\":" << status << "}";
            response = ss.str();
        }
    }
    else
    {
        string szLocal = string(rsp, dataLength);
        char *base64EncodeBuffer = NULL;
        int iRet = base64_encode(szLocal.c_str(), strlen(szLocal.c_str()), &base64EncodeBuffer);
        cout << base64EncodeBuffer << endl;
        string szLocalBase64 = base64EncodeBuffer;
        response = "{\"status\":true,\"error\":0,\"data\":\"" + szLocalBase64 + "\"}";
    }
    cout << "[artemisPost]request response:" << response << "\n"
         << endl;

    //release memory
    pf3(rsp);
    dlclose(handle);
    return response;
}

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
    //导出方法get
    napi_value ArtemisHttpGet(napi_env env, napi_callback_info info)
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

        printf("dllPath:[%s],url:[%s],appKey:[%s],appSecret:[%s],timeout:[%d]\n", dllPath, url, appKey, appSecret, int(timeout));

        //调用原生cpp方法拿到结果
        std::string artemisRes = artemisGet(std::string(dllPath), std::string(url), std::map<string, string>(headers), std::string(appKey), std::string(appSecret), int(timeout));
        //创建一个返回值对象，包装进result,返回出去
        std::string response = artemisRes; //genSK(std::string(dllPath));
        status = napi_create_string_utf8(env, (char *)response.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok)
            return nullptr;
        return result;
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

        //调用原生cpp方法拿到结果
        std::string artemisRes = artemisPost(std::string(dllPath), std::string(url), std::map<string, string>(headers), std::string(body), std::string(appKey), std::string(appSecret), int(timeout));
        //创建一个返回值对象，包装进result,返回出去
        std::string response = artemisRes; //genSK(std::string(dllPath));
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
            {"artemisHttpGet", 0, ArtemisHttpGet, 0, 0, 0, napi_default, 0},

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
