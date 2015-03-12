Pebble.addEventListener('ready',
    function (e) {
	console.log("JavaScript app ready and running!");
    }
);

Pebble.addEventListener('showConfiguration',
    function(e) {
	Pebble.openURL('http://jeff.squyres.com/pebble-sorta/config/');
    }
);

Pebble.addEventListener('webviewclosed',
    function(e) {
	// If we got a response, handle it
	if (e.response) {
	    var configuration = JSON.parse(decodeURIComponent(e.response));
	    console.log('Configuration window returned: ' +
			JSON.stringify(configuration));

	    var message = new Object;
	    if (configuration.shake_enable == "on") {
		message.KEY_SHAKE_ENABLE = 1;
		// Default value
		message.KEY_SHAKE_TIMEOUT = 5;

		if (configuration.timeout_5 == true) {
		    message.KEY_SHAKE_TIMEOUT = 5;
		} else if (configuration.timeout_10 == true) {
		    message.KEY_SHAKE_TIMEOUT = 10;
		} else if (configuration.timeout_30 == true) {
		    message.KEY_SHAKE_TIMEOUT = 30;
		}
		console.log('Configuration shake=on, timeout=' +
			    message.KEY_SHAKE_TIMEOUT);
	    } else {
		console.log('Configuration shake=off. Sadness.');
		message.KEY_SHAKE_ENABLE = 0;
	    }

	    // Send the config message to the Pebble.  Currently
	    // ignore it if the message fails to send...
	    Pebble.sendAppMessage(message,
                function(e) {
		    console.log('Config message sent to Pebble successfully');
		},
                function(e) {
		    console.log('Config message failed to send to Pebble!');
		}
            );
	} else {
	    // If we canceled, there's nothing to do
	    console.log('Configuration canceled');
	}
    }
);
