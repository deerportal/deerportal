#
# Regular cron jobs for the deerportal package
#
0 4	* * *	root	[ -x /usr/bin/deerportal_maintenance ] && /usr/bin/deerportal_maintenance
