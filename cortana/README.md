# Cortana voice commands
Custom voice commands for Cortana can be defined by using a Universal Windows app. Each command is technically an instruction to launch your app which will then handle the command.

There is currently no way to directly add voice commands that do not depend on a host app.

Built with Visual Studio 2015.

## CortanaCommands
Universal Windows app to register the voice command definitions and handle the commands. Launches the appropriate URI string for the command.

Note that this must be a foreground app to work. Launching files or URIs will fail from an app service so Cortana must first launch the app in the foreground. The app is set to automatically close itself after handling the command. The app must be launched at least once for the voice command definitions to be registered.

## cortana-uri
Handler that accepts a URI string and runs the appropriate command.

Universal Windows Platform apps are not permitted to directly run executables or scripts. Instead, the handler must be associated with a custom URI protocol which the Cortana command service will launch.

### Usage
	cortana-uri uristring

The *uristring* parameter should consist of the URI protocol (*cortana:*), the command, and the parameters for the command.

Possible commands:

	cortana:wol-(macaddress)-(broadcastip)-(subnetmask)
	cortana:shutdown-(computer)
	cortana:sleep-(computer)
	cortana:restart-(computer)

Examples:

	cortana-uri cortana:00a1b2c3d4e5-192.168.0.1-255.255.255.0
	cortana-uri cortana:shutdown-Kaystros
	cortana-uri cortana:sleep-Kaystros
	cortana-uri cortana:restart-Kaystros

The wake-on-LAN functionality uses the WolCmd utility, available here: https://www.depicus.com/wake-on-lan/wake-on-lan-cmd

The shutdown, sleep, and restart commands use PsShutdown, available as part of the PsTools package here: https://technet.microsoft.com/en-us/sysinternals/psshutdown.aspx

Both utilities must be visible to the command line. Edit the system PATH environment variable accordingly.

### Registering URI
The URI scheme must be registered with Windows in order for the handler to be called when the URI is launched. See here: https://msdn.microsoft.com/en-us/library/aa767914%28v=vs.85%29.aspx

A batch file is included that will register the URI scheme. It can be called as follows:

	register-protocol [handler]

The optional *handler* parameter should be the location of the handler executable. If this parameter is omitted, it will be assumed that the *cortana-uri.exe* executable exists in the location from which the script is called.
