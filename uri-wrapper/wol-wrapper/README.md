Wrapper for command line wake-on-LAN utility WolCmd.

WolCmd can be downloaded at: https://www.depicus.com/wake-on-lan/wake-on-lan-cmd

Usage
--------------
	wol-wrapper pathtoWolCmd uri

The path will be called directly through command prompt. The URI will look something like

	wol:(macaddress)-(broadcastip)-(subnetmask)

The three parameters in the URI will be passed directly to WolCmd in order. See the WolCmd documentation at the link above for how to format the parameters.

Example:

	wol-wrapper C:\WolCmd.exe wol:00a1b2c3d4e5-192.168.0.1-255.255.255.0
