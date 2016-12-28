'use strict';

include(['flipbook.js', 'slider.js', 'toggleButton.js'], function() {
  var playerTag = inheritFrom(HTMLElement, function() {
    this.createdCallback = function() {
      var _this = this;

      var idleTimer = null;

      _this.player = new FlipBook();
      _this.appendChild(_this.player);

      _this.player.init(800);

      _this.button = new ToggleButton();
      _this.button.setAttribute('active', 'assets/pngs/playButton.png');
      _this.button.setAttribute('inactive', 'assets/pngs/pauseButton.png');

      _this.slider = new Slider();
      _this.slider.setAttribute('orient', 'horiz');

      _this.controls = µ('+div', _this);
      _this.controls.className = 'controls';
      _this.controls.appendChild(_this.button);
      _this.controls.appendChild(_this.slider);

      _this.appendChild(_this.controls);

      _this.onVideoEnd = ()=>{};

      _this.player.onStop = function() {
        console.log('ended');
        _this.button.set();
        _this.onVideoEnd();
      };

      _this.player.onUpdate = () => {
        _this.slider.setPercent(_this.player.getPercentDone());
      };

      _this.button.onSet = () => {
        _this.player.stop();
      };

      _this.button.onReset = () => {
        if (_this.slider.getPercent() >= .99) _this.player.setFrameByPercent(0);
        _this.player.play();
      };

      _this.slider.onMoved = () => {
        _this.player.setFrameByPercent(_this.slider.getPercent());
        _this.player.stop();
        _this.button.set();
      };

      _this.onLoad = () => {

      };

      _this.player.onLoad = () => {
        _this.onLoad();
      };

      _this.loadSet = (dir) => {
        console.log(dir + ' in videoP');
        clearInterval(idleTimer);
        idleTimer = setInterval(_this.player.idle, 50);
        _this.player.loadSet(dir);
      };

      _this.onUnload = () => {

      };

      _this.unload = () => {
        clearInterval(idleTimer);

        _this.player.reset();
      };

      _this.play = () => {
        _this.player.play();
        _this.button.reset();
      };

      _this.pause = () => {
        _this.player.stop();
        _this.button.set();
      };
    };

    this.attachedCallback = function() {

    };
  });

  window.VideoPlayer = document.registerElement('video-player', playerTag);
});
