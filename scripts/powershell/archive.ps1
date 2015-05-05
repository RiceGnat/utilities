[CmdletBinding()]
param (
    # Directory to be archived
    [Parameter(Mandatory=$True)]
    [string]$Location,

    # Archive
    [Parameter(Mandatory=$True)]
    [string]$Archive
)

# Import logging functions
. .\logging.ps1

# Location must be a directory
if ((Get-Item $Location) -is [System.IO.DirectoryInfo]) {
    LogDebug 'Location found'
}
else {
    Write-Error 'Location does not exist or is not a directory'
    exit
}

# Get absolute path
$Location = Get-Item $Location

# Begin
$begin = Get-Date

# Check if archive exists
$exists = $false
Get-Item ($Archive + '*') -ErrorAction SilentlyContinue |
foreach {
    if ($_.Name -match ".*\.7z$") {
        LogDebug 'Archive exists'
        $exists = $true
        $Archive = $_.FullName
    }
}

# Archive directory contents
Log ('Archiving contents of "' + $Location + '"')
7za a $Archive ($Location + '\*') -mx9 -y |
foreach {
    if (!($_ -match "^\s*$")) { LogVerbose $_ }
}

if ($?) {
    $result = if ($exists) { 'Updated' } else { 'Created' }

    if (!$exists) {
        Get-Item ($Archive + '*') -ErrorAction SilentlyContinue |
        foreach {
            if ($_.Name -match ".*\.7z$") {
                $Archive = $_.FullName
            }
        }
    }

    Log ($result + ' archive "' + $Archive + '"')
}
else {
    Log ('Failed to archive directory!')
}

$end = Get-Date
Log ('Elapsed time: ' + (New-TimeSpan -Start $begin -End $end))