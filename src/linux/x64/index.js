const os = require("os");
const hikopenapi = require(`${genPath()}`);

function genPath() {
    const platform = os.platform();
    const arch = process.arch;
    let scriptBasePath = ``;

    if (platform === 'win32') {
        scriptBasePath = "./lib/windows/hikopenapi.node";
    } else if (platform === 'linux') {
        scriptBasePath = `./build/Release/hikopenapi.node`;
    }
    return scriptBasePath;
}

/**
 *  say hello,没有参数
 */
const sayHello= ()=>{
    return hikopenapi.sayHello();
}
/**
 *  say something
 */
const saySomething= ()=>{
    return hikopenapi.saySomething(__dirname);
}

/** artemis封装post接口
 * 
 * @param {*} url 完整请求url,如https://123.123.123.123:443/artemis/api/video/v1/cameras/previewURLs
 * @param {*} headers 请求头,json格式，如{"content-type":"application/json;charset=UTF-8","accept":"application/json"}
 * @param {*} body 请求消息体，请自行使用JSON.stringify处理json
 * @param {*} appKey 认证参数,请联系海康工作人员获取
 * @param {*} appSecret 认证参数,请联系海康工作人员获取
 * @param {*} timeout 超时时长，推荐配置15
 */
const httpPost=(url,headers,body,appKey,appSecret,timeout)=>{
    return hikopenapi.artemisHttpPost(__dirname,url,headers,body,appKey,appSecret,timeout);
}

/** artemis封装put接口
 * 
 * @param {*} url 完整请求url,如https://123.123.123.123:443/artemis/api/video/v1/cameras/previewURLs
 * @param {*} headers 请求头,json格式，如{"content-type":"application/json;charset=UTF-8","accept":"application/json"}
 * @param {*} body 请求消息体，请自行使用JSON.stringify处理json
 * @param {*} appKey 认证参数,请联系海康工作人员获取
 * @param {*} appSecret 认证参数,请联系海康工作人员获取
 * @param {*} timeout 超时时长，推荐配置15
 */
const httpPut=(url,headers,body,appKey,appSecret,timeout)=>{
    return hikopenapi.artemisHttpPut(__dirname,url,headers,body,appKey,appSecret,timeout);
}

/** artemis封装get接口
 * 
 * @param {*} url 完整请求url,如https://123.123.123.123:443/artemis/api/video/v1/cameras/previewURLs
 * @param {*} headers 请求头,json格式，如{"content-type":"application/json;charset=UTF-8","accept":"application/json"}
 * @param {*} appKey 认证参数,请联系海康工作人员获取
 * @param {*} appSecret 认证参数,请联系海康工作人员获取
 * @param {*} timeout 超时时长，推荐配置15
 */
const httpGet=(url,headers,appKey,appSecret,timeout)=>{
    return hikopenapi.artemisHttpGet(__dirname,url,headers,appKey,appSecret,timeout);
}


/** artemis封装delete接口
 * 
 * @param {*} url 完整请求url,如https://123.123.123.123:443/artemis/api/video/v1/cameras/previewURLs
 * @param {*} headers 请求头,json格式，如{"content-type":"application/json;charset=UTF-8","accept":"application/json"}
 * @param {*} appKey 认证参数,请联系海康工作人员获取
 * @param {*} appSecret 认证参数,请联系海康工作人员获取
 * @param {*} timeout 超时时长，推荐配置15
 */
const httpDelete=(url,headers,appKey,appSecret,timeout)=>{
    return hikopenapi.artemisHttpDelete(__dirname,url,headers,appKey,appSecret,timeout);
}

module.exports = {
    httpPost,
    httpGet,
    httpPut,
    httpDelete,
    sayHello,
    saySomething,
};

let res=httpPost('https://44.3.123.123:443/artemis/api/resource/v1/cameras/indexCode',{"content-type":"application/json"},"hello","12345","12aasfasdfasdfasdf",5);
console.log(res);
