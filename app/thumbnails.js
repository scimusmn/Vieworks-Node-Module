'use strict';

include([], function() {
  var thumbnail = inheritFrom(HTMLElement, function() {

    this.createdCallback = function() {
      var _this = this;
      //_this.thumb = µ('+img', _this);

      _this.onSelect = () => {};

      _this.onMouseStart = (which) => {};

      _this.onMouseFinish = () => {};

      _this.setImage = ()=>{
        _this.style.backgroundImage = "url('"+_this.setName + '/400.jpg?' + Math.random()+"')";
      }
    };

    this.attachedCallback = function() {
      var _this = this;

      _this.setName = µ('|>setName', _this);
      _this.start = { x:0, y:0 };
      _this.init = { x:0, y:0 };

      _this.reset = () => {
        _this.className = '';
        //_this.thumb.src = _this.setName + '/400.jpg?' + Math.random();
        _this.setImage();
      };

      _this.onmousedown = (e) => {
        e.preventDefault();
        _this.clicked = true;
        document.onmouseup = _this.onmouseup;
        _this.onMouseStart(_this);

        var rest = µ('thumb-nail');
        for (var i = 0; i < rest.length; i++) {
          rest[i].reset();
        }
      };

      _this.onmouseup = function(e) {
        console.log(e.target);
        if (_this.clicked) {

          _this.className = '';
          _this.onSelect();
        }

        _this.onMouseFinish();

        _this.clicked = false;
        document.onmouseup = null;
        document.onmousemove = null;
      };

      _this.refreshSet = function() {
        _this.setImage();
      };
    };

    this.attributeChangedCallback = function(attr, oldVal, newVal) {
      var _this = this;
      if (attr == 'setname') {
        _this.setName = newVal;
        _this.setImage();
      }
    };
  });

  var Thumbs = document.registerElement('thumb-nail', thumbnail);

  var thumbGroup = inheritFrom(HTMLElement, function() {
    this.createdCallback = function() {
      var _this = this;

      _this.player = µ('#' + µ('|>player', _this));
      _this.max = parseInt(µ('|>max', _this));
      _this.scrollable = (µ('|>scrollable', _this) == 'true');
      _this.activeThumb = null;
      _this.start = { x:0, y:0 };
      _this.init = { x:0, y:0 };

      _this.logThumb = (aTh) => {
        _this.activeThumb = aTh;
        console.log('thumb!');
      };

      _this.onMoved = () => {};

      _this.onmousedown =  (e) => {
        if (_this.scrollable) {
          _this.clicked = true;
          _this.start.y = e.clientY - extractNumber(_this.style.marginTop);
          _this.init.y = e.clientY;
        }
      };

      _this.onmousemove = function(e) {
        if (_this.clicked && _this.scrollHeight > _this.parentNode.clientHeight) {
          if (Math.abs(e.clientY - _this.init.y) > 20 && _this.activeThumb)
            _this.activeThumb.clicked = false;
          var offset = e.clientY - _this.start.y;
          var max = _this.parentNode.clientHeight - _this.scrollHeight;
          if (offset > 0) offset = 0;
          else if (offset < max) offset = max;
          _this.style.marginTop = offset + 'px';

          _this.onMoved();
        }
      };

      _this.onmouseup = () => {
        _this.clicked = false;
      };

      _this.onChoose = () =>{

      }

      this.handleSet = function(setName) {
        var set = null;
        if (µ('[setName="' + setName + '"]') && µ('[setName="' + setName + '"]').length) {
          set = µ('[setName="' + setName + '"]')[0];
          set.setAttribute('setName', setName);
          set.refreshSet();
          _this.removeChild(set);
        } else {
          if (_this.childNodes.length >= _this.max) _this.removeChild(_this.lastChild);
          set = document.createElement('thumb-nail');//new setPointer(setName,flipPlr);
          set.setAttribute('setName', setName);

          set.onSelect = function() {
            _this.onChoose();
            _this.player.loadSet(set.setName + '/');
          };

          set.onMouseStart = _this.logThumb;
          set.onMouseFinish = _this.onmouseup;
        }

        if (_this.childNodes.length > 1) {
          _this.insertBefore(set, _this.firstChild);
        } else {
          _this.appendChild(set);
        }
      };

    };
  });

  var ThumbGroup = document.registerElement('thumb-group', thumbGroup);

  //console.log(visitorCaps);
});
