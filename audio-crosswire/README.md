Allows routing of existing sound devices through the 7.1 channel output ports. Captures audio devices and outputs them to the additional channels of the primary playback device.

Currently pipes the default playback device's stereo output to channels 5 and 6 (back left and right) and the default capture device to channels 7 and 8 (side left and right).

Code is based off of the following WASAPI sample code:
http://blogs.msdn.com/b/matthew_van_eerde/archive/2014/11/05/draining-the-wasapi-capture-buffer-fully.aspx
https://github.com/mvaneerde/blog/tree/master/loopback-capture