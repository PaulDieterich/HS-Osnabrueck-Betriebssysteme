#!/bin/sh
#Paul Dieterich, Jan Weimer
[ ! -d ~/.trashBin ] && mkdir -p ~/.trashBin/ #überprüft ob .trashBin im home verzeichnis existerit,
					      #wenn nicht erstellt er ein .trashBin
[ ! -f ~/.trashBin/.dir ] && touch ~/.trashBin/.dir 	#schaut ob im ./trashBin eine datei namens .dir exisitert
							#wenn nicht erstellt er eine

zaehler=1
for var in $@ #vorschleife die durch alle Übergabeparameter geht.
do
	[ ! -f $var ] && echo "$var diese Datei ist nicht vorhanden"
	[ -d $var ] && echo "$var ist ein Verzeichnis und kann nicht gelöscht erden" 2>/dev/null && break
	pID=$$ #gibt die prozessID zurück
	datum=$(date '+%y%m%d%H%M%S') #gibt darum zurück im angegebenden format
	rename=$datum"_"$pID"_"$zaehler".dat" #fügt alle drei Variablen zusammen
	echo $rename" "$(pwd)/$var >> ~/.trashBin/.dir #schreibt den namen und den pfad in die .dir
	mv $var $HOME/.trashBin/$rename #verschiebt die datei ins .trashBin verzeichnis und nennt die datei um
	zaehler=$((zaehler +1)) #zählt die zaehlervariable hoch
done


