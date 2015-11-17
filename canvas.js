var canVas = inheritFrom(HTMLCanvasElement, function() {
  this.clear = function() {
    this.width = this.width;
  };

  this.mirror = function() {
    this.ctx.translate(this.width, 0);
    this.ctx.scale(-1, 1);
  };

  this.resize = function(nw, nh) {
    this.width = nw;
    this.height = nh;
  };

  this.createdCallback = function() {
    console.log(this.id);
    this.ctx = this.getContext('2d');
  };
});

document.registerElement('can-vas', {prototype: canVas.prototype, extends: 'canvas'});
