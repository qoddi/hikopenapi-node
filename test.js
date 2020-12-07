const Hikopenapi=require('./index.js');


let res=Hikopenapi.httpPost('http://www.baidu.com',{"content-type":"application/json"},"ello","123123","dasaiauoaf",15);
let resParse=JSON.parse(res);
let decode=Buffer.from(resParse.data,'base64').toString();
console.log('----------------------------------------------------------------------------------------------');
console.log(decode);