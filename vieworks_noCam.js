
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

wss.broadcast = function(data){
  wss.clients.forEach(function each(client) {
    client.send(data);
  });
}

////////////////////////////////////////////////////////////////////////
// ########################### Declarations ############################
////////////////////////////////////////////////////////////////////////

var timeoutFlag = true;
var blinkInt = null;
var blinkBool = 1;

var redInt = null;

let idleTO = null;
var dirNum = 0;

var goShown = false;
var audioPracticePlaying = false;

var goTimeout = null;

let resetIdleTimeout = () => {
  timeoutFlag = false;
  if (idleTO) clearTimeout(idleTO);
  idleTO = setTimeout(()=> {
    timeoutFlag = true;
    redEntranceLight(1);
    greenEntranceLight(0);
  }, 60000);
};

var output = document.querySelector('#output');

var cam = new vieworks.camera(function(){
  cam.setFrameRate(200);
  cam.setImageGain(6);

  cam.allocateBuffer(1600);

  var pre = document.querySelector('#predraw');
  var can = document.querySelector('#display');

  cam.startCapture(function(val){
    cam.ready = true;
    var ctx = can.getContext('2d');
    var ptx = pre.getContext('2d');

    var w = Math.ceil(cam.getWidth());
    var h = Math.ceil(cam.getHeight());
    console.log(w + ' is w and h is ' + h);
    can.width = h;
    can.height = w;

    pre.width = w;
    pre.height = h;

    setInterval(()=>{
      if(!cam.isCapturing()){
        var t = cam.getImage(function(t){
          if(t&&t.length>=w*h*3){
            var im = ptx.createImageData(w,h);
            var con = new Uint8ClampedArray(w*h*4);
            for(let i=0,j=0; j< t.length; i+=4,j+=3){
              //var p = palette(t[i]);
              con[i] = t[j+2];
              con[i+1] = t[j+1];
              con[i+2] = t[j];
              con[i+3] = 255;
            }
            im.data.set(con);
            ptx.fillStyle = 'black';
            ptx.putImageData(im,0, 0);

            ctx.save();
            ctx.translate(can.width/2,can.height/2);
            ctx.rotate(Math.PI/2);
            ctx.drawImage(pre,-pre.width/2,-pre.height/2);
            //ctx.drawImage(pre,-320,-240);
            ctx.restore();
          }
        });
      }
    },50);
  });

  output.textContent = 'Ready to record';
});

let beep = document.querySelector('#beep');
beep.load();

let longBeep = document.querySelector('#longBeep');
longBeep.load();

let clickTrack = document.querySelector('#click');
clickTrack.load();

let getReady = document.querySelector('#getReady');
getReady.load();

getReady.volume = .25;

let exitTrack = document.querySelector('#exit');
exitTrack.load();

exitTrack.volume = .75;

let audio = [];

for (var i = 0; i < 4; i++) {
  audio.push(document.querySelector('#audio_' + (i)));
  audio[i].load();
}

window.loopPractice = () => {
  cam.ready=false;
  arduino.digitalWrite(13, 0);
  console.log('Loop practice');
  setTimeout(() => {
    arduino.digitalWrite(13, 1);
  }, 100);
};

window.showGo = () => {
  arduino.digitalWrite(12, 0);
  console.log('Show go');
  goShown = true;
  setTimeout(() => {
    arduino.digitalWrite(12, 1);
    setTimeout(() => {
      goShown = false;
      //loopPractice();
    }, 16000);
  }, 100);
};

window.showPracticeAudio = (fxn) => {
  arduino.digitalWrite(11, 0);
  console.log("practice audio");
  audioPracticePlaying = true;
  resetIdleTimeout();
  setTimeout(() => {
    arduino.digitalWrite(11, 1);
    goTimeout = setTimeout(() => {
      audioPracticePlaying = false;
      showGo();
      if(fxn) fxn();
    }, 16000);
  }, 100);
};

//resetIdleTimeout();
//alternateVideo();

var greenExitLight = (state) => {
  arduino.digitalWrite(3, state);
};

