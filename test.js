const Hikopenapi=require('./index.js');


let res=Hikopenapi.httpGet('http://www.baidu.com',{"content-type":"application/json"},"123123","dasaiauoaf",15);
console.log(res);