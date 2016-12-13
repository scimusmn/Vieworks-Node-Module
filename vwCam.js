'use strict';

var vieworks = require('bindings')('vieworks');
var cfg = require('./config.js').config;

var cam = new vieworks.camera(function(){
  cam.setFrameRate(cfg.frameRate);
  cam.setImageGain(cfg.imageGain);

  cam.allocateBuffer(cfg.recordTime*2);


  cam.onStart = ()=>{

  };

  cam.start = function() {
    cam.startCapture(cam.onStart);
  };

  cam.onSave = ()=>{};

  cam.saveImages = function(dir) {
    cam.isSaving = true;
    cam.save(dir,(num)=>{
      console.log(num + ' saved');
      cam.onSave(dir);
      cam.isSaving = false;
    });
  }

  cam.prepare = function() {
    cam.isWaiting = true;
  }

  cam.record = function() {
    console.log('start record')
    cam.capture();
    cam.isWaiting = false;
  }

  cam.stopRecord = function() {
    console.log('stop record');
    cam.stopCapture();
  }

  cam.onReady();

  /*cam.startCapture(function(val){
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
        cam.getImage(function(t){
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
            ptx.putImageData(im,0,0);

            ctx.save();
            ctx.translate(200,240);
            ctx.rotate(Math.PI/2);
            ctx.drawImage(pre,-320,-240);
            ctx.restore();
          }
        });
      }
    },50);
  });*/
});

cam.onReady = ()=>{
  console.log('Camera is ready.');
}

cam.isReady = function() {
  //return !(cam.isCapturing() || cam.isSaving || cam.isWaiting);
  console.log(cam.isWaiting + ' ' + cam.isSaving);
  if(cam.isWaiting) return false;
  else if(cam.isSaving || cam.isCapturing()) return false;
  else return true;
}

exports.cam = cam;
