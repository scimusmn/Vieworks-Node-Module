'use strict';

var thumbnail = inheritFrom(HTMLElement, function() {
  this.setPlayer = function(plr) {
    this.player = plr;
  };

  this.createdCallback = function() {
    var _this = this;
    _this.thumb = µ('+img', _this);
  };

  this.attachedCallback = function() {
    var _this = this;
    _this.setName = µ('|>setName', _this).replace('/', '-');

    _this.reset = () => {
      _this.className = '';
      _this.thumb.src = _this.setName.replace('-', '/') + '/thumb.jpg';
    };

    _this.onmousedown = () => {
      _this.clicked = true;
      _this.thumb.src = 'assets/pngs/thumbBG.png';
      _this.className = 'thumbSelect';
      var rest = µ('thumb-nail');
      for (var i = 0; i < rest.length; i++) {
        rest[i].reset();
      }
    };

    _this.onmouseup = function() {
      if (_this.clicked) {
        thumbClick();
				_this.className = 'thumbSelect';
        _this.player.loadSet(_this.setName.replace('-', '/') + '/');
      }
    };
  };

  this.attributeChangedCallback = function(attr, oldVal, newVal) {
    console.log(attr);
    if (attr == 'setname') {
      console.log('setAttribute');
      this.thumb.src = newVal.replace('-', '/') + '/thumb.jpg';
      this.setName = newVal.replace('/', '-');
    }
  };
});

var Thumbs = document.registerElement('thumb-nail', thumbnail);

function thumbClick() {
  var selEl = document.querySelectorAll('.playback');
  console.log(selEl);
  for (var i = 0; i < selEl.length; i++) {
    selEl[i].style.display = 'table-row';
  }

  selEl = document.querySelectorAll('.select');
  for (var i = 0; i < selEl.length; i++) {
    selEl[i].style.display = 'none';
  }
}

window.setGroup = function(flp, parent, rws, clm, asTable) {
  var flipPlr = flp;
  var sets = [];
  var elements = [];

  var rows = rws;
  var columns = clm;
  var visitorMode = true;
  var tableMode = asTable;

  this.setCelebMode = function() {
    visitorMode = false;
    flipPlr.celebMode();

    //for(var i=0; i<sets.length; i++){
    //sets[i].setCelebMode(this.resetSelected());
    //}
  };

  this.resetSelected = function() {
    for (var i = 0; i < sets.length; i++) {
      sets[i].reset();
    }
  };

  this.addOrChangeSet = function(setName) {
    console.log(setName);
    setName = setName.replace('/', '-');

    //var setFol = setName.split('/');
    //var set = setFol[setFol.length-1]
    if (µ('[setName=' + setName + ']') && µ('[setName=' + setName + ']').length) {
      var set = µ('[setName=' + setName + ']')[0];
      set.setAttribute('setName', setName);
      if (parent.childNodes.length > 1) {
        var temp = parent.removeChild(set);
        parent.insertBefore(temp, parent.firstChild);
      }
    } else {
      //console.log(flipPlr);
      console.log(setName);
      var newSet = document.createElement('thumb-nail');//new setPointer(setName,flipPlr);
      newSet.setAttribute('setName', setName);
      newSet.setPlayer(flipPlr);

      //if(!visitorMode) newSet.setCelebMode(this);
      var curNum = sets.length;
      sets.push(newSet);
      if (parent.childNodes.length > 1) {
        parent.insertBefore(newSet, parent.firstChild);
      } else {
        parent.appendChild(newSet);
      }
    }
  };

};

//console.log(visitorCaps);
