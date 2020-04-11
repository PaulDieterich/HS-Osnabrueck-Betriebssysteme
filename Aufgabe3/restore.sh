#!/bin/sh
#Paul Dieterich, Jan Weimer

#grep $1 ~/.trashBin/.dir - gibt die zeile zurück in der der übergabeparameter steht
#awk '{ print $2}' - gibt die zweite spalte der zeile zurück

function restore {
	if [ -f ~/.trashBin/$1 ]; then
		dateiPfad=$(grep $1 ~/.trashBin/.dir | awk '{ print $2}') #nimmt sich den pfad aus der .dir zum wieder hersstell
		pfadOhneDatei=$(echo $dateiPfad | grep -o '.*/')
		[ -d $pfadOhneDatei ] && mv ~/.trashBin/$1 $dateiPfad && echo "Datei wurde wieder hergestellt" || echo "Wieder Herstellungspfad kann konnte nicht gefunden werden"
		sed -i "/$1/d" ~/.trashBin/.dir
	else
		echo "$1 konnte nicht in ./trashBin gefunden werden" # fehlermeldung falls die datei nicht in ./trashBin gefunden wurde
	fi
}

function restorePerName {
	echo 'Es liegen mehrere Dateien in /.trashBin, Bitte wählen sie aus'
	options=$(grep $1 ~/.trashBin/.dir | awk '{print $1}')
	select opt in $options
	do
		case $opt in
			"$opt" ) restore $opt  break;;
		esac
	done
}

function restorePerID {
	for var in $@ #vorschleife die durch alle übergabeparameter geht.
	do
		echo $var
		restore $var
	done
}

for var in $@
do
#restorePerID $var
restorePerName $var
done
