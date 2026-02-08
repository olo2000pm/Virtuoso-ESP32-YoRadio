/* minimal settings UI logic */
(function(){
  function show(el){ if(el) el.classList.remove('hidden'); }
  var nav = document.getElementById('navigation'); show(nav);
  var navitems = nav ? nav.querySelectorAll('.navitem') : []; for (var i=0;i<navitems.length;i++) show(navitems[i]);
  ['group_system','group_display','group_controls','group_timezone','group_weather'].forEach(function(id){ show(document.getElementById(id)); });
  var ws = null;
  try { ws = new WebSocket((location.protocol==='https:'?'wss://':'ws://') + location.host + '/ws'); } catch(e) {}
  if (ws) {
    ws.onopen = function(){
      ws.send('getweather=1');
      ws.send('getscreen=1');
      ws.send('getsystem=1');
    };
    ws.onmessage = function(ev){
      try {
        var data = JSON.parse(ev.data);
        if (typeof data.wen !== 'undefined') {
          var wchk = document.getElementById('showweather');
          if (wchk) { wchk.classList[data.wen?'add':'remove']('on'); wchk.classList[data.wen?'remove':'add']('off'); }
        }
        if (typeof data.wlat !== 'undefined') { var e=document.getElementById('weatherlat'); if(e) e.value=data.wlat; }
        if (typeof data.wlon !== 'undefined') { var e=document.getElementById('weatherlon'); if(e) e.value=data.wlon; }
        if (typeof data.wkey !== 'undefined') { var e=document.getElementById('weatherkey'); if(e) e.value=data.wkey; }
      } catch(_){}
    };
  }
  var aw = document.getElementById('applyweather');
  if (aw) aw.addEventListener('click', function(){
    var lat = document.getElementById('weatherlat').value;
    var lon = document.getElementById('weatherlon').value;
    var key = document.getElementById('weatherkey').value;
    if(ws){
      ws.send('lat=' + lat);
      ws.send('lon=' + lon);
      ws.send('key=' + key);
      ws.send('getweather=1');
    }
  });
})(); 
