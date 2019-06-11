var net = require('net');
var ws = require('nodejs-websocket');
var x,xx=null;
var server = net.createServer(function (connection) {
	x = connection;
	console.log('client connected');
	connection.on("data", function (data) {
		if(xx!=null){
			xx.sendText(data)
		}else{
			console.log("ws server is not online.")
		}
		console.log("status:" + data);
	});
});
server.listen({
	port: 8003,
	host: "192.168.1.101",
	exclusive: true,//独占的
}, function () {
	console.log('server is listening');
});
var PORT = 8002;
var wsserver = ws.createServer(function (conn) {
	xx=conn;
	conn.on("text", function (str) {
		x.write(str);
	});
	conn.on("close", function (code, reason) {
		console.log("ws connection closed");
	});
	conn.on("error", function (err) {
		console.log(err)
	});
}).listen(PORT);
console.log('websocket server listening on port:' + PORT);
