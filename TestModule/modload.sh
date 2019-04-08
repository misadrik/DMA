#!/bin/sh
module = "testmodule2"
device = "testmodule"
#mode = "664"

#invoke insmod with all arguments we got
#and use a pathname, as newer modutils don't look in . by default.
/sbin/insmod ./$module.ko $* || exit 1

# remove stale nodes
rm -f /dev/${device}
# rm -f /dev/${device}[0-3]

major = $(awk "\\$2= =\"$module\" {print \\ $1}" /proc/devices)

mknod /dev/${device} c $major 0
#mknod /dev/${device}1 c $major 1

# give appropriate group/permissions, and change the group.
# Not all distributions have staff, some have "wheel" instead.
# group = "staff"
# grep -q '^staff:' /etc/group || group= 'wheel'

# chgrp $group /dev/${device}[0-3]
# chmod $mode /dev/${device}[0-3]

# last few lines
# the script must be run by the superuser, so newly created special files are owned by root
# The permission bits defaullt so that only root has write access, whil anyone can get read access
# so these lines give access to a group of users
