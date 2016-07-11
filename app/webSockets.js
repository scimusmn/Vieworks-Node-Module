function wsClient(){
	var _this = this;
	var ws;
	var connectInterval;

	this.onMessage = function(evt){};

	this.connect = function(){
		if ("WebSocket" in window) ws = new WebSocket("ws://"+window.location.host+":8080/"); //ws://echo.websocket.org is the default testing server
	}

	this.connect();

    ws.onopen = function()
    {
       // Web Socket is connected, send data using send()
       ws.send("Message to send");
	   clearInterval(connectInterval);
    };

	ws.onerror = function ( error ) {
		if ("WebSocket" in window) connectInterval = setInterval(this.connect(),2000);
	}

    ws.onmessage = function (evt) {
		//var received_msg = evt.data;
		//alert("Message is received... " + received_msg);
		console.log(evt.data);

		_this.onMessage(evt);
    };

	this.setMsgCallback = function(cb){
		ws.onmessage = cb;
	}

    ws.onclose = function(){
       // websocket is closed.
       alert("Connection is closed...");
	   connectInterval = setInterval(this.connect(),2000);
    };

}

window.webSockClient = new wsClient();
