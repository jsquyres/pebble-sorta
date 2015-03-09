<!DOCTYPE html>
<html>
  <head>
    <title>Configurable</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.css" />
    <script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
    <script src="http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.js"></script>
  </head>
  <body>
    <div data-role="page" id="main">
      <div data-role="header" class="jqm-header">
        <h1>Configuration for sorta Pebble watch face</h1>
      </div>

      <div data-role="content">

        <div data-role="fieldcontain">
          <label for="shake_exact">Shake to show exact time:</label>
          <select name="shake_exact" id="shake_exact" data-role="slider">
            <option value="off">Off</option>
            <option value="on" selected="selected">On</option>
          </select>
        </div>

        <div data-role="fieldcontain">
          <fieldset data-role="controlgroup">
            <legend>How many seconds to display the exact time:</legend>
            <input type="radio" name="radio-exact-time" id="exact_5" class="custom" checked="checked"/>
            <label for="exact_5">5 seconds</label>

            <input type="radio" name="radio-exact-time" id="exact_10" class="custom" />
            <label for="exact_10">10 seconds</label>

            <input type="radio" name="radio-exact-time" id="exact_30" class="custom" />
            <label for="exact_30">30 seconds</label>

            </fieldset>
          </div>
        </div>

        <div class="ui-body ui-body-b">
          <fieldset class="ui-grid-a">
              <div class="ui-block-a"><button type="submit" data-theme="d" id="b-cancel">Cancel</button></div>
              <div class="ui-block-b"><button type="submit" data-theme="a" id="b-submit">Submit</button></div>
            </fieldset>
          </div>
        </div>
      </div>
    </div>
    <script>
      function saveOptions() {
        var options = {}
        //Add all textual values
        $('textarea, select, [type="hidden"], [type="password"], [type="text"]').each(function(){options[$(this).attr('id')] = $(this).val();})
        //Add all checkbox type values
        $('[type="radio"], [type="checkbox"]').each(function(){options[$(this).attr('id')] = $(this).is(':checked');})
        return options;
      }
      $().ready(function() {
        $("#b-cancel").click(function() {
          console.log("Cancel");
          document.location = "pebblejs://close";
        });
        $("#b-submit").click(function() {
          console.log("Submit");
          var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(saveOptions()));
          console.log("Warping to: " + location);
          console.log(location);
          document.location = location;
        });
        //Set form values to whatever is passed in.
        var obj = jQuery.parseJSON(decodeURIComponent(window.location.search.substring(1)));
        for(key in obj) {
          $("#"+[key]).val(obj[key]);
          $("#"+[key]).val(obj[key]).slider("refresh");
        }
      });
    </script>
  </body>
</html>