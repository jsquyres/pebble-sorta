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
        <h1>sorta configuration</h1>
      </div>

      <div data-role="content">

        <div data-role="fieldcontain">
          <fieldset data-role="controlgroup">
            <legend>Watch face colors:</legend>
            <input type="radio" name="black_text" id="black_text_on" class="custom" checked="checked"/>
            <label for="black_text_on">Black text on white background</label>

            <input type="radio" name="black_text" id="black_text_off" class="custom" />
            <label for="black_text_off">White text on black background</label>
          </fieldset>
        </div>

        <div data-role="fieldcontain">
          <label for="shake_enable">Shake to show exact time:</label>
          <select name="shake_enable" id="shake_enable" data-role="slider">
            <option value="off">Off</option>
            <option value="on" selected="selected">On</option>
          </select>
        </div>

        <div data-role="fieldcontain">
          <fieldset data-role="controlgroup">
            <legend>How many seconds to display the exact time:</legend>
            <input type="radio" name="radio-timeout" id="timeout_5" class="custom" checked="checked"/>
            <label for="timeout_5">5 seconds</label>

            <input type="radio" name="radio-timeout" id="timeout_10" class="custom" />
            <label for="timeout_10">10 seconds</label>

            <input type="radio" name="radio-timeout" id="timeout_30" class="custom" />
            <label for="timeout_30">30 seconds</label>

          </fieldset>
        </div>

        <div class="ui-body ui-body-b">
          <fieldset class="ui-grid-a">
              <div class="ui-block-a"><button type="submit" data-theme="d" id="b-cancel">Cancel</button></div>
              <div class="ui-block-b"><button type="submit" data-theme="a" id="b-submit">Submit</button></div>
          </fieldset>
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
