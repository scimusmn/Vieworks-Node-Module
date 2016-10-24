
'use strict';

window.arduino = require('./arduino.js').arduino;
var serial = require('./arduino.js').serial;
var cfg = require('./config.js').config;


var cState = false;

var greenLight = (state) => {
  arduino.digitalWrite(11, state);
};

var redLight = (state) => {
  arduino.digitalWrite(10, state);
};

var pollLight = new function(){
  var cInt = null;

  //64=red, 2 = y1, 1 = y2, 16=y3 32=g
  //var cArr = [64,2,1,16,32,115];
  //var cArr = [64,66,67,83,115,115];
  var cArr = [115,83,67,66,64,64];
  //var cArr = [1, 3, 11, 27, 59, 123];
  var cCount = 0;

  this.setStage = function(count){
    //arduino.wireSend(8,[0]);
    //setInterval(()=>{
      arduino.wireSend(8,[cArr[count]]);
      //if(++cCount>=6) cCount = 0;
    //},500);
  }

  this.blink = function () {
    cInt = setInterval(()=>{
      arduino.wireSend(8,[115*cCount]);
      cCount=!cCount;
    },500);
  }

  this.stopBlink = function () {
    clearInterval(cInt);
    arduino.wireSend(8,[cArr[5]]);
  }
}




var countdown = (count) => {
  if (count > 0) {
    //greenLight(0);
    //redLight(1);
    //arduino.wireSend(8, [cArr[count-1]]);
    pollLight.setStage(count);
    setTimeout(() => { countdown(count - 1); }, 1000);
  } else {
    pollLight.blink();
    console.log('start capture');

    setTimeout(function() {
      pollLight.stopBlink();
      arduino.wireSend(8, [0]);
      console.log('done capturing');
      cState = false;
    }, cfg.recordTime);
  }
};

arduino.connect(cfg.portName, function() {
  //setTimeout(()=>{
  //pollLight.setStage();

  arduino.watchPin(12, function(pin, state) {
    console.log(state + " is the current state");
    if (!state) {
      countdown(5);
      greenLight(0);
      redLight(0);
      arduino.digitalWrite(3, 0);
      arduino.digitalWrite(5, 1);
    }
  });

  arduino.watchPin(8, function(pin, state) {
    console.log(state + " is the current state");
    if (state) {
      setTimeout(()=>{
        greenLight(0);
        redLight(1);
        arduino.digitalWrite(3, 0);
        arduino.digitalWrite(5, 1);
      },1000);
    }
  });

  arduino.watchPin(7, function(pin, state) {
    console.log(state + " is the current state on "+ pin);
    if (state) {
      setTimeout(()=>{
        greenLight(0);
        redLight(1);
        arduino.digitalWrite(3, 1);
        arduino.digitalWrite(5, 0);
      },1000);
    }
  });

  console.log('arduino start')

  //arduino.wireSend(8, [4]);
  greenLight(0);
  redLight(1);
  arduino.digitalWrite(3, 1);
  arduino.digitalWrite(5, 0);
  //},3000);

});

/*var three = µ('#three');
var two = µ('#two');
var one = µ('#one');
var go = µ('#go');*/

/*let idleTO = null;

let resetIdleTimeout = () => {
  if(idleTO) clearTimeout(idleTO);
  idleTO = setTimeout(()=>{
    //set the idle loop video
  },120000);
}

let audio = [];

for (var i = 0; i < 4; i++) {
  audio.push(document.querySelector('#audio_' + (i + 1)));
  audio[i].load();
}

let nextAudio = function(num) {
  if (num < 4) {
    audio[num].play();
    console.log(num);
    setTimeout(()=> {nextAudio(num + 1);}, 1000);
  }
};

document.onkeypress = (ev) => {
  ev.preventDefault();
  var press = String.fromCharCode(ev.keyCode);
  if (press == ' ') {
    nextAudio(0);
  }
};

/*'use strict';

var express = require('express');
var app = express();

app.use(express.static('app'));

app.listen(4030, function() {
  console.log('listening on 80');
});

var cfg = require('./config.js').config;
var fs = require('fs');

var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({ port: 8080 });
var webSock = null;

function readDir(path) {
  var files = fs.readdirSync(path);
  var ret = [];

  files.sort(function(b, a) {
    return fs.statSync('./' + path + b).atime.getTime() - fs.statSync('./' + path + a).atime.getTime();
  });

  for (var i = 0; i < files.length; i++) {
    if (files[i].charAt(0) != '.') ret.push(path + files[i]);
  }

  return ret;
}

var fileTimes = [];

function watchDir(path) {
  var files = fs.readdirSync(path);
  var ret = null;
  for (var i = 0; i < files.length; i++) {
    var time = fs.statSync('./' + path + files[i]).atime.getTime();
    if (time != fileTimes[i]) {
      ret = files[i];
      fileTimes[i] = time;
    }
  }

  return ret;
}

setInterval(function() {
  var newDir = watchDir('app/sequences/');
  if (newDir) {
    console.log(newDir);
    if (webSock) webSock.send('seq=' + 'sequences/' + newDir);
  }
}, 500);

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
}*/
