#######################################################
#! /bin/sh
echo "Delaying 60 seconds before Motion Lab startup."
sleep 60

echo "Starting Motion Lab."

cd /usr/local/src/motionLab
npm start
