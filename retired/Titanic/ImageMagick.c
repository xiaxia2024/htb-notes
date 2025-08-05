#!/bin/bash
timeout="10000"

scrot /tmp/screen.png
[[ -f ~/.config/i3/lock.png ]] &&
convert /tmp/screen.png -paint l -swirl 360 ~/.config/i3/lock.png -gravity center -composite -matte /temp/screen.png

mocp -P
mpc pause

i3lock -e -f -c 000000 -i /tmp/screen.png

while [[ $(pgrep -x i3lock) ]];do
  [[ $timeout -lt $(xssstate -i) ]] && xset dpms force off
  sleep 5
done

//跑网上学习magick2天
