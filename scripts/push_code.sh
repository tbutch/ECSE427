#!/bin/bash

usr=tbouch9
dest=mimi.cs.mcgill.ca
pwd=sWeLIXv_
destDir=/home/2020/tbouch9/src
currDir="$(pwd)"
parentdir="$(dirname "$currDir")"

showMenu() {
    echo "-------------------------------------------------------------"
    echo "                       MGCIL MACHINE"
    echo "-------------------------------------------------------------"
    echo "[1] push source folder to dest"
    echo "[4] exit"
    echo "-------------------------------------------------------------"

    read -p "option: " mc
    return $mc
}

while [[ "$m" != "4" ]]
do
    if [[ "$m" == "1" ]]; then
        echo "connecting to " $dest "..."
        echo "copying " "$parentdir" to "$usr"@"$dest:$destDir"
        sshpass -p "$pwd" scp -rq "$parentdir" $usr@$dest:$destDir
    fi
    showMenu
    m=$?

done



