#include "cameraArray.h"
#include "imageProcessing/imageProcessingPipeline.h"
#include "imageProcessing/standardFilter.h"
#include "imageProcessing/standardOutput.h"
#include <vector>
#include <boost/lockfree/spsc_queue.hpp>
#include <opencv2/core.hpp>

using namespace boost;

int main()
{
    lockfree::spsc_queue<cv::Mat, lockfree::capacity<128>> imageQueue;
    vector<int> cams = {8};
    CameraArray array(cams, imageQueue);
    ImageProcessingPipeline pipeline(imageQueue);
    pipeline.registerFilter(std::make_unique<EdgeDetectFilter>());
    pipeline.registerOutput(std::make_unique<FileSystemOutput>());
    pipeline.run();
    array.startImageCapture();
    for (;;)
    {
    }
    return 0;
}