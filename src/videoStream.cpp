#include "videoStream.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <array>

using namespace std;

StreamSettings::StreamSettings(string streamAddress)
{
    this->address = streamAddress;
}

void StreamSettings::setFrameRate(int rate)
{
    this->framerate = to_string(rate);
}

map<int, string> cameraNameOptions = {
    {1 , "/dev/video8"},
    {2 , "/dev/video10"}
};

map<string, string> pixelFormatOptions = {
    {"420p" , "yuv420p"}
};

map<string, string> presetOptions = {
    {"ultra" , "ultrafast"},
    {"super" , "superfast"},
    {"very" , "veryfast"},
    {"faster" , "faster"},
    {"fast" , "fast"},
    {"med" , "medium"},
    {"slow" , "slow"},
    {"slower" , "slower"},
    {"veryslow" , "veryslow"}
};

map<string, string> tuningOptions = {
    {"fast" , "fastdecode"},
    {"zerolat" , "zerolatency"},
    {"film" , "film"},
};

map<string, string> encodingOptions = {
    {"H.264" , "libx264"},
    {"H.265" , "libx265"}
};

/* 
Default stream Command

ffmpeg -f v4l2 -i /dev/video8 -pix_fmt yuv420p -preset ultrafast -tune zerolatency
 -vcodec libx264 -r 30 -b:v 512k -s 640x360 -f mpegts -flush_packets 0 udp://127.0.0.1:5000
*/

string appendStreamCommand(StreamSettings settings)
{
    string str = "ffmpeg -f v4l2 -i ";
    str.append(settings.cameraName);
    str.append(" -pix_fmt ");
    str.append(settings.pixelFormat);
    str.append(" -preset ");
    str.append(settings.preset);
    str.append(" -tune ");
    str.append(settings.quatlityTuning);
    str.append(" -vcodec ");
    str.append(settings.encodingCodec);
    str.append(" -r ");
    str.append(settings.framerate);
    str.append(" -b:v ");
    str.append(settings.bitRate);
    str.append(" -s ");
    str.append(settings.frameSize);
    str.append(" -f ");
    str.append(settings.format);
    str.append(" -flush_packets 0 ");
    str.append(settings.address);

    return str;
}

FILE* startStream(StreamSettings settings) 
{
    string command = appendStreamCommand(settings);

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        cerr << "Failed to create a pipe for the stream" << endl;
    }
    
    return pipe;
}

void endStream(FILE* pipe)
{
    auto result = pclose(pipe);

    cout << result << endl;
}

FILE* restartStream(StreamSettings settings, FILE* pipe)
{
    endStream(pipe);
    pipe = startStream(settings);

    return pipe;
}