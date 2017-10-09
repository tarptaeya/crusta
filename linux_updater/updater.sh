# this file may contains some syntax errors
# and is not complete, this file will be complete
# only when .deb of crusta is compiled.

wget http://crustabrowser.com/version/current.txt

zenity --question --title 'Crusta Updater' --text 'A new version of Crusta is available.\nDo you want to install new version of Crusta now?'
if [ $? -eq 0 ]
then
    while [ true ]
    do
        wget http://crustabrowser.com/version/setup.deb
        if [ $? -eq 0 ]
        then
            zenity --question --title 'Crusta Updater' --text 'Download of setup.deb finished.\nDo you want to run the installer now?'
            if [ $? -ne 0 ]
            then
                echo 'run setup.deb'
            fi
            break
        else
            zenity --question --title 'Crusta Updater' --text 'Download of setup.deb Interrupted.\nDo you want to retry now?'
            if [ $? -ne 0 ]
            then
                break
            fi
        fi
    done
fi
 
rm current.txt
rm setup.deb
