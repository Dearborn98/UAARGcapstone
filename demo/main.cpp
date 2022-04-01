#include <iostream>
#include <string>
#include <opencv2/core.hpp>

#include "cameraController.h"
#include "imageProcessing/standardFilter.h"
#include "imageProcessing/standardOutput.h"

#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/core.hpp"

using namespace std;

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

CameraController controller{vector<int>{8, 10}};

template <typename T>
T queryValue()
{
    cout << "Enter a value> ";
    float value;
    cin >> value;
    return value;
}

char inputLoop()
{
    string input;

    for (;;)
    {
        cout << "Select an option:" << endl;
        cout << "1. Start Image Capture." << endl;
        cout << "2. Start Video Capture." << endl;
        cout << "3. Stop all Captures." << endl;
        cout << "4. Toggle Edge Detection." << endl;
        cout << "5. Change Brightness" << endl;
        cout << "6. Select Camera" << endl;
        cout << endl;
        cout << "Input> ";
        cin >> input;
        if (input.size() == 0)
        {
            continue;
        }
        char c = input[0];
        if (c < '1' || c > '6')
        {
            continue;
        }
        return c;
    }
}

void toggleEdgeDetect()
{
    static bool enabled = false;

    auto &pipeline = controller.getImagePipeline();
    pipeline.clearFilters();

    if (!enabled)
    {
        pipeline.registerFilter(make_unique<EdgeDetectFilter>());
    }

    enabled = !enabled;
}

void redirectLogToFile()
{
    logging::add_file_log(keywords::file_name = "capstone.log", keywords::auto_flush = 1);
    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
}

int main()
{
    char input;
    int value, index;

    redirectLogToFile();
    controller.getImagePipeline().registerOutput(make_unique<FileSystemOutput>());

    for (;;)
    {
        input = inputLoop();

        switch (input)
        {
        case '1':
            controller.startImageCapture();
            break;
        case '2':
            controller.startVideoStream();
            break;
        case '3':
            controller.stopVideoStream();
            controller.stopImageCapture();
            break;
        case '4':
            toggleEdgeDetect();
            break;
        case '5':
            value = queryValue<float>();
            controller.setCameraProperty(cv::CAP_PROP_BRIGHTNESS, value);
            break;
        case '6':
            index = queryValue<int>();
            controller.selectCamera(index);
            break;
        }
    }

    return 0;
}