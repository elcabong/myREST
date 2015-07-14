# myREST
Based on the aREST-node module for node.js by Marco Schwartz http://github.com/marcoschwartz
 
Re-wrote the aREST api for esp8266 paired with  atmega328p/arduino.  the atmega does all the heavy lifting, the esp uses the nodemcu firmware as a tcp server / passthrough.

The interface is based on an older version of the aREST-node example that used curl/php to control the device.  I've found this works better than the jQuery $.get() function.

Basically the interface should work like this:

1) In interface.html, onClick triggers a function called buttonClick in script.js which uses a $.get to call curl.php which sends the command to the device. This works fine all by itself and is based off of Marco Schwartz' original example code.

2) Meanwhile, the setInterval function in script.js should poll the device every 2 seconds using jQuery (doesn't seem to be working right now), then change the css style of the buttons based on the status of the pin associated with the buttons.

Besides controlling the device, it should appear that clicking one of the on/off buttons changes their colors (with up to a 2 second delay of course), but really they change because the status of the pin changed.

Almost works, except the .get inside of setinterval is not receiving a fresh json string for every time it calls curl.php.  A page reload does nothing.  Full browser exit and re-open is the only way to get new data.

On the plus side, the .css is nice.
