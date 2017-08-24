@echo off
taskkill /IM crusta.exe && start %temp%\setup.exe /VERYSILENT
exit
