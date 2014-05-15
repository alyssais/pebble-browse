var inboxSize;

var sendMessage = function(message, callback) {
  var chunkLength = 500;
  var chunkCount = ((message.length / chunkLength)|0) + 1;

  var chunkQueue = []
  for (var i = 0; i < chunkCount; i++) {
    chunkQueue.push(message.substr(chunkLength * i, chunkLength));
  }

  // send meta message
  Pebble.sendAppMessage({responseLength: message.length}, function() {
    // message has been sent successfully.
    var sendRemainingChunks = function() {
      var chunk = chunkQueue[0];
      if (!chunk) return callback();
      Pebble.sendAppMessage({chunk: chunk}, function() {
        // chunk sent successfully, remove from array
        chunkQueue.shift();
        sendRemainingChunks();
      }, function() {
        // chunk was not delivered, keep in array to try again
        sendRemainingChunks();
      });
    };
    sendRemainingChunks();
  });
}

var HTTP = new function() {
  this.get = function(url) {
    var request = new XMLHttpRequest();
    request.open("GET", url, true);
    request.onload = function() {
      if (this.status >= 200 && this.status < 400) {
        sendMessage(this.response, function() {
          console.log("sent message");
        });
      }
    };
    request.send();
  };
};

Pebble.addEventListener("appmessage", function(e) {
  if (e.payload.inboxSize) {
    inboxSize = parseInt(e.payload.inboxSize.reverse().map(function(byte) {
      var string = byte.toString(2);
      while (string.length < 8) string = "0" + string;
      return string;
    }).join(""), 2);

    HTTP.get('http://alyssa.is');
  }
});
