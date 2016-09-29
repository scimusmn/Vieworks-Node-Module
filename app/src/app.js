'use strict';

let incs = [
  'thumbSelect_revise.js',
  'webSockets.js',
  'videoPlayer.js',
  'button.js',
];

include(incs, () => {
  console.log('loaded!');

  window.onload = function() {
  };

  var visGroup = µ('#thumbs');

  var celebGroup = µ('#celebThumbs');

  window.webSockClient.onMessage = (evt) => {
    switch (evt.data.split('=')[0]){
      case 'seq':
        visGroup.handleSet(evt.data.split('=')[1],parseInt(evt.data.split(':')[2]));
        break;
      case 'cel':

        celebGroup.handleSet(evt.data.split('=')[1],parseInt(evt.data.split(':')[2]));
        break;
      default:
        break;
    }
  };

  webSockClient.connect();

  µ('#visitorPlayer').onLoad = () => {
    µ('#visitorPlayer').play();
  };

  µ('#celebPlayer').onLoad = () => {
    µ('#celebPlayer').play();
  };

  /////////////////////////////
  // mode selectors
  /////////////////////////////

  function showJY() {
    µ('.justYou').style('display', 'inline-block');

    µ('.sideBySide').style('display', 'none');
  }

  function showSBS() {
    µ('#celebPlayer').unload();
    µ('.sideBySide').style('display', 'inline-block');

    µ('.justYou').style('display', 'none');
  }

  function showSelect() {
    showJY();

    µ('#visitorPlayer').unload();

    µ('.select').style('display', 'inline-block');

    µ('.playback').style('display', 'none');
  }

  //////////////////////////////
  // ui functions
  /////////////////////////////

  µ('#jy').onClick = showJY;

  µ('#sbs').onClick = showSBS;

  µ('#fyv').onClick = showSelect;

  µ('#playBoth').onSet = function() {
    µ('#visitorPlayer').pause();
    µ('#celebPlayer').pause();
  };

  µ('#playBoth').onReset = function() {
    µ('#visitorPlayer').play();
    µ('#celebPlayer').play();
  };

  µ('#celebSlider').onMoved = () => {
    var _this = µ('#celebSlider');
    var cThumbs = µ('#celebThumbs');
    cThumbs.style.marginTop = -(cThumbs.scrollHeight - cThumbs.parentNode.clientHeight) * _this.getPercent() + 'px';
  };

  µ('#celebThumbs').onMoved = () => {
    var _this = µ('#celebThumbs');
    var sld = µ('#celebSlider');
    sld.setPercent(-(parseFloat(_this.style.marginTop) / (_this.scrollHeight - _this.parentNode.clientHeight)));
  };
});
