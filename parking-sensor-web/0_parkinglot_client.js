var net = require('net');
var readline = require('readline')
console.log("format: w 1(1-5) 0500 (write)   i 1 (request)\n  space0 is sended by 51 mechine");
const rl = readline.createInterface({
    input:process.stdin,
    output:process.stdout
})
rl.on('line',function(line) {
    var tokens = line.split(' ')
    if(tokens[0]=="w"){
        park[parseInt(tokens[1]-1)]=tokens[1]+tokens[2];
    }else if(tokens[0]=="r"){
        console.log(park[parseInt(tokens[1]-1)]);
    }
})
rl.on('close',function(){
    process.exit()
})
const HOST = '192.168.1.101';
const PORT = 8003;
var park=new Array("10501","20501","30501","40501","50501");

var tcpClient = net.Socket();

tcpClient.connect(PORT, HOST, function(){
   console.log('connect success.');
});
setInterval(_send,10000);
function _send(){
    // console.log("1 freshed = "+park1)
    tcpClient.write(park[0]);//服务器向客户端发送消息
    setTimeout(s1,1000);
    setTimeout(s2,2000);
    setTimeout(s3,3000);
    setTimeout(s4,4000);
}
function s1(){
    // console.log("2 freshed = "+park2)
    tcpClient.write(park[1]);
}
function s2(){
    // console.log("3 freshed = "+park3)
    tcpClient.write(park[2]);
}
function s3(){
    // console.log("4 freshed = "+park4)
    tcpClient.write(park[3]);
}
function s4(){
    // console.log("5 freshed = "+park5)
    tcpClient.write(park[4]);
}

//监听服务器端发过来的数据
// tcpClient.on('data', function(data){
//    console.log('received: ', data.toString());
// });