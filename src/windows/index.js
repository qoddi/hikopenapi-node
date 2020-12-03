const os = require("os");
const hikidentify = require(`${genPath()}`);
const hikopenapi = require(`${genPath()}`);

function genPath() {
    const platform = os.platform();
    const arch = process.arch;
    let scriptBasePath = ``;

    if (platform === 'win32') {
        scriptBasePath = "./build/release/hikopenapi.node";
    } else if (platform === 'linux') {
        scriptBasePath = `./lib/linux/${arch}/hikopenapi.node`;
    }
    return scriptBasePath;
}

/*
    加密
 */
const encryption = val => {
    return hikidentify.encryptData(__dirname, val);
};
/*
    获取云存储需要的SK加密串
 */
const exportSK = () => {
    return hikidentify.exportSK(__dirname);
};
/**
 *  say hello,没有参数
 */
const sayHello= ()=>{
    return hikopenapi.sayHello();
}
/**
 *  say hello,没有参数
 */
const saySomething= ()=>{
    return hikopenapi.saySomething(__dirname);
}

const httpPost=(url,headers,body,appKey,appSecret,timeout)=>{
    return hikopenapi.artemisHttpPost(__dirname,url,headers,body,appKey,appSecret,timeout);
}

module.exports = {
    encryption,
    exportSK,
    sayHello,
    saySomething,
};


let json={
    "cameraIndexCode": "65d2e1d5bad64443967811519ed06d37",
    "streamType": 0,
    "protocol": "rtsp",
    "transmode": 0,
    "expand": "transcode=0"
};

let res=httpPost('https://44.136.134.5:443/artemis-web/api/video/v1/cameras/previewURLs',{"Content-Type":"application/json","test":"test1"},JSON.stringify(json),'25747611','zt4sP15zdqKjtBYVypkl',15);
console.log(res);