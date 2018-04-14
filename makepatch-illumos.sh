#!/bin/sh

files=$(git show --name-status $1 | grep '^[A-Z]	' | sed 's;^..;;')
for file in $files; do
	git show "$1" -- $file > $file.patch
done
patchfiles=$(echo $files | tr ' ' '\012' | sed -e 's;$;.patch;')
tar cf patch.tar $patchfiles
