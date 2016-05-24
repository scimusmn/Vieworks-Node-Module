var vieworks = require('bindings')('vieworks');
var arduino = require('./arduino.js').arduino;
var serial = require('./arduino.js').serial;
var fs = require('fs');

var WebSocketServer = require('ws').Server, wss = new WebSocketServer({port: 8080});
var webSock = null;

var cam = new vieworks.camera(10);

var greenLight = (state) => {
  arduino.digitalWrite(11, state);
}

var redLight = (state) => {
  arduino.digitalWrite(10, state);
}

var cInt = null;

var cArr = [0,1,3,4,5,6];
var cCount = 0;

var countdown = (count) => {
  if (count>0) {
    greenLight(0);
    redLight(1);
    arduino.wireSend(8, [128+count]);
    setTimeout(()=>{countdown(count-1)},1000);
  } else {
    cInt = setInterval(()=>{
      arduino.wireSend(8, [1<<cArr[cCount++]]);
      if(cCount>=6) cCount=0;
    },100);
    cam.capture();
    console.log("start capture");

    setTimeout(function () {
      clearInterval(cInt);
      arduino.wireSend(8, [0]);
      cam.stopCapture();
      console.log("done capturing");
      redLight(0);
      greenLight(1);
      var dir = './app/sequences/temp'+dirNum++;
      if(!fs.existsSync(dir)){
        fs.mkdirSync(dir)
      }
      cam.save(dir,function(){
        //cam.stop();
        if(webSock) webSock.send("seq="+"sequences/"+dir);
        console.log("saved to " + dir);
      });
      //cam.stop();
      //serial.close();
    }, 3000);
  }
}

arduino.connect('COM11', function() {
  var i = 0;
  var red = 0;
  var green = 1;
  setTimeout(()=>{
      arduino.watchPin(12,function (pin,state) {
      console.log(state);
      if(!cam.isCapturing()&&!state){
        countdown(9);
      }
    });
  },3000);

});

cam.setFrameRate(200);
cam.setImageGain(6);

cam.allocateBuffer(600);

cam.start();

var dirNum = 0;

///////////////////////////////////////////////////////////////////////

function readDir(path){
	var files = fs.readdirSync(path);

	files.sort(function(a,b){
		return fs.statSync("./"+path+a).mtime.getTime()-fs.statSync("./"+path+b).mtime.getTime();
	});
	for(var i=0; i<files.length; i++){
		files[i]=path+files[i];
	}

	return files;
}

var changedFile;


//Tell the wsServer what to do on connnection to a client;

wss.on('connection', function(ws) {

	webSock = ws;

	onOpen();

    ws.on('message', function(message) {
        console.log('received: %s', message);
		//ws.send('something');
    });

	ws.on('close',function(){
		webSock=null;
	});

	ws.on('error',function(error){
		webSock=null;
		console.log("Error: "+error);
	});
});

function onOpen(){
	var files = readDir("app/sequences/");
	var celFiles = readDir("app/celeb_seq/");
	if(webSock){
		for(var i=0; i<files.length; i++){
      console.log(files[i]);
			webSock.send("seq=../"+files[i]);
		}
		for(var i=0; i<celFiles.length; i++){
			webSock.send("cel=../"+celFiles[i]);
		}
	}
}
