#!/bin/sh
ctags -R -f - * | grep -v EXPORT_SYMBOL > tags
