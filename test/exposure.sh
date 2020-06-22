#!/bin/bash

# */30 * * * * bash /root/exposure.sh
# v4l2-ctl -d /dev/video0 --list-ctrls-menu

OFFSET=$((30 * 60))
SUNDIAL=`./sundial 35.690 139.692`
SUNRISE=`echo $SUNDIAL | cut -d ',' -f 1`
SUNSET=`echo $SUNDIAL | cut -d ',' -f 2`
NOW=`date +%s`

#date "+%Y/%m/%d %H:%M:%S" --date @$NOW
#date "+%Y/%m/%d %H:%M:%S" --date @$(($SUNRISE + $OFFSET))
#date "+%Y/%m/%d %H:%M:%S" --date @$(($SUNSET - $OFFSET))

if [ $(($SUNRISE + $OFFSET)) -le $NOW -a $NOW -le $(($SUNSET - $OFFSET)) ]
then
  isDay=1
else
  isDay=0
fi

if [ $isDay -eq `cat ./exposure.pre` ]
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
    v4l2-ctl -d /dev/video0 -c exposure_time_absolute=50   # min=1 max=10000 step=1 default=1000
    v4l2-ctl -d /dev/video0 -c auto_exposure=1   # 0: Auto Mode 1: Manual Mode default=0

    # mjpg-streamer2
    #v4l2-ctl -d /dev/video0 -c exposure_absolute=312   # min=78 max=1250 step=1 default=312
    #v4l2-ctl -d /dev/video0 -c exposure_auto=3   # 1: Manual Mode 3: Aperture Priority Mode default:3
    #v4l2-ctl -d /dev/video2 -c exposure_absolute=312   # min=78 max=1250 step=1 default=312
    #v4l2-ctl -d /dev/video2 -c exposure_auto=3   # 1: Manual Mode 3: Aperture Priority Mode default:3
fi
