#!/bin/bash
#
# Quick and easy script to add and commit changes in ditz,
# check if connected to dudley, and if so push changes to
# dudley.

WGET="/usr/bin/wget"
GIT="/usr/bin/git"

## Commit  with message "ditz update"
cd $($GIT rev-parse --show-cdup) # git root then .dizt/
cd .ditz
$GIT add .
$GIT commit . -m "ditz update"
cd ..

## Test if we can reach dudley
$WGET -q --tries=5 --timeout=5 http://dudley.dhcp.meraka.csir.co.za/ -O /tmp/index.dudley &> /dev/null
if [ -s /tmp/index.dudley ]
then
    $GIT push origin  # we are connected, push to dudley
fi