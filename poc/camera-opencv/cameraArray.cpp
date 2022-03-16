#include "cameraArray.h"

#include <stdexcept>
#include <string>

CameraArray::CameraArray(vector<string> const devices)
{
    for (string path : devices)
    {
        cv::VideoCapture camera(path);

        if (!camera.isOpened())
        {
            string error = "Cannot open camera " + path;
            throw std::runtime_error(error);
        }

        cameraArray.push_back(camera);
    }
}

CameraArray::CameraArray(vector<int> const devices)
{
    vector<string> device_paths;
    for (int device : devices)
    {
        device_paths.push_back("/dev/video" + to_string(device));
    }

    for (string path : device_paths)
    {
        cv::VideoCapture camera(path);

        if (!camera.isOpened())
        {
            string error = "Cannot open camera " + path;
            throw std::runtime_error(error);
        }

        cameraArray.push_back(camera);
    }
}

void CameraArray::setCameraProperty(cv::VideoCaptureProperties cameraProperty, float value)
{
    for (cv::VideoCapture camera : cameraArray)
    {
        camera.set(cameraProperty, value);
    }
}

float CameraArray::getCameraProperty(cv::VideoCaptureProperties cameraProperty){
    cv::VideoCapture camera = cameraArray.front();
    return camera.get(cameraProperty);
}

cv::Mat CameraArray::getFrame(int cameraIndex) {
    cv::Mat frame;
    cameraArray[cameraIndex] >> frame;
    return frame;
}