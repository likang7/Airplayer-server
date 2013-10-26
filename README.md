Airplayer_server
================

A video-on-demand platform on Linux and Android</br>
A project to help watch the PC’s video on phone via WIFI! </br>
The needed information can be transferred through clients based on FFMPEG/Live555/RTSP, </br>
and be converted to fit the Android device in the fly at the same time. </br>

Dependency Information
======================
FFmpeg  
git://source.ffmpeg.org/ffmpeg.git  
enable-libx264 needed

Live555  
http://www.live555.com/liveMedia  

Installation
============
make

Usage
=====
./apserver cp portNumber pathname ethname  
cp : the path of the content file  
portNumber : the port number of airPlayerServer  
ethname : the eth of the shared network   

default setting : ./apserver . 8188 /usr/bin eth0 </br>