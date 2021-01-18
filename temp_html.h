const char TEMP_HTML[] PROGMEM = R"=====(
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
        {$temp}
        <hr>
        
        <form action="save_temp" method="POST">
            {$temp_form}
            
            <button type="submit">Save</button>
        </form>

        <a href="/">Main menu</a><br>
    </div>
  
    <script>
      getTemp();
      
      function getTemp()
      {
        var xmlhttp = new XMLHttpRequest();
        var url = "/tempJson";
        
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                processResponse(
                  JSON.parse(this.responseText)
                );
            }
        };
        xmlhttp.open("GET", url, true);
        xmlhttp.send();

        setTimeout(function(){ getTemp(); }, 1000);
      }
      
      function processResponse(arr) 
      {
          let i;
          for(i = 0; i < arr['TEMP'].length; i++) {
              document.getElementById('temp_' + arr['TEMP'][i]['ADDRESS']).innerHTML = arr['TEMP'][i]['VALUE'];
          }
      }
    </script>
</body>
</html>
)=====";
