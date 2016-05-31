
"use strict";

var vieworks = require('bindings')('vieworks');
var arduino = require('./arduino.js').arduino;
var serial = require('./arduino.js').serial;
var cfg = require('./config.js').config;
var fs = require('fs');

var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({ port: 8080 });
var webSock = null;

var cam = new vieworks.camera(10);

var cState = false;

var greenLight = (state) => {
  arduino.digitalWrite(10, state);
};

var redLight = (state) => {
  arduino.digitalWrite(11, state);
};

var save = (dir) => {
  cam.save(dir, function() {
    //cam.stop();
    console.log('seq=' + 'sequences/temp' + (dirNum-1));
    if (webSock) webSock.send('seq=' + 'sequences/temp' + (dirNum-1));
    console.log('saved to ' + dir);
    cam.ready = true;
  });
}

var cInt = null;

var cArr = [0, 1, 3, 4, 5, 6];
//var cArr = [1, 3, 11, 27, 59, 123];
var cCount = 1;

var countdown = (count) => {
  if (count > 0) {
    greenLight(0);
    redLight(1);
    arduino.wireSend(8, [128 + count]);
    setTimeout(() => { countdown(count - 1); }, 1000);
  } else {
    cInt = setInterval(() => {
      arduino.wireSend(8, [128*(cCount)]);
      cCount = !cCount;
    }, cfg.recordTime/12);
    cam.capture();
    console.log('start capture');

    setTimeout(function() {
      clearInterval(cInt);
      arduino.wireSend(8, [0]);
      cam.stopCapture();
      console.log('done capturing');
      redLight(0);
      greenLight(1);
      var dir = './app/sequences/temp' + dirNum++;
      if (!fs.existsSync(dir)) {
        fs.mkdirSync(dir);
        save(dir);
      } else {
        var exec = require('child_process').exec;
        exec('rm -r'+dir, ()=>{
          //fs.mkdirSync(dir);
          save(dir);
        });
      }



      cState = false;
    }, cfg.recordTime);
  }
};

arduino.connect(cfg.portName, function() {
  //setTimeout(()=>{
  arduino.watchPin(12, function(pin, state) {
    console.log(state);
    if (!cam.isCapturing() && !state && cam.ready) {
      cam.ready = false;
      countdown(9);
    }
    /*if (!cState && !state){
      countdown(9);
      cState = true;
    }*/
  });

  //},3000);

});

cam.setFrameRate(200);
cam.setImageGain(6);

cam.allocateBuffer(600);

cam.start();
cam.ready = true;

var dirNum = 0;

///////////////////////////////////////////////////////////////////////

function readDir(path) {
  var files = fs.readdirSync(path);

  files.sort(function(a, b) {
    return fs.statSync('./' + path + a).mtime.getTime() - fs.statSync('./' + path + b).mtime.getTime();
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
      console.log(files[i]);
      webSock.send('seq=' + files[i].substring(4));
    }

    for (var i = 0; i < celFiles.length; i++) {
      webSock.send('cel=' + celFiles[i].substring(4));
    }
  }
}
