#!/bin/bash
#
# Quick and easy script to add and commit changes in ditz,
# check if connected to dudley, and if so push changes to
# dudley.

# this is not working correctly, please help with cd command

WGET="/usr/bin/wget"
GIT="/usr/bin/git"
DITZ_ROOT=$($GIT rev-parse --show-cdup)/.ditz/   # git root then .ditz/

## Commit  with message "ditz update"
cd $DITZ_ROOT
$GIT add .
$GIT commit . -m "ditz update"
cd ..

## Test if we can reach dudley
$WGET -q --tries=5 --timeout=5 http://dudley.dhcp.meraka.csir.co.za/ -O /tmp/index.dudley &> /dev/null
if [ -s /tmp/index.dudley ]
then
    $GIT push origin  # we are connected, push to dudley
fi