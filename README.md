# FFmpeg Wrapper
Minimalistic ffmpeg wrapper for frame size / video bitrate / subtitles managing

This app is just a simple GUI wrapper around FFmpeg executables. It lets you have no worries about writing long CLI commands for performing such tasks as:
* Changing frame size and bitrate for a video stream (using libx264 encoding)
* Managing embedded subtitles (adding new UTF-encoded SRT streams and deleting existing ones)

<p align="center">
  <img width="460" height="366" src="https://i.imgur.com/9zUIFlM.png">
</p>

## Using
FFmpeg Wrapper functionality heavily relies on ffmpeg executables, so they should be installed in a system.

#### Linux
On Linux machines you can build ffmpeg from sources, which can be obtained from official website (https://www.ffmpeg.org/download.html), or download the binary package from a repository, for example, if you are using Ubuntu:
```
sudo apt-get install ffmpeg
```

#### Windows
On Windows it's easier to download precompiled binaries [from here](https://ffmpeg.zeranoe.com/builds/).
Note that on Windows machine the app will be looking for the ffmpeg executables in a folder ```bin``` next to ```ffmpeg-wrapper.exe```. 
