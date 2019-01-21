var Pusher = require('pusher');

var pusher = new Pusher({
  appId: '617388',
  key: '7a8493d0fc933acc6124',
  secret: 'a324ad3b78a1f14800a8',
  cluster: 'us2',
  encrypted: true
});

pusher.trigger('my-channel', 'my-event', {
  "message": "hello world"
});
