# HIKOPENAPI-NODE
iSee/iFar Artemis Http Client Nodejs SDK


接口获取:[海康威视开放平台](https://open.hikvision.com)

GITHUB:https://github.com/qoddi/hikopenapi-node.git

适配ISC/PVIA等基于iSee/iFar架构的平台，基于OpenAPI 安全认证库封装了 HTTP/HTTPS 的 POST/GET 方法，提供nodejs下调 OpenAPI HTTP 接口统一的入口。OpenAPI 安全认证库屏蔽了 OpenAPI HTTP 接口签名细节以及重定向处理过程，降低对接OpenAPI 的复杂度，使用方只需引入即可方便快速的实现 HTTP 通信。 

## Support
适配windows x64环境与linux x64环境,已经测试通过的环境包括

【windows 10 + nodejs v10.16.0】 【windows server 2016 + nodejs v10.16.0】 【CentOS-7.4-hik-r4 + nodejs v10.16.0】

平台需部署openapi网关后即可调用相关接口，appKey appSecret请联系海康工作人员获取，请求请使用https://portal_ip:443/artemis/

## Author
qoddi

email:moranran8023@gmail.com


## Installation

```bash
$ npm install hikopenapi-node
```

## Usage

Example:


WINDOWS下安装部署即可直接使用，无需特殊配置，基于cpp认证库，提供post get put delete四种方法支持
```js
const Hikopenapi=require('hikopenapi-node');

let requestUrl='https://123.123.123.123:443/artemis/api/video/v1/cameras/previewURLs';
let headers={"content-type":"application/json","accept":"application/json"};
let body=JSON.stringify({
    "cameraIndexCode":  "fsjdfkj2hkjsdhfkajhskdjahksdjahksjfhsf" ,
    "streamType":0,
    "protocol":"rtsp",
    "transmode":0,
    "expand":"transcode=0"});
let timeout=15;
let appKey='12345';
let appSecret='skjfhakjshdakjh12312jkhk';
//封装认证库POST方法
let httpRes=await Hikopenapi.httpPost(requestUrl,headers,body,appKey,appSecret,timeout);
console.log(httpRes);

//封装认证库GET方法
let httpRes=await Hikopenapi.httpGet(requestUrl,headers,"12345","sfakjshdkjh1",timeout);
console.log(httpRes);

//封装认证库DELETE方法
let httpRes=await Hikopenapi.httpDelete(requestUrl,headers,"12345","sfakjshdkjh1",timeout);
console.log(httpRes);

//封装认证库PUT方法
let httpRes=await Hikopenapi.httpPut(requestUrl,headers,body,"12345","sfakjshdkjh1",timeout);
console.log(httpRes);
```


LINUX下需要配置动态库的环境变量，基于c认证库，提供post get两种方法支持，配置方法如下
1. 查找本模块的安装位置，拷贝目录下lib/linux/x64下全部文件到/root/openapilib文件夹(自行创建)
2. 编辑/etc/ld.so.conf,在最下面新增一行/root/openapiilib并保存退出
3. 执行ldconfig指令,使配置文件生效果，然后就和windows一样使用了

```js
const Hikopenapi=require('hikopenapi-node');

let requestUrl='https://123.123.123.123:443/artemis/api/video/v1/cameras/previewURLs';
let headers={"content-type":"application/json","accept":"application/json"};
let body=JSON.stringify({
    "cameraIndexCode":  "fsjdfkj2hkjsdhfkajhskdjahksdjahksjfhsf" ,
    "streamType":0,
    "protocol":"rtsp",
    "transmode":0,
    "expand":"transcode=0"});
let timeout=15;
let appKey='12345';
let appSecret='skjfhakjshdakjh12312jkhk';
//封装认证库POST方法
let httpRes=await Hikopenapi.httpPost(requestUrl,headers,body,appKey,appSecret,timeout);
console.log(httpRes);

//封装认证库GET方法
let httpRes=await Hikopenapi.httpGet(requestUrl,headers,"12345","sfakjshdkjh1",timeout);
console.log(httpRes);

```


## License

(The MIT License)