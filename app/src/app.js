'use strict';

let incs = [
  'thumbSelect_revise.js',
  'mouseFxn.js',
  'flipPlayer.js',
  'webSockets.js',
  'posSlider.js',
  'thumbSlider.js',
];

include(incs, () => {
  console.log('loaded!');
  var visitorCaps = new flipPlayer('#visitor-caps');
  var celebCaps = new flipPlayer('#celeb-caps');

  window.onload = function() {
    //visitorCaps.loadSet("default/");
    visitorCaps.init();
    celebCaps.init();
  };

  var visGroup = new setGroup(visitorCaps, µ('#thumbs'), 3, 7, true);
  var celebGroup = new setGroup(celebCaps, µ('#celebContainer'), 20, 1, false);

  celebGroup.setCelebMode();

  window.webSockClient.onMessage = (evt) => {
    switch (evt.data.split('=')[0]){
      case 'seq':
        visGroup.addOrChangeSet(evt.data.split('=')[1]);
        break;
      case 'cel':
        celebGroup.addOrChangeSet(evt.data.split('=')[1]);
        break;
      default:
        break;
    }
  };
});
