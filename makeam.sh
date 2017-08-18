#!/bin/sh

# Given a directory, produce a set of "git format-path" patches for
# each commit in the directory.
#

dir="$1"
new="$2"
patchdir=$(basename $dir)

X=0
for i in $(git log --reverse --oneline $dir | sed -e 's; .*;;'); do
	mkdir -p $patchdir
	git format-patch --stdout ${i}^..${i} | sed -e "s;$dir;$new;g" > $patchdir/$(printf %4.4d $X)-${i}.patch
	X=$(( $X + 1 ))
done
