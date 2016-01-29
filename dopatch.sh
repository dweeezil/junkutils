#!/bin/sh
dopatch() {
patchfile="$1"
base="$(basename $1)"
base="${base%.patch}"
gitls="$(git ls-files */$base)"
count="$(echo $gitls | wc -w)"

if [ "$count" -ne 1 ]; then
	echo too many files: $patchfile
	return 1
fi
echo patch "$gitls" "$patchfile"
}

for i in "$@"; do
	dopatch "$i"
done
