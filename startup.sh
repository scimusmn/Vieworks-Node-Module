#######################################################
#! /bin/sh
echo "Delaying 30 seconds before Motion Lab startup."
sleep 30

echo "Starting Motion Lab."

cd /usr/local/src/motionLab
npm start
