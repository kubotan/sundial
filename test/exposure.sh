#!/bin/bash

# */1 * * * * bash /root/exposure.sh
# v4l2-ctl -d /dev/video0 --list-ctrls-menu

startOfDay=`date "+%Y/%m/%d"`
timestampStartOfDay=`date +%s -d $startOfDay`
isDay=`/usr/local/bin/sundial 35.690 139.692 $timestampStartOfDay`
elapsedTime=$(expr $(date +%s) - $(date +%s -r ./exposure.pre))

if [ $isDay -eq `cat ./exposure.pre` -o $elapsedTime -lt 1800 ]
then
    exit
fi
echo $isDay > ./exposure.pre

if [ $isDay -eq 0 ]
then
    # night

    # mjpg-streamer1
    v4l2-ctl -d /dev/video0 -c auto_exposure=1   # 0: Auto Mode 1: Manual Mode default=0
    v4l2-ctl -d /dev/video0 -c exposure_time_absolute=10000   # min=1 max=10000 step=1 default=1000

    # mjpg-streamer2
    #v4l2-ctl -d /dev/video0 -c exposure_auto=1   # min=78 max=1250 step=1 default=312
    #v4l2-ctl -d /dev/video0 -c exposure_absolute=1250   # 1: Manual Mode 3: Aperture Priority Mode default:3
    #v4l2-ctl -d /dev/video2 -c exposure_auto=1   # min=78 max=1250 step=1 default=312
    #v4l2-ctl -d /dev/video2 -c exposure_absolute=1250   # 1: Manual Mode 3: Aperture Priority Mode default:3
else
    # day

    # mjpg-streamer1
    v4l2-ctl -d /dev/video0 -c exposure_time_absolute=1000   # min=1 max=10000 step=1 default=1000
    v4l2-ctl -d /dev/video0 -c auto_exposure=1   # 0: Auto Mode 1: Manual Mode default=0

    # mjpg-streamer2
    #v4l2-ctl -d /dev/video0 -c exposure_absolute=312   # min=78 max=1250 step=1 default=312
    #v4l2-ctl -d /dev/video0 -c exposure_auto=3   # 1: Manual Mode 3: Aperture Priority Mode default:3
    #v4l2-ctl -d /dev/video2 -c exposure_absolute=312   # min=78 max=1250 step=1 default=312
    #v4l2-ctl -d /dev/video2 -c exposure_auto=3   # 1: Manual Mode 3: Aperture Priority Mode default:3
fi
