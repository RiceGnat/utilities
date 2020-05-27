function New-VbsLauncher {
    [CmdletBinding()]
    param(
        # The run path for the app
        [Parameter(Mandatory=$True)]
        [string]$StartPath,

        # The executable or command to run
        [Parameter(Mandatory=$True)]
        [string]$AppTarget,
        
        # The output file
        [Parameter(Mandatory=$True)]
        [string]$OutputFile
    )
"On Error Resume Next
Set objShell = CreateObject(""Wscript.Shell"")
strApp = ""$StartPath""

objShell.CurrentDirectory = strApp
objShell.Run ""$AppTarget""" | Out-File $OutputFile
}

function New-VisualElementsManifest {
    [CmdletBinding()]
    param(
        # The output file
        [Parameter(Mandatory=$True)]
        [string]$OutputFile,

        # The image to be used as the large tile icon
        [Parameter(ValueFromPipelineByPropertyName,Mandatory=$True)]
        [string]$IconLarge,
    
        # The image to be used as the small tile icon
        [Parameter(ValueFromPipelineByPropertyName)]
        [string]$IconSmall,
    
        # The hex code for the tile's background color
        [Parameter(ValueFromPipelineByPropertyName,Mandatory=$True)]
        [string]$BackgroundColor,
    
        # Use dark foreground text instead of light on the tile
        [Parameter(ValueFromPipelineByPropertyName)]
        [switch]$UseDarkForegroundText,
    
        # Show name on the tile
        [Parameter(ValueFromPipelineByPropertyName)]
        [switch]$ShowName
    )

    $attributes = "
      Square150x150Logo='$IconLarge'
      Square70x70Logo='$IconSmall'
      ShowNameOnSquare150x150Logo=";

    if ($ShowName) {
        $attributes += "'on'"
    }
    else {
        $attributes += "'off'"
    }

    $attributes += "
      BackgroundColor='$BackgroundColor'
      ForegroundText="

    if ($UseDarkForegroundText) {
        $attributes += "'dark'"
    }
    else {
        $attributes += "'light'"
    }


"<Application xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>
  <VisualElements $attributes />
</Application>" | Out-File $OutputFile
}

function Edit-ShortcutLastWrite {
    [CmdletBinding()]
    param(
        # The shortcut to modify
        [Parameter(Mandatory=$True)]
        [string]$ShortcutFile
    )
    (ls "$ShortcutFile").lastwritetime = Get-Date
}