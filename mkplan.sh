#!/bin/bash

if [ $# -lt 2 ]; then
    echo Not enough arguments.
    return 1
fi
tmp=/tmp/mkplan.sh-$RANDOM.html
while [ $# -gt 1 ]; do
    output+="$($1)"
    shift
done
outfile="$1"
parse_test.py <<< "$output" | format_plan.py > $tmp
wkhtmltopdf -q $tmp "$outfile"
rm $tmp
xdg-open "$outfile"
