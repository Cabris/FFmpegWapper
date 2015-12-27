how to set environment
download  "ffmpeg-20140806-git-305f72a-win32-dev.7z",
"ffmpeg-20140806-git-305f72a-win32-shared.7z"
from http://ffmpeg.zeranoe.com/builds/
unzip them.

import ffmpeg:
in ffmpeg-20140806-git-305f72a-win32-dev (*.lib,*.h)
avcodec.lib
avdevice.lib
avfilter.lib
avformat.lib
avutil.lib
postproc.lib
swresample.lib
swscale.lib
place them at C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib
place all the include header folders place at C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include

in ffmpeg-20140806-git-305f72a-win32-shared (*.dll)
place all contained dlls at the projrct output folder

#the files downloaded from ffmpeg.zeranoe.com may not be "*20140806*" cause been updated. 
