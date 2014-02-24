#!/bin/sh
abend() {
	echo hunkextract: $*
	exit 1
}

helper="$(dirname $(realpath $0))/hunkextract"
checkout="$1"
[ -s "$checkout" ] || abend "checkout file \"$checkout\" does not exist"
rej="$1.rej"
[ -s "$rej" ] || abend "rej file \"$rej\" does not exist"

$helper < "$rej" | grep -v '^+' | sed -e 's;^.;;'  > hunk.old
$helper < "$rej" | grep -v '^-' | sed -e 's;^.;;'  > hunk.new
$helper < "$checkout" > hunk.checkout
