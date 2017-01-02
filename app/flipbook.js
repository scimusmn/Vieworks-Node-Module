
'use strict';

include([], function() {

  //window.flipbook = function() {
  var flipBook = inheritFrom(HTMLCanvasElement, function() {
    this.attachedCallback = function() {
      console.log('new book');
      var _this = this;
      var ctx = _this.getContext('2d');

      _this.frames = [];      //create the array of images which we flip through
      _this.sets ={};
      _this.currentFrame = 0;      //stores the value of the current image being displayed
      _this.loaded = false,
      _this.playing = false,
      _this.loading = false;    //keeps track of whether or not the images are loaded.
      _this.curDir = 'default/';  //storing the name of the directory which we are currently browsing.
      _this.size = 0;

      _this.cache = false;

      var imgPad = 20;

      _this.imageOffset = {x: 0, y: -40};

      _this.onStop = () => {};

      _this.onUpdate = () => {};

      _this.onLoad = () => {};

      _this.onStateChange = ()=>{};

      var defaultSrc = 'assets/pngs/imageFrame.png';
      var notLoadedImg = new Image();
      notLoadedImg.onload = function() {
      };

      notLoadedImg.src = defaultSrc;

      var loadingImg = new Image();

      loadingImg.src = 'assets/pngs/loading.png';

      _this.current = function(aug) {
        if(!aug) aug=0;
        //if (!_this.loaded) return notLoadedImg;
        //else
        let img = _this.set[_this.currentFrame + aug];
        if (!img.loaded || !img.complete || (typeof img.naturalWidth != "undefined" && img.naturalWidth == 0)) return null;
        return img;
      };

      //this is declaring member functions of the book class. The init function is used to load the images
      // from the disk, and inform the rest of the class when they are loaded. It also clears out old info.

      _this.init = function(num) {
        _this.loading = true;
        notLoadedImg.src = 'assets/pngs/imageFrame.png';
        _this.frames = [];
        _this.loaded = false;

        //_this.width = _this.width;      //this clears the html5 canvas, for some reason

        for (let x = 1; x <= num; x++) {
          let imageObj = new Image();                       // new instance for each image
          imageObj.loaded = false;
          _this.frames.push(imageObj);
        }
      };

      _this.makeNewSet = (dir)=>{
        _this.sets[dir] = [];
        for (let x = 1; x <= _this.frames.length; x++) {
          let imageObj = new Image();                       // new instance for each image
          imageObj.loaded = false;
          _this.sets[dir].push(imageObj);
        }
        _this.sets[dir].loaded = false;
        _this.sets[dir].size = 0;
      }

      _this.checkForSet = (dir)=>{
        dir = dir.replace('/','_');
        console.log(dir);
        if(!_this.sets[dir]) _this.makeNewSet(dir);
        else _this.set.loaded = true;
        _this.set = _this.sets[dir];
      }

      _this.loadSet = function(dir) {
        _this.width = _this.clientWidth;
        _this.height = _this.clientHeight;
        if(_this.cached) _this.checkForSet(dir);
        else {
          _this.set = _this.frames;
          _this.set.loaded = false;
          _this.set.size = 0;
        }

        _this.reset();

        _this.set.totalImgs = 0;

        let set = _this.set;
        if(!_this.set.loaded)
        for (let i = 0; i < _this.set.length; i++) {
          let img = _this.set[i];
          img.loaded = false;
          var src = dir + zeroPad(i, 3) + '.jpg';
          if(!_this.cached) src += '?' + Math.random();
          img.src = src;
          img.onload = () => {
            img.loaded = true;
            set.totalImgs++;
            set.size++;
            if (set.totalImgs == set.length) {
              set.loaded = true;
              _this.onLoad();
            }
          };

          img.onerror = (e) => {
            set.totalImgs++;
            e.preventDefault();
            if (set.totalImgs == set.length) {
              set.loaded = true;
              _this.onLoad();
            }
          };
        }
      };

      // _this.unload = function() {
      //   _this.set.loaded = _this.loading = _this.playing = false;
      //   notLoadedImg.src = defaultSrc;
      //   for (var i = 0; i < _this.frames.length; i++) {
      //     _this.frames[i].src = null;
      //   }
      //
      //   _this.frames = null;
      //   _this.frames = [];
      // };

      _this.changeNotLoadedImage = function(title) {
        defaultSrc = title;
        notLoadedImg.src = title;
      };

      function drawRotated(degrees, img) {

        // save the unrotated ctx of the _this so we can restore it later
        // the alternative is to untranslate & unrotate after drawing
        ctx.save();

        // move to the center of the canvas (_this)
        ctx.translate(_this.width / 2, _this.height / 2);

        // rotate the ctx to the specified degrees
        ctx.rotate(degrees * Math.PI / 180);

        // draw the image
        // since the ctx is rotated, the image will be rotated also
        ctx.drawImage(img, -img.width / 2, -img.height / 2);

        // we’re done with the rotating so restore the unrotated ctx
        ctx.restore();
      }

      var degs = 0;

      _this.idle = function() {            //increment the image pointer, if we are playing
        if (_this.playing && _this.currentFrame < _this.set.size - 1 && _this.current(1).loaded) {
          _this.currentFrame++;
          _this.onUpdate();
        } else if (_this.currentFrame >= _this.set.size - 1 && _this.playing) {
          _this.stop();
          _this.onStop();
        }

        ctx.globalAlpha = 1;
        if (_this.current()) ctx.drawImage(_this.current(), _this.imageOffset.x, _this.imageOffset.y, _this.width-_this.imageOffset.x, _this.height-_this.imageOffset.y);
        //else ctx.drawImage(notLoadedImg, 0, 0, _this.width, _this.height);

        if (!_this.set.loaded) {
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
          _this.onStateChange();
      };

      _this.stop = function() {
        _this.playing = false;
        _this.onStateChange();
      };

      _this.togglePlay = function() {
        if (_this.playing) _this.stop();
        else _this.play();
      };

      _this.setFrameNumber = function(val) {
        _this.currentFrame = val;
      };

      _this.setFrameByPercent = function(perc) {
        _this.currentFrame = Math.round(perc * (_this.set.length - 1));
      };

      _this.getPercentDone = function() {
        return _this.currentFrame / _this.set.size;
      };

      _this.reset = function() {
        console.log('resetting ' + _this.curDir);
        _this.currentFrame = 0;
        _this.stop();
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
