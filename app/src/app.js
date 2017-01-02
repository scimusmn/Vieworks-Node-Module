'use strict';

console.log('app');

let incs = [
  'thumbnails.js',
  'webSockets.js',
  'videoPlayer.js',
  'button.js',
  'config.js',
];

include(incs, () => {
  console.log('loaded!');

  window.onload = function() {
  };

  var visGroup = µ('#thumbs');

  var celebGroup = µ('#celebThumbs');

  visGroup.onChoose = ()=>{
    µ('body')[0].className = 'JustYou';
  }

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

  //µ('#celebPlayer').player.changeNotLoadedImage('assets/pngs/imageFrame.png');
  µ('#celebPlayer').player.cached = true;

  µ('#visitorPlayer').onLoad = () => {
    µ('#visitorPlayer').play();
  };

  µ('#celebPlayer').onLoad = () => {
    µ('#celebPlayer').play();
  };

  µ('#celebPlayer').player.onStateChange = ()=>{
    let celebState = µ('#celebPlayer').player.playing;
    let visitorState = µ('#visitorPlayer').player.playing;

    if(celebState || visitorState){
      µ('#playBoth').reset();
    } else if(!celebState && !visitorState){
      µ('#playBoth').set();
    }
  }

  µ('#visitorPlayer').player.onStateChange = µ('#celebPlayer').player.onStateChange;

  /////////////////////////////
  // mode selectors
  /////////////////////////////

  function showJY() {
    µ('body')[0].className = 'JustYou';
    µ('#celebPlayer').unload();
  }

  function showSBS() {
    µ('body')[0].className = 'SideBySide';
  }

  function showSelect() {
    µ('#visitorPlayer').unload();
    µ('#celebPlayer').unload();
    µ('body')[0].className = 'findYourVideo';
    if(resetTimer) clearTimeout(resetTimer);
    visGroup.resetActive();
    celebGroup.resetActive();
  }

  //////////////////////////////
  // ui functions
  /////////////////////////////

  µ('#jy').onclick = showJY;

  µ('#sbs').onclick = showSBS;

  µ('#fyv').onclick = showSelect;

  µ('#playBoth').onSet = function() {
    µ('#visitorPlayer').pause();
    µ('#celebPlayer').pause();
  };

  µ('#playBoth').onReset = function() {
    µ('#visitorPlayer').play();
    µ('#celebPlayer').play();
  };

  // µ('#celebSlider').onMoved = () => {
  //   var _this = µ('#celebSlider');
  //   var cThumbs = µ('#celebThumbs');
  //   cThumbs.style.marginTop = -(cThumbs.scrollHeight - cThumbs.parentNode.clientHeight) * _this.getPercent() + 'px';
  // };
  //
  // µ('#celebThumbs').onMoved = () => {
  //   var _this = µ('#celebThumbs');
  //   var sld = µ('#celebSlider');
  //   sld.setPercent(-(parseFloat(_this.style.marginTop) / (_this.scrollHeight - _this.parentNode.clientHeight)));
  // };

  var resetTimer = null;

  µ('#visitorPlayer').onVideoEnd = ()=>{
    resetTimer = setTimeout(()=>{
      showSelect();
    },60000);
  }

  µ('body')[0].onclick = ()=>{
    console.log('body click!');

    clearTimeout(resetTimer);
    resetTimer = setTimeout(()=>{
      showSelect();
    },120000);
  }
});
