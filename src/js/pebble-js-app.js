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
	    if (configuration.shake_exact == "on") {
		message.KEY_ENABLE_SHAKE_EXACT = 1;
		// Default value
		message.KEY_SHAKE_EXACT_TIMEOUT = 5;

		if (configuration.exact_5 == true) {
		    message.KEY_SHAKE_EXACT_TIMEOUT = 5;
		} else if (configuration.exact_10 == true) {
		    message.KEY_SHAKE_EXACT_TIMEOUT = 10;
		} else if (configuration.exact_30 == true) {
		    message.KEY_SHAKE_EXACT_TIMEOUT = 30;
		}
		console.log('Configuration shake=on, timeout=' +
			    message.KEY_SHAKE_EXACT_TIMEOUT);
	    } else {
		console.log('Configuration shake=off. Sadness.');
		message.KEY_ENABLE_SHAKE_EXACT = 0;
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
