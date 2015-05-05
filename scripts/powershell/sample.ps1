.\backup.ps1 -Location .\test -BackupDir ".\test dest" *>> test.log
.\archive.ps1 -Location .\test -Archive test *>> test.log
"Done"
exit