var redExitLight = (state) => {
  arduino.digitalWrite(4, state);
};

var greenEntranceLight = (state) => {
  arduino.digitalWrite(5, state);
  //if (state) showGo();
};

var redEntranceLight = (state) => {
  arduino.digitalWrite(6, state);
  if (state) loopPractice();
};

var waitForSave = false;
var cageOccupied = false;


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

window.save = (dir,saveOther) => {
  if (fs.existsSync(dir)) deleteFolderRecursive(dir);
  fs.mkdirSync(dir);
  output.textContent = 'Saving...';
  cam.save(dir, function() {
    //cam.stop();
    output.textContent = 'Done Saving.';
    fs.utimesSync(dir,NaN,NaN);
    console.log('seq=' + dir);
    //var num = fs.readdirSync('sequences/temp' + (dirNum - 1)).length;
    if (wss&&!saveOther) wss.broadcast('seq=' + 'sequences\\temp' + (dirNum - 1));
    console.log('saved to ' + dir);
    cam.ready = true;
    waitForSave = false;
  });
};

var startBut = document.querySelector('#start');
var saveBut = document.querySelector('#save');

saveBut.onclick = (e)=>{
  //console.log(document.querySelector('#folder').value);
  save(document.querySelector('#folder').value,true);
}


var pollLight = new function(){
  var cInt = null;

  //64=red, 2 = y1, 1 = y2, 16=y3 32=g
  //var cArr = [64,2,1,16,32,115];
  //var cArr = [64,66,67,83,115,115];
  //var cArr = [115,83,67,66,64,64];
  //pin 7 = red, 8 =gr 9 =yellow3 10 = y2, 14 = y1
  var pole = [7,8,9,10,15];
  var cArr = [[0,1,1,1,1],
              [0,0,1,1,1],
              [0,0,0,1,1],
              [0,0,0,0,1],
              [0,0,0,0,0],
              [0,0,0,0,0]
            ];

  console.log(cArr[0])
  //var cArr = [1, 3, 11, 27, 59, 123];
  var cCount = 0;

  this.setGreen = function(){
    //arduino.wireSend(8,[32]);
    for(let i=0; i<5; i++){
      arduino.digitalWrite(pole[i], 0);
    }
    arduino.digitalWrite(8, 1);
  }

  this.setRed = function(){
    //arduino.wireSend(8,[64]);
    for(let i=0; i<5; i++){
      arduino.digitalWrite(pole[i],0);
    }
    arduino.digitalWrite(7, 1);
  }

  this.setStage = function(count){
    //arduino.wireSend(8,[0]);
    //setInterval(()=>{
       //arduino.wireSend(8,[cArr[count]]);
      //if(++cCount>=6) cCount = 0;
    //},500);
    for(let i=0; i<5; i++){
      arduino.digitalWrite(pole[i], cArr[count][i]);
    }
  }

  this.blink = function () {
    cCount = 1;
    cInt = setInterval(()=>{
      for(let i=1; i<5; i++){
        arduino.digitalWrite(pole[i], ((cCount)?1:0));
      }
      cCount=!cCount;
    },250);
  }

  this.stopBlink = function () {
    clearInterval(cInt);
  }
}

var countdown = (count) => {
  pollLight.setStage(count);

  if (count > 0) {
    output.textContent = count;
    // if(audio[i+1]&&!audio[i+1].paused){
    //   beep.pause();
    //   beep.currentTime = 0;
    // }
    if(count<4) audio[count].play();
    setTimeout(() => { countdown(count - 1); }, 1000);
    if(count == 1 ) cam.capture();
    else if(count == 5) getReady.play();
  } else {
    output.textContent = 'Recording...';
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
      exitTrack.play();
      output.textContent = 'Done Recording';
      cam.stopCapture();
      pollLight.stopBlink();
      pollLight.setRed();
      console.log('done capturing');
      var dir = './app/sequences/temp' + dirNum++;
      if(dirNum>=21) dirNum = 0;
      greenExitLight(1);
      blinkInt = setInterval(()=>{
        blinkBool = !blinkBool;
        greenExitLight((blinkBool)?1:0);
      },500)
      clearInterval(redInt);
      redExitLight(0);
      save(dir);
    }, cfg.recordTime);
  }
};

