var vieworks = require('bindings')('vieworks');
var arduino = require('./arduino.js').arduino;
var serial = require('./arduino.js').serial;
var fs = require('fs');

arduino.connect('COM11', function() {
  var i = 0;
  var red = 0;
  var green = 1;
  setInterval(function() {
    arduino.digitalWrite(11, green);
    arduino.digitalWrite(10, red);
    red = !red;
    green = !green;
    //arduino.wireSend(8, [i++]);
    if (i >= 10) i = 0;
  }, 100);
});

var cam = new vieworks.camera(10);

cam.setFrameRate(200);
cam.setImageGain(6);

cam.allocateBuffer(600);

cam.start();


setTimeout(function () {
  cam.capture();
  console.log("two");
}, 2000);

setTimeout(function () {
  cam.stopCapture();
  console.log("three");
  var dir = './temp1';
  if(!fs.existsSync(dir)){
    fs.mkdirSync(dir)
  }
  cam.save(dir,function(){
    //cam.stop();
    console.log("saved to " + dir);
  });
  cam.stop();
  serial.close();
}, 4500);
