if((Test-Path setup.exe) -eq $true){
    Remove-Item setup.exe
}

$webclient = New-Object System.Net.WebClient
$webclient.DownloadFile("http://crustabrowser.com/version/current.txt","new_version.txt")

Import-Module BitsTransfer

$currentfiletext=[System.IO.File]::ReadAllText("current.txt")
$newfiletext=[System.IO.File]::ReadAllText("new_version.txt")
$equal = $currentfiletext -ceq $newfiletext

if($equal -eq $false){
    $webclient.DownloadFile("http://crustabrowser.com/version/release_notes.txt","releasenotes.txt")
    $releasenotestext = [System.IO.File]::ReadAllText("releasenotes.txt")
    $choice=(New-Object -ComObject Wscript.Shell).Popup('A new version of Crusta is available. Do you want to install new Crusta now?

Local version of Crusta   : '+$currentfiletext+'
Current version of Crusta : '+$newfiletext+'

New in this version :

'+$releasenotestext,0,'Crusta Updater',0x4)
    if ($choice -eq 6){
        while((Test-Path setup.exe) -eq $false){
            Start-BitsTransfer http://crustabrowser.com/version/setup.exe setup.exe
        }
        $run = (New-Object -ComObject Wscript.Shell).Popup('Downloaded setup file - Do you want to run it now?',0,'Crusta Updater',0x0)
        if($run -eq 1){
            # run uninstaller.exe
            #run setup.exe
        }
    }
}
