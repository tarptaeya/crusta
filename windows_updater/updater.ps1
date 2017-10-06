$webclient = New-Object System.Net.WebClient
$webclient.DownloadFile("http://crustabrowser.com/version/current.txt","new_version.txt")

Import-Module BitsTransfer

$currentfiletext=[System.IO.File]::ReadAllText("current.txt")
$newfiletext=[System.IO.File]::ReadAllText("new_version.txt")
$equal = $currentfiletext -ceq $newfiletext

if($equal -eq $false){
    $choice=(New-Object -ComObject Wscript.Shell).Popup('A new version of Crusta is available. Do you want to install new Crusta now?',0,'Crusta Updater',0x4)
    if ($choice -eq 6){
         Start-BitsTransfer http://crustabrowser.com/version/setup.exe setup.exe
         $run = (New-Object -ComObject Wscript.Shell).Popup('Downloaded setup file - Do you want to run it now?',0,'Crusta Updater',0x0)
         if($run -eq 1){
            setup.exe
         }
    }
}
