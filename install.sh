#!/bin/bash
if [ -f liczbyPierwszeFinder]
	./compile.sh	

cp ./liczbyPierwszeFinder /usr/bin/
chmod +x /usr/bin/liczbyPierwszeFinder

touch liczbyPierwsze.desktop
echo "[Desktop Entry]
Type=Application
Name=Liczby Pierwsze Finder
Terminal=true
Exec=/usr/bin/liczbyPierwszeFinder" >> liczbyPierwsze.desktop

cp ./liczbyPierwsze.desktop /usr/share/applications
rm ./liczbyPierwsze.desktop
