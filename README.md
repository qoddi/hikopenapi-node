# HIKOPENAPI-NODE
iSee/iFar Artemis Http Client Nodejs SDK

[海康威视开放平台](https://open.hikvision.com)

适配ISC/PVIA等基于iSee/iFar架构的平台，平台需部署openapi网关后即可调用相关接口

appKey appSecret请联系海康工作人员获取，请求请使用https://运管ip:443/artemis

## Support
目前仅适配windows x64环境，后续会增加linux x64支持

## Author
qoddi

email:chenruizhe@hikvision.com


## Installation

```bash
$ npm install hikopenapi-node
```

## Usage

Example:

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
let appSecret='skjfhakjshdakjh12312jkhk'
//封装认证库POST方法
let httpRes=await Hikopenapi.httpPost(requestUrl,headers,body,appKey,appSecret,timeout);
console.log(res);

//封装认证库GET方法
let httpRes=await Hikopenapi.httpGet(requestUrl,headers,"12345","sfakjshdkjh1",timeout);
console.log(res);

//封装认证库DELETE方法
let httpRes=await Hikopenapi.httpDelete(requestUrl,headers,"12345","sfakjshdkjh1",timeout);
console.log(res);

//封装认证库PUT方法
let httpRes=await Hikopenapi.httpPut(requestUrl,headers,body,"12345","sfakjshdkjh1",timeout);
console.log(res);



```

## License

(The MIT License)