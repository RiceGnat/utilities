@echo off
if "%1"=="" goto default
set handler="%1"
goto keys

:default
set handler="%cd%\cortana-uri.exe"

:keys
echo Adding registry keys...
reg add HKEY_CLASSES_ROOT\cortana /ve /d "Cortana command URI" /f
reg add HKEY_CLASSES_ROOT\cortana /v "URL Protocol" /f
reg add HKEY_CLASSES_ROOT\cortana\DefaultIcon /ve /d \"%handler%\",1 /f
reg add HKEY_CLASSES_ROOT\cortana\shell\open\command /ve /d \"%handler%\"" %%1" /f