window.resetCam = function () {
  waitForSave = false;
}

window.startCntdn = function(pin, state) {
  console.log(state + " is the current state");
  if ( !state && !waitForSave && !cageOccupied) {
    //timeoutFlag = false;
    resetIdleTimeout();
    clearTimeout(goTimeout);
    audioPracticePlaying = false;
    clearInterval(blinkInt);
    waitForSave = true;
    cageOccupied = true;
    countdown(5);
    greenExitLight(0);
    clearInterval(redInt);
    redExitLight(0);
    greenEntranceLight( 0);
    redEntranceLight( 1);
  }

};

startBut.onclick = ()=>{
  cageOccupied = false;
  window.startCntdn();
}

window.admitNext = ()=>{
  showGo();
  resetIdleTimeout();
  greenExitLight(0);
  clearInterval(redInt);
  redInt = setInterval(()=>{
    blinkBool = !blinkBool;
    redExitLight((blinkBool)?1:0);
  },500);
  clearInterval(blinkInt);
  greenEntranceLight( 1);
  redEntranceLight( 0);
}


/////////////////////////////////////////////////////////////////////////////
//####################### Arduino Declarartions #############################
/////////////////////////////////////////////////////////////////////////////

arduino.connect(cfg.portName, function() {
  console.log('Connecting to Arduino');
  //pollLight.setStage(4);

  arduino.watchPin(2, window.startCntdn);

  arduino.watchPin(14, function(pin, state) {
    console.log(state + " is the current state on "+ pin);
    if (state) {
      setTimeout(()=>{
        cageOccupied = false;
        greenExitLight(0);
        clearInterval(redInt);
        redInt = setInterval(()=>{
          blinkBool = !blinkBool;
          redExitLight((blinkBool)?1:0);
        },500);
        clearInterval(blinkInt);
      },1000);
    }
  });

  arduino.watchPin(16, function(pin, state) {
    if (!state) {
      console.log('practice cage occupied');
      if(!audioPracticePlaying){
        if(timeoutFlag){
          console.log('show practive with audio');
          showPracticeAudio(admitNext);
        } else if(!cageOccupied && !goShown){
          admitNext();
        }
      }


      /*setTimeout(()=>{
        cageOccupied = false;
        greenExitLight(0);
        //redExitLight(1);
        clearInterval(redInt);
        redInt = setInterval(()=>{
          blinkBool = !blinkBool;
          redExitLight((blinkBool)?1:0);
        },500);
        clearInterval(blinkInt);
        showGo();
        resetIdleTimeout();
        greenEntranceLight( 1);
        redEntranceLight( 0);
      },15000);*/
    }
  });

  console.log('arduino start')

  greenExitLight(0);
  //redExitLight(1);
  clearInterval(redInt);
  redInt = setInterval(()=>{
    blinkBool = !blinkBool;
    redExitLight((blinkBool)?1:0);
  },500);
  greenEntranceLight( 1);
  redEntranceLight( 0);

});

/////////////////////////////////////////////////////////////////////////////
//############################ File Handling ################################
/////////////////////////////////////////////////////////////////////////////

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

  var files = readDir('app/sequences/');
  var celFiles = readDir('app/celeb_seq/');
  if (ws) {
    for (var i = 0; i < files.length; i++) {
      //var num = fs.readdirSync(files[i]).length;
      ws.send('seq=' + files[i].substring(4));
    }

    for (var i = 0; i < celFiles.length; i++) {
      //var num = fs.readdirSync(celFiles[i]).length;
      ws.send('cel=' + celFiles[i].substring(4));
    }
  }

  ws.on('message', function(message) {
    //console.log('received: %s', message);
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

}

document.onkeypress = (e) => {
  var press = String.fromCharCode(e.keyCode);
  if(press == 'g') {
    showGo();
  } else if(press == 'c') startCntdn();
  else if(press == 'r') waitForSave = false;
}
