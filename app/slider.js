'use strict';

include([], function() {
  var sliderTag = inheritFrom(HTMLElement, function() {
    this.createdCallback = function() {
      var _this = this;
      _this.handle = µ('+div', _this);
      _this.handle.className = 'slider-handle';
    };

    this.attachedCallback = function() {
      var _this = this;
      _this.orient = µ('|>orient', _this);

      var mName = ((_this.orient == 'vert') ? 'marginTop' : 'marginLeft');
      var eName = ((_this.orient == 'vert') ? 'Y' : 'X');
      var rName = ((_this.orient == 'vert') ? 'top' : 'left');
      var sName = ((_this.orient == 'vert') ? 'clientHeight' : 'clientWidth');

      _this.clicked = false;
      _this.offset = 0;
      _this.start = 0;

      var margin = function() {
        return _this.handle.style[mName];
      };

      var handleSize = function() {
        return parseFloat(_this.handle[sName]);
      };

      var bgSize = function() {
        return parseFloat(_this[sName]);
      };

      _this.setPercent = function(perc) {
        _this.handle.style[mName] = (perc * (bgSize() - handleSize())) + 'px';
      };

      _this.getPercent = function() {
        return parseFloat(margin()) / (parseFloat(bgSize()) - handleSize());
      };

      _this.onMoved = () => {};

      _this.moveHandle = function(off) {
        if (off > bgSize() - handleSize()) off = bgSize() - handleSize();
        else if (off < 0) off = 0;
        _this.handle.style[mName] = off + 'px';

        /*buuk.changePosByPercent(posX / (parseFloat(bgDiv.style.width) - handle.width));
        buuk.stop();*/
        _this.onMoved();
      };

      _this.handle.onmousemove = function(e) {
        if (_this.clicked) _this.moveHandle((e['client' + eName] -  (_this.start - _this.offset)));
      };

      _this.handle.onmouseup = function() {
        _this.clicked = false;
        document.onmousemove = null;
        document.onmouseup = null;
      };

      _this.handle.onmousedown = function(e) {
        _this.clicked = true;
        var rect = _this.handle.getBoundingClientRect();
        _this.offset = 0;
        _this.start = e['client' + eName];
      };

      _this.onmousedown = function(e) {
        _this.clicked = true;
        var bg = _this.getBoundingClientRect();
        var hnd = _this.handle.getBoundingClientRect();
        _this.offset = hnd[rName] - bg[rName];
        _this.start = e['client' + eName];
        _this.moveHandle(e['client' + eName] - (bg[rName] + handleSize() / 2));
        document.onmousemove = _this.handle.onmousemove;
        document.onmouseup = _this.handle.onmouseup;
        return false;
      };
    };
  });

  window.Slider = document.registerElement('sli-der', sliderTag);
});
