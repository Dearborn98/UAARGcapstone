#pragma once

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

using namespace std;

class CameraArray
{
public:
    /**
     * Construct camera array using devices.
     *
     * @param devices List of device paths ["/dev/video0", "/dev/video1"]
     */
    CameraArray(vector<string> const devices);

    /**
     * Construct camera array using device indices.
     *
     * @param devices List of device indices
     */
    CameraArray(vector<int> const devices);

    /**
     * Sets a property for all cameras to a certain value.
     *
     * @see https://docs.opencv.org/3.4/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
     */
    void setCameraProperty(cv::VideoCaptureProperties cameraProperty, float value);

    float getCameraProperty(cv::VideoCaptureProperties cameraProperty);

    cv::Mat getFrame(int cameraIndex);

private:
    vector<cv::VideoCapture> cameraArray;
};