Pebble.addEventListener('ready', function() {
  console.log("Pebble JS is ready!");
  getWeather();
});

var getWeather = function() {
  var request = new XMLHttpRequest();
  request.open('GET',
               'http://api.openweathermap.org/data/2.5/weather?lat=37.77&lon=-122.42',
               true);
  request.onload = sendWeather;
  request.send();
};

var sendWeather = function(xhr) {
  var response = JSON.parse(xhr.target.response);
  var temp = kToF(response.main.temp);
  Pebble.sendAppMessage({
    'temperature': temp + '\u00B0F'
  });
};

var kToF = function(k) {
  var f = (k - 273.15) * 1.8 + 32;
  return f.toFixed(1);
}
