#include <vector>
#include <string>
#include <map>

using namespace std;

/* ffmpeg -f v4l2 -i /dev/video8 -pix_fmt yuv420p -preset ultrafast -tune zerolatency
 -vcodec libx264 -r 30 -b:v 512k -s 640x360 -f mpegts -flush_packets 0 udp://127.0.0.1:5000 */

/**
 * Class that holds all the settings for the video Stream
 */
class StreamSettings
{

    StreamSettings();

    /**
     * @brief Construct a new Stream Settings object with a specified address
     * 
     * @param streamAddress Address that will be used for streaming Ex. udp://127.0.0.1:5000
     */
    StreamSettings(string streamAddress);

public:
    string cameraName = "/dev/video8"; 
    string pixelFormat = "yuv420p";
    string preset = "ultrafast";
    string quatlityTuning = "zerolatency";
    string encodingCodec = "libx264";
    string framerate = "30";
    string bitRate = "512k";
    string frameSize = "640x360";
    string format = "mpegts";
    string address = "udp://127.0.0.1:5000";

    void setFrameRate(int rate);
};

/**
 * Map to select the camera that will be streaming
 * 
 * Enter [v4l2-ctl --list-devices] in the console to find the correct camera names
 */
map<int, string> cameraNameOptions {};

/**
 * Map for Pixel Format Options
 * 
 * Enter [ffmpeg -pix_fmts] in the console for more potential optionss
 */
map<string, string> pixelFormatOptions {};

/**
 * Map for selecting the preset option
 * 
 * @see https://trac.ffmpeg.org/wiki/Encode/H.264
 */
map<string, string> presetOptions {};

/**
 * Map for selecting the tuning option
 * 
 * @see https://trac.ffmpeg.org/wiki/Encode/H.264
 */
map<string, string> tuningOptions {};

/**
 * Map for encoding option
 * 
 * Type [ffmpeg -encoders] in the console for more potential options
 */
map<string, string> encodingOptions {};

/**
 * @brief Takes the stream setting and appends them into one console command
 * 
 * @param settings Object holding the stream settings
 * @return The full command as a string
 */
string appendStreamCommand(StreamSettings settings);

/**
 * @brief Creates a thread and starts a video stream in the console using that thread
 * 
 * @param settings The object holding the stream settings
 * @return The pipe used for the stream
 * 
 * @see https://man7.org/linux/man-pages/man3/popen.3.html
 * @see https://stackoverflow.com/questions/44610978/popen-writes-output-of-command-executed-to-cout
 */
FILE* startStream(StreamSettings settings);


/**
 * @brief Closes the pipe that is running the stream
 * 
 * @param pipe The pipe of the current stream that will be closed
 */
void endStream(FILE* pipe);

/**
 * @brief Ends the stream and starts it again 
 * 
 * @param settings The object holding the stream settings
 * @param pipe The pipe that will be closed and reopened for the stream
 * @return The pipe used for the stream
 */
FILE* restartStream(StreamSettings settings, FILE* pipe);