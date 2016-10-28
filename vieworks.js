
"use strict";

var vieworks = require('bindings')('vieworks');
window.arduino = require('./arduino.js').arduino;
var serial = require('./arduino.js').serial;
var cfg = require('./config.js').config;
var fs = require('fs');

var express = require('express');
var app = express();

app.use(express.static('app'));

app.listen(80, function(){
  console.log('listening on 80');
});

var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({ port: 8080 });
var webSock = null;

var cam = new vieworks.camera(function(){
  cam.setFrameRate(200);
  cam.setImageGain(3.99);

  cam.allocateBuffer(1000);

  var pre = document.createElement('canvas');
  var can = document.querySelector('#display');

  cam.start(function(val){
    /*cam.ready = true;
    var ctx = can.getContext('2d');
    var ptx = pre.getContext('2d');

    var w = Math.ceil(cam.getWidth());
    var h = Math.ceil(cam.getHeight());
    can.width = h;
    can.height = w;

    pre.width = w;
    pre.height = h;

    setInterval(()=>{
      if(!cam.isCapturing()){
        var t = cam.getImage();
        if(t&&t.length>=w*h*3){
          var im = ptx.createImageData(w,h);
          var con = new Uint8ClampedArray(w*h*4);
          for(let i=0,j=0; i< t.length; i+=4,j+=3){
            //var p = palette(t[i]);
            con[i] = t[j+2]*1.1;
            con[i+1] = t[j+1];
            con[i+2] = t[j];
            con[i+3] = 255;
          }
          im.data.set(con);

          //note: to mirror this, draw to intermediate canvas, and then drawImage
          // the intermediate into the display canvas

          ptx.putImageData(im,0,0);

          ctx.save();
          ctx.translate(200,240);
          ctx.rotate(Math.PI/2);
          //ctx.fillStyle = '#777';
          //ctx.fillRect(-320,-240,50,100);
          ctx.drawImage(pre,-320,-240);
          ctx.restore();
        }
      }
    },50);*/

});

var cState = false;

let beep = document.querySelector('#beep');
beep.load();

let longBeep = document.querySelector('#longBeep');
longBeep.load();

let clickTrack = document.querySelector('#click');
clickTrack.load();

let audio = [];

for (var i = 0; i < 4; i++) {
  audio.push(document.querySelector('#audio_' + (i)));
  audio[i].load();
}


let idleTO = null;

let resetIdleTimeout = () => {
  console.log()
  if (idleTO) clearTimeout(idleTO);
  idleTO = setTimeout(()=> {
    alternateVideo();
  }, 60000);
};

resetIdleTimeout();

var loopPractice = () => {
  arduino.digitalWrite(9, 0);
  console.log('Loop practice');
  setTimeout(() => {
    arduino.digitalWrite(9, 1);
  }, 100);
};

var showGo = () => {
  arduino.digitalWrite(4, 0);
  console.log('Show go');
  setTimeout(() => {
    arduino.digitalWrite(4, 1);
    setTimeout(() => {
      loopPractice();
    }, 10000);
  }, 100);
};

var alternateVideo = () => {
  arduino.digitalWrite(6, 0);
  console.log("alternate");
  setTimeout(() => {
    arduino.digitalWrite(6, 1);
  }, 100);
};

var greenExitLight = (state) => {
  arduino.digitalWrite(11, state);
};

var redExitLight = (state) => {
  arduino.digitalWrite(10, state);
};

var greenEntranceLight = (state) => {
  arduino.digitalWrite(3, state);
  if (state) showGo();
};

var redEntranceLight = (state) => {
  arduino.digitalWrite(5, state);
  if (state) loopPractice();
};

var justRecorded = false;

var save = (dir) => {
  cam.save(dir, function() {
    //cam.stop();
    console.log('seq=' + 'sequences/temp' + (dirNum - 1));
    //var num = fs.readdirSync('sequences/temp' + (dirNum - 1)).length;
    if (webSock) webSock.send('seq=' + 'sequences\\temp' + (dirNum - 1));
    console.log('saved to ' + dir);
    cam.ready = true;
  });
};

var pollLight = new function(){
  var cInt = null;

  //64=red, 2 = y1, 1 = y2, 16=y3 32=g
  //var cArr = [64,2,1,16,32,115];
  //var cArr = [64,66,67,83,115,115];
  //var cArr = [115,83,67,66,64,64];
  var cArr = [51,19,3,2,0,0];
  //var cArr = [1, 3, 11, 27, 59, 123];
  var cCount = 0;

  this.setGreen = function(){
    arduino.wireSend(8,[32]);
  }

  this.setRed = function(){
    arduino.wireSend(8,[64]);
  }

  this.setStage = function(count){
    //arduino.wireSend(8,[0]);
    //setInterval(()=>{
      arduino.wireSend(8,[cArr[count]]);
      //if(++cCount>=6) cCount = 0;
    //},500);
  }

  this.blink = function () {
    cCount = 1;
    cInt = setInterval(()=>{
      arduino.wireSend(8,[51*cCount]);
      cCount=!cCount;
    },250);
  }

  this.stopBlink = function () {
    clearInterval(cInt);
  }
}

var deleteFolderRecursive = function(path) {
  if(path && fs.existsSync(path)) {
    fs.readdirSync(path).forEach(function(file,index){
      var curPath = path + "/" + file;
      if(fs.lstatSync(curPath).isDirectory()) { // recurse
        deleteFolderRecursive(curPath);
      } else { // delete file
        fs.unlinkSync(curPath);
      }
    });
    fs.rmdirSync(path);
  }
};

var countdown = (count) => {
  pollLight.setStage(count);
  if (count > 0) {
    /*if(audio[i+1]&&!audio[i+1].paused){
      beep.pause();
      beep.currentTime = 0;
    }*/
    if(count<4) audio[count].play();
    setTimeout(() => { countdown(count - 1); }, 1000);
    if(count == 1 ) cam.capture();
  } else {
    //longBeep.play();
    audio[count].play();
    clickTrack.play();
    //audio[count].onended = function () {
      //longBeep.play();
    //}
    pollLight.blink();
    //longBeep.play();
    //cam.capture();
    console.log('start capture');

    setTimeout(function() {
      //beep.play();
      pollLight.stopBlink();
      pollLight.setRed();
      cam.stopCapture();
      console.log('done capturing');
      var dir = './app/sequences/temp' + dirNum++;
      if(dirNum>=21) dirNum = 0;
      greenExitLight(1);
      redExitLight(0);
      if (fs.existsSync(dir)) deleteFolderRecursive(dir);
      fs.mkdirSync(dir);
      save(dir);
      fs.utimesSync(dir,NaN,NaN);


      cState = false;
    }, cfg.recordTime);
  }
};

window.resetCam = function () {
  justRecorded = false;
}

window.startCntdn = function(pin, state) {
  console.log(state + " is the current state");
  if(!cam.isCapturing() && cam.ready)
    if ( !state && !justRecorded) {
      resetIdleTimeout();
      justRecorded = true;
      cam.ready = false;
      countdown(5);
      greenExitLight(0);
      redExitLight(0);
      greenEntranceLight( 0);
      redEntranceLight( 1);
    }

  /*if (!cState && !state){
    countdown(9);
    cState = true;
  }*/
};

arduino.connect(cfg.portName, function() {
  console.log('Connecting to Arduino');
  pollLight.setStage(4);

  arduino.watchPin(12, window.startCntdn);

  arduino.watchPin(8, function(pin, state) {
    console.log(state + " is the current state");
    if (state) {
      setTimeout(()=>{
        greenExitLight(0);
        redExitLight(1);
        greenEntranceLight( 0);
        redEntranceLight( 1);
      },1000);
    }
  });

  arduino.watchPin(7, function(pin, state) {
    console.log(state + " is the current state on "+ pin);
    if (state) {
      setTimeout(()=>{
        justRecorded = false;
        greenExitLight(0);
        redExitLight(1);
        greenEntranceLight( 1);
        redEntranceLight( 0);
        pollLight.setStage(4)
      },1000);
    }
  });

  console.log('arduino start')

  arduino.digitalWrite(9,1);
  arduino.digitalWrite(6,1);
  arduino.digitalWrite(4,1);

  greenExitLight(0);
  redExitLight(1);
  greenEntranceLight( 1);
  redEntranceLight( 0);

});


cam.setFrameRate(200);
cam.setImageGain(3.99);

cam.allocateBuffer(1000);

var pre = document.createElement('canvas');
var can = document.querySelector('#display');

setInterval(()=>{
  cam.idle();
},50);

cam.start(function(val){
  cam.ready = true;
  var ctx = can.getContext('2d');
  var ptx = pre.getContext('2d');

  var w = Math.ceil(cam.getWidth());
  var h = Math.ceil(cam.getHeight());
  can.width = h;
  can.height = w;

  pre.width = w;
  pre.height = h;

  setInterval(()=>{
    if(!cam.isCapturing()){
      var t = cam.getImage();
      if(t&&t.length>=w*h*3){
        var im = ptx.createImageData(w,h);
        var con = new Uint8ClampedArray(w*h*4);
        for(let i=0,j=0; i< t.length; i+=4,j+=3){
          //var p = palette(t[i]);
          con[i] = t[j+2]*1.1;
          con[i+1] = t[j+1];
          con[i+2] = t[j];
          con[i+3] = 255;
        }
        im.data.set(con);

        //note: to mirror this, draw to intermediate canvas, and then drawImage
        // the intermediate into the display canvas

        ptx.putImageData(im,0,0);

        ctx.save();
        ctx.translate(200,240);
        ctx.rotate(Math.PI/2);
        //ctx.fillStyle = '#777';
        //ctx.fillRect(-320,-240,50,100);
        ctx.drawImage(pre,-320,-240);
        ctx.restore();
      }
    }
  },50);
});


var dirNum = 0;

///////////////////////////////////////////////////////////////////////

function readDir(path) {
  var files = fs.readdirSync(path);

  files.sort(function(a, b) {
    return fs.statSync('./' + path + a).atime.getTime() - fs.statSync('./' + path + b).atime.getTime();
  });

  for (var i = 0; i < files.length; i++) {
    files[i] = path + files[i];
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
  });

  ws.on('close', function() {
    webSock = null;
  });

  ws.on('error', function(error) {
    webSock = null;
    console.log('Error: ' + error);
  });
});

function onOpen() {
  var files = readDir('app/sequences/');
  var celFiles = readDir('app/celeb_seq/');
  if (webSock) {
    for (var i = 0; i < files.length; i++) {
      //var num = fs.readdirSync(files[i]).length;
      webSock.send('seq=' + files[i].substring(4));
    }

    for (var i = 0; i < celFiles.length; i++) {
      //var num = fs.readdirSync(celFiles[i]).length;
      webSock.send('cel=' + celFiles[i].substring(4));
    }
  }
}

document.onkeypress = (e) => {
  var press = String.fromCharCode(e.keyCode);
  if(press == 'g') {
    showGo();
  } else if(press == 'c') startCntdn();
  else if(press == 'r') justRecorded = false;
}
