
// 初始化节点
const rosnodejs = require('rosnodejs');
rosnodejs.initNode('/my_node')
.then(() => {
  // do stuff
});

//Subscribe
const nh = rosnodejs.nh;
const sub = nh.subscribe('/chatter', 'std_msgs/String', (msg) => {
  console.log('Got msg on chatter: %j', msg);   //接收的地方
});
//Publish
const pub = nh.advertise('/chatter', 'std_msgs/String');
pub.publish({ data: "hi" }); //要修改的地方




var net = require('net');

const HOST = '192.168.1.101';
const PORT = 8003;

var tcpClient = net.Socket();

tcpClient.connect(PORT, HOST, function(){
   console.log('connect success.');

   tcpClient.write('r');//服务器向客户端发送消息
});

//监听服务器端发过来的数据
tcpClient.on('data', function(data){
    if(data.toString()=='-1'){

    }else{
        let n=parseInt(data.toString());
    }
});