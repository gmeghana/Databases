#!/bin/sh

grep 'local-infile=1' /etc/mysql/my.cnf > /dev/null
if [[ "$?" -eq "0" ]]; then
    echo "LOAD DATA LOCAL is already enabled."
    exit 1
fi

sudo patch /etc/mysql/my.cnf <<EOF
--- /etc/mysql/my.cnf	2014-02-19 13:18:21.000000000 -0800
+++ ../my.cnf	2015-10-09 22:01:35.395587715 -0700
@@ -106,6 +106,7 @@
 # ssl-ca=/etc/mysql/cacert.pem
 # ssl-cert=/etc/mysql/server-cert.pem
 # ssl-key=/etc/mysql/server-key.pem
+local-infile=1
 
 
 
@@ -115,6 +116,7 @@
 max_allowed_packet	= 16M
 
 [mysql]
+local-infile=1
 #no-auto-rehash	# faster start of mysql but no tab completition
 
 [isamchk]
EOF

echo "MySQL configuration has changed. Reboot your virtual machine."