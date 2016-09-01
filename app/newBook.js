
'use strict';

include([], function() {

  //window.flipbook = function() {
  var flipBook = inheritFrom(HTMLCanvasElement, function() {
    this.attachedCallback = function() {
      console.log('new book');
      var _this = this;
      var ctx = _this.getContext('2d');

      _this.frames = [];      //create the array of images which we flip through
      _this.currentFrame = 0;      //stores the value of the current image being displayed
      _this.loaded = false,
      _this.playing = false,
      _this.loading = false;    //keeps track of whether or not the images are loaded.
      _this.curDir = 'default/';  //storing the name of the directory which we are currently browsing.
      _this.size = 0;
      _this.numImages = 600;

      var imgPad = 20;

      _this.onStop = () => {};

      _this.onUpdate = () => {};

      _this.onLoad = () => {};

      var defaultSrc = 'assets/pngs/imageFrame.png';
      var notLoadedImg = new Image();
      notLoadedImg.onload = function() {
      };

      notLoadedImg.src = defaultSrc;

      var loadingImg = new Image();

      loadingImg.src = 'assets/pngs/loading.png';

      _this.current = function(aug = 0) {
        //if (!_this.loaded) return notLoadedImg;
        //else
        if (!_this.frames[_this.currentFrame + aug].loaded) return null;
        return _this.frames[_this.currentFrame + aug];
      };

      //this is declaring member functions of the book class. The init function is used to load the images
      // from the disk, and inform the rest of the class when they are loaded. It also clears out old info.

      _this.init = function(num) {
        _this.loading = true;
        notLoadedImg.src = 'assets/pngs/imageFrame.png';
        _this.frames = [];
        _this.loaded = false;

        //_this.width = _this.width;      //this clears the html5 _this, for some reason

        for (let x = 1; x <= num; x++) {
          let imageObj = new Image();                       // new instance for each image
          imageObj.loaded = false;
          _this.frames.push(imageObj);
        }
      };

      _this.loadSet = function(dir) {
        _this.width = _this.clientWidth;
        _this.height = _this.clientHeight;
        _this.reset();
        _this.loaded = false;
        var totalImgs = 0;
        _this.size = 0;
        for (let i = 0; i < _this.frames.length; i++) {
          let img = _this.frames[i];
          img.loaded = false;
          img.src = dir + zeroPad(i, 3) + '.jpg';
          img.onload = () => {
            img.loaded = true;
            totalImgs++;
            _this.size++;
            if (totalImgs == _this.frames.length) {
              _this.loaded = true;
              _this.onLoad();
            }
          };

          img.onerror = (e) => {
            totalImgs++;
            e.preventDefault();
            if (totalImgs == _this.frames.length) {
              _this.loaded = true;
              _this.onLoad();
            }
          };
        }
      };

      _this.unload = function() {
        _this.loaded = _this.loading = _this.playing = false;
        notLoadedImg.src = defaultSrc;
        for (var i = 0; i < _this.frames.length; i++) {
          _this.frames[i].src = null;
        }

        _this.frames = null;
        _this.frames = [];
      };

      _this.changeNotLoadedImage = function(title) {
        defaultSrc = title;
        notLoadedImg.src = title;
      };

      function drawRotated(degrees, img) {

        // save the unrotated ctx of the _this so we can restore it later
        // the alternative is to untranslate & unrotate after drawing
        ctx.save();

        // move to the center of the _this
        ctx.translate(_this.width / 2, _this.height / 2);

        // rotate the _this to the specified degrees
        ctx.rotate(degrees * Math.PI / 180);

        // draw the image
        // since the ctx is rotated, the image will be rotated also
        ctx.drawImage(img, -img.width / 2, -img.height / 2);

        // we’re done with the rotating so restore the unrotated ctx
        ctx.restore();
      }

      var degs = 0;

      _this.idle = function() {            //increment the image pointer, if we are playing
        if (_this.playing && _this.currentFrame < _this.size - 1 && _this.current(1).loaded) {
          _this.currentFrame++;
          _this.onUpdate();
        } else if (_this.currentFrame >= _this.size - 1 && _this.playing) {
          _this.playing = false;
          _this.onStop();
        }

        ctx.globalAlpha = 1;
        if (_this.current()) ctx.drawImage(_this.current(), 0, 0, _this.width, _this.height);

        if (!_this.loaded) {
          ctx.globalAlpha = 0.5;
          ctx.beginPath();
          ctx.rect(0, 0, _this.width, _this.height);
          ctx.fillStyle = 'black';
          ctx.fill();
          ctx.globalAlpha = 1;
          drawRotated(degs, loadingImg);//ctx.drawImage(notLoadedImg, 0, 0, _this.width, _this.height);
          degs = (degs + 10) % 360;
        }
      };

      _this.play = function() {
        _this.playing = true;
      };

      _this.stop = function() {
        _this.playing = false;
      };

      _this.togglePlay = function() {
        if (_this.playing) _this.stop();
        else _this.play();
      };

      _this.setFrameNumber = function(val) {
        _this.currentFrame = val;
      };

      _this.setFrameByPercent = function(perc) {
        _this.currentFrame = Math.round(perc * (_this.frames.length - 1));
      };

      _this.getPercentDone = function() {
        return _this.currentFrame / _this.size;
      };

      _this.reset = function() {
        console.log('resetting ' + _this.curDir);
        _this.currentFrame = 0;
        _this.playing = false;
        _this.loaded = false;
				_this.width = _this.width;
        for (let i = 0; i < _this.frames.length; i++) {
          _this.frames[i].loaded = false;
        }

        _this.onUpdate();
      };

    };
  });

  window.FlipBook = document.registerElement('flip-book', { prototype: flipBook.prototype, extends: 'canvas' });
});
