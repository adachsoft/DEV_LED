const char CONFIG_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>DEV_LED</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<style>
    body, html {
        background-color: lightgray;
        width: 100%;
        margin: 0;
        padding: 0;
    }
  .input {
    margin-bottom: 5px;
  }
  .input span {
    display: inline-block;
    width: 150px;
  }
  .form {
      width: 400px;
      margin-left: auto;
      margin-right: auto;
  }
  .form button{
    display: block;
    border-radius: 10px;
    background-color: #1a75ff;
    margin-bottom: 15px;
    margin-top: 15px;
    color: white;
    font-size: 15px;
    width: 200px;
    height: 35px;
    margin-left: auto;
    margin-right: auto;
  }
</style>

<body>
    <h1>DEV_LED</h1>
  
    <div class="form">
        <form action="save" method="POST">
            <div class="input"><span>WIFI ssid:</span><input type="text" name="ssid" value="{$ssid}" /></div>
            <div class="input"><span>WIFI password:</span><input type="password" name="password" value="{$password}" /></div>

            <div class="input"><span>MQTT server:</span><input type="text" name="mqtt_server" value="{$mqtt_server}" /></div>
            <div class="input"><span>MQTT topic:</span><input type="text" name="mqtt_topic" value="{$mqtt_topic}" /></div>
            <div class="input"><span>MQTT group topic:</span><input type="text" name="mqtt_group_topic" value="{$mqtt_group_topic}" /></div>

            <div class="input"><span>Light:</span><input type="text" name="light" value="{$light}" /></div>
            <div class="input"><span>Time off:</span><input type="text" name="time_off" value="{$time_off}" /></div>
            <div class="input"><span>Fade time:</span><input type="text" name="fade_time" value="{$fade_time}" /></div>
            <div class="input">
              <span>Led pin [{$led_pin}]:</span>
              
              <select name="led_pin" id="led_pin" data-selected="{$led_pin}">
                <option value="16">D0 / GPIO 0</option>
                <option value="5">D1 / GPIO 5</option>
                <option value="4">D2 / GPIO 4</option>
                <option value="0">D3 / GPIO 0</option>
                <option value="2">D4 / GPIO 2</option>
                <option value="14">D5 / GPIO 14</option>
                <option value="15">D8 / GPIO 15</option>
                <option value="3">RX / GPIO 3</option>
                <option value="1">TX / GPIO 1</option>
              </select>
             </div>
            <button type="submit">Save</button>
        </form>

        <a href="/">Main menu</a><br>
    </div>
  
    <script>
      var ledPin = document.getElementById('led_pin');
      ledPin.value = ledPin.dataset.selected;
    </script>
</body>
</html>
)=====";
