#!/bin/bash

usr=tbouch9
dest=mimi.cs.mcgill.ca
pwd=sWeLIXv_
destDir=/home/2020/tbouch9/src
currDir="$(pwd)"
parentdir="$(dirname "$currDir")"

showMenu() {
    echo "-------------------------------------------------------------"
    echo "                       PUSH TO SERVER"
    echo "-------------------------------------------------------------"
    echo "[1] push source folder to dest"
    echo "[2] help"
    echo "[4] exit"
    echo "-------------------------------------------------------------"

    read -p "option: " mc
    return $mc
}

displayHelp() {
    echo "Configure the script destination and source in this script."
    echo "This script should be placed as follows:"
    echo "    ."
    echo "├── Assignment1"
    echo "│   ├── ..."
    echo "└── scripts"
    echo "    └── push_code.sh"
    echo ""
    echo "It copies the parent folder (aka the one containing both the /scripts"
    echo "and the /Assignment1 directories to the destination"
}

while [[ "$m" != "4" ]]
do
    if [[ "$m" == "1" ]]; then
        echo "connecting to " $dest "..."
        echo "copying " "$parentdir" to "$usr"@"$dest:$destDir"
        sshpass -p "$pwd" scp -rq "$parentdir" $usr@$dest:$destDir
    elif [[ "$m" == "2" ]]; then
        displayHelp
    fi
    #clear
    showMenu
    m=$?

done



