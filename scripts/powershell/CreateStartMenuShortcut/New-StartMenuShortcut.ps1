[CmdletBinding()]
param(
    # The shortcut to modify
    [Parameter(Mandatory=$True)]
    [string]$ShortcutFile,

    # The image to be used as the large tile icon
    [Parameter(Mandatory=$True)]
    [string]$IconLarge,
    
    # The image to be used as the small tile icon
    [string]$IconSmall,
    
    # The hex code for the tile's background color
    [string]$BackgroundColor="#323232",
    
    # Use dark foreground text instead of light on the tile
    [switch]$UseDarkForegroundText,
    
    # Show name on the tile
    [switch]$ShowName,
        
    # Copy icons to shortcut folder
    [switch]$CopyIcons,

    # The location to output the new shortcut
    [string]$OutputPath
)

Import-Module -Name "$PSScriptRoot\functions"

$name = Split-Path -Path $ShortcutFile -LeafBase

if (!($OutputPath)) {
    $OutputPath = Get-Location
}

echo "Copying shortcut"
$out = Join-Path -Path $OutputPath -ChildPath "out.lnk"
Copy-Item $ShortcutFile -Destination $out -Force

$fullPath = $out | Resolve-Path

$sh = New-Object -ComObject WScript.Shell
$shortcut = $sh.CreateShortcut($fullPath)
$target = $shortcut.TargetPath
$arguments = $shortcut.Arguments
$working = $shortcut.WorkingDirectory

$out = New-Item -ItemType "directory" -Path $OutputPath -Name $name -Force | Resolve-Path

if ($IconLarge -and $CopyIcons) {
    echo "Copying large icon"
    Copy-Item -LiteralPath $IconLarge -Destination $out -Force
    $IconLarge = Split-Path -Path $IconLarge -Leaf
}

if ($IconSmall -and $CopyIcons) {
    echo "Copying small icon"
    Copy-Item -LiteralPath $IconSmall -Destination $out -Force
    $IconSmall = Split-Path -Path $IconSmall -Leaf
}

echo "Creating VBS launcher"
$out = [io.path]::combine($OutputPath, $name, "$name.vbs")
New-VbsLauncher -StartPath $working -AppTarget """""$target"""" $arguments" -OutputFile $out

$shortcut.TargetPath = $out
$shortcut.WorkingDirectory = $OutputPath | Resolve-Path
$shortcut.Arguments = ""
$shortcut.Save()

echo "Creating visual elements manifest"
if (!($IconSmall)) {
    $IconSmall = $IconLarge
}
$out = [io.path]::combine($OutputPath, $name, "$name.visualelementsmanifest.xml")
$params = @{
    "IconLarge"=$IconLarge;
    "IconSmall"=$IconSmall;
    "BackgroundColor"=$BackgroundColor;
    "UseDarkForegroundText"=$UseDarkForegroundText;
    "ShowName"=$ShowName
}
New-Object -TypeName PSObject -Prop $params | New-VisualElementsManifest -OutputFile $out

$out = Join-Path -Path $OutputPath -ChildPath "$name.lnk"
Move-Item $fullPath -Destination $out -Force

Edit-ShortcutLastWrite $out