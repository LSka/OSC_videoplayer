# OSC_videoplayer
made with OpenFrameworks 0.10.1

this branch has some fixes to run on Raspberry Pi and uses [ofxOMXPlayer](http://https://github.com/jvcleave/ofxOMXPlayer "ofxOMXPlayer") for playing videos

tested on Raspbian Buster on a Raspberry Pi 3 model B V2

# Settting up
- you need to install the [ofxOMXPlayer](http://https://github.com/jvcleave/ofxOMXPlayer "ofxOMXPlayer") addon (remember to run the *install_depends.sh* script inside the ofxOMXPlayer directory
- you may have problems installing all the dependencies for openFrameworks. You should substitute "libgl1-mesa-dev" with "libglvnd-dev" inside */home/pi/openFrameworks/scripts/linux/debian/install_dependencies.sh*
