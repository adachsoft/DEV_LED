const char INDEX_HTML[] PROGMEM = R"=====(
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
        max-width: 360px;
        margin-top: 0;
        margin-bottom: 0;
        margin-left: auto;
        margin-right: auto;
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
        <a href="config">Config</a><br>)====="
#ifdef DS18B20_MODULE 
        R"=====(<a href="temp">Temp</a><br>)====="
#endif
R"=====(
    </div>
  
    <script>
      
    </script>
</body>
</html>
)=====";
