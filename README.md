# FFmpegWapper
x86 is stable,do not use x64.
how to use:
this solution conatins projects that bulid required dll "FFmpegCppWrapper.dll" 
FFmpegCppWrapper is the building project.
FFmpegCppWrapper.dll should be located in "Debug" folder outside the FFmpegCppWrapper folder.
read readme.txt in FFmpegCppWrapper for environment setting.

WrapperTestCpp
WrapperTestCs
are demos show how to call the functions in "FFmpegCppWrapper.dll" implemented with cpp and c#
it will produce a video file with h264 encode.
make sure "FFmpegCppWrapper.dll" is copy to their output folder

StreamingDemoCpp
StreamingDemoCs
are Streaming server implemented in cpp and c#, make sure "FFmpegCppWrapper.dll" is copy to their output folder.
it will not produce a video file with h264 encode,instead it will streaming video stream via LAN 
and deisplay on an android app of another project "hoster".

hoster is in here 
https://github.com/Cabris/unity_lab/tree/new_hoster/hoster


