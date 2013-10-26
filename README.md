Airplayer_server
================

A video-on-demand platform on Linux and Android</br>
A project to help watch the PC’s video on phone via WIFI! </br>
The needed information can be transferred through clients based on FFMPEG/Live555/RTSP, </br>
and be converted to fit the Android device in the fly at the same time. </br>

Dependency Information
======================
FFmpeg </br>
git://source.ffmpeg.org/ffmpeg.git</br>
enable-libx264 needed</br>

Live555</br>
http://www.live555.com/liveMedia/</br>

Installation
============
make

Usage
=====
./apserver cp portNumber pathname ethname  </br>
cp : the path of the content file </br>
portNumber : the port number of airPlayerServer </br>
ethname : the eth of the shared network </br>

default setting : ./apserver . 8188 /usr/bin eth0 </br>