#!/usr/bin/expect -f
spawn scp /cygdrive/c/PROJECTS/samp-server-awesome-gamemode/awesome-freeroam.amx root@194.87.236.51:/home/dimka/samp03/gamemodes
expect "password:"
send "OfqpmaJ4\r\n"
interact
