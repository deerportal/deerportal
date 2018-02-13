#!/bin/sh
debian/deerportal/opt/deerportal/deerportal --test &
sleep 1
import -window root ss.png
