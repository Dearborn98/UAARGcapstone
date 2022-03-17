#include "cameraArray.h"
#include <vector>
#include <boost/lockfree/spsc_queue.hpp>
#include <opencv2/core.hpp>

using namespace boost;

int main()
{
    lockfree::spsc_queue<cv::Mat, lockfree::capacity<128>> imageQueue;
    vector<int> cams = {8};
    CameraArray array(cams, imageQueue);
    array.startImageCapture();
    cv::Mat frame;
    int i = 0;
    for (;;) {
        i++;
        if (i ==  10) {
            array.stopImageCapture();
        }
        if (imageQueue.pop(&frame)) {
            cv::imwrite("image.jpg", frame);
        } else {
            printf("EMPTY\n");
        }
        sleep(1);
    }
    return 0;
}