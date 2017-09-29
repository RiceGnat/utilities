# Logging functions
function Timestamp($message) {
    return '[' + (Get-Date -Format "yyyy-MM-dd HH:mm:ss") + '] ' + $message
}

function Log($message) {
    Write-Output (Timestamp $message)
}

function LogVerbose($message) {
    Write-Verbose (Timestamp $message)
}

function LogDebug($message) {
    Write-Debug (Timestamp $message)
}