"use strict";

var numToCopy = 800;

var srcFolder = process.argv[2];
var destFolder = process.argv[3];
var startNum = process.argv[4];

var fs = require('fs-extra')

function zeroPad(num, size) {
    var s = num+"";
    while (s.length < size) s = "0" + s;
    return s;
}

fs.ensureDirSync(destFolder);

for (var i = 0; i < numToCopy; i++) {
  var fileName = zeroPad(i+parseInt(startNum),3)+".jpg";
  var newName = zeroPad(i,3)+".jpg";

  fs.copy(srcFolder+fileName, destFolder+newName, function (err) {
    if (err) return console.error(err)
    console.log("copied " + destFolder+newName);
  }) // copies file
}

fs.copy(srcFolder+'thumb.jpg', destFolder+'thumb.jpg', function (err) {
  if (err) return console.error(err)
  console.log("copied " + destFolder+'thumb.jpg');
}) // copies file
