#pragma once

#include <vector>
#include <string>
#include <map>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <boost/lockfree/spsc_queue.hpp>

#include "utils/timer.h"
#include "utils/common.h"

using namespace std;

/**
 * Class that manages multiple cameras and their properties.
 * 
 * Note that cameras are released when not selected to conserve bandwidth.
 */
class CameraArray
{
public:
    /**
     * Construct camera array using devices.
     *
     * @param devices List of device paths ["/dev/video0", "/dev/video1"]
     */
    CameraArray(vector<string> const devices, ImageQueue &queue);

    /**
     * Construct camera array using device indices.
     *
     * @param devices List of device indices
     */
    CameraArray(vector<int> const devices, ImageQueue &queue);

    /**
     * Sets a property for all cameras to a certain value.
     *
     * @see https://docs.opencv.org/3.4/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
     */
    void setCameraProperty(cv::VideoCaptureProperties cameraProperty, float value);

    float getCameraProperty(cv::VideoCaptureProperties cameraProperty);

    /**
     * Helper function to set the resolution of the camera.
     * This can be done using setCameraProperty as well.
     */
    void setResolution(int width, int height);

    /**
     * Gets a frame from the selected camera index.
     */
    cv::Mat getFrame();

    void startImageCapture();

    void stopImageCapture();

    void selectCamera(uint index);

    int getSelectedCamera()
    {
        return selectedCamera;
    }

private:
    vector<string> devicePaths;
    cv::VideoCapture camera;
    Timer timer;

    // Images are added here when ever capture is triggered.
    ImageQueue &imageQueue;

    int selectedCamera;

    // Map containing camera properties to set for all cameras.
    map<int, float> cameraProperties;
    mutex cameraMutex;
    
    void triggerImageCapture();
};