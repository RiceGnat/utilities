[CmdletBinding()]
param (
    # Directory to be backed up
    [Parameter(Mandatory=$True)]
    [string]$Location,

    # Backup directory
    [Parameter(Mandatory=$True)]
    [string]$BackupDir
)

# Import logging functions
. .\logging.ps1

# Save current path
$path = Convert-Path .

# Get date
$date = Get-Date -Format "yyyy-MM-dd"

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
Log ('Beginning backup for "' + $Location + '"');
$begin = Get-Date

# Check if backup directory exists and create if not
if ((Get-Item $BackupDir -ErrorAction SilentlyContinue) -is [System.IO.DirectoryInfo]) {
    LogDebug 'Backup directory exists'
}
else {
    LogDebug 'Backup path does not exist or is not a directory'

    Log ('Created "' + (New-Item $BackupDir -ItemType Directory) + '"')
}

# Get absolute backup path
$BackupDir = Get-Item $BackupDir

# Navigate to backup directory
Set-Location $BackupDir

# Copy files
Log ('Copying items to "' + $BackupDir + '"')
$count = 0
Copy-Item $Location -Force -Recurse -PassThru |
foreach {
    LogVerbose ('Copied "' + $_.FullName + '"')
    $count++
}
$plural = if ($count -eq 1 ) { '' } else { 's' }
Log ('Copied ' + $count + ' item' + $plural)

# Archive directory contents
Log ('Archiving contents of "' + $Location + '"')
$name = Split-Path $Location -Leaf
$archive = $name + ' backup ' + $date;
7za a $archive ($Location + '\*') -mx9 -y |
foreach {
    if (!($_ -match "^\s*$")) { LogVerbose $_ }
}
if ($?) {
    Log ('Created archive "' + $archive + '.7z"')
}
else {
    Log ('Failed to create archive!')
}

# Remove copied items
Log 'Deleting directory copy'
Remove-Item $name -Force -Recurse | Out-Null

# Remove old archives
Log 'Checking for archives older than 30 days'
$count = 0
Get-Item .\* -Filter ($name + ' backup*') |
foreach {
    if ($_.Name -match "\d{4}-\d{2}-\d{2}") {
        if ((New-TimeSpan -Start $matches[0] -End $date).Days -gt 30) {
            LogVerbose ('Deleting archive "' + $_.Name + '"')
            Remove-Item $_.FullName -Force | Out-Null
            $count++
        }
    }
}
if ($count -gt 0) {
    $plural = if ($count -eq 1 ) { '' } else { 's' }
    Log ('Deleted ' + $count + ' archive' + $plural)
}
else {
    Log ('No old archives found')
}

# Navigate back to original path (useful for directly running in shell)
Set-Location $path

# End
Log 'Backup complete'
$end = Get-Date
Log ('Elapsed time: ' + (New-TimeSpan -Start $begin -End $end))