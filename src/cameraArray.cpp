#include "cameraArray.h"

#include <stdexcept>
#include <string>
#include <boost/log/trivial.hpp>

CameraArray::CameraArray(vector<string> const devices, ImageQueue &queue)
    : timer(std::bind(&CameraArray::triggerImageCapture, this), 1000),
      imageQueue(queue)
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

CameraArray::CameraArray(vector<int> const devices, ImageQueue &queue)
    : timer(std::bind(&CameraArray::triggerImageCapture, this), 1000),
      imageQueue(queue)
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

float CameraArray::getCameraProperty(cv::VideoCaptureProperties cameraProperty)
{
    cv::VideoCapture camera = cameraArray.front();
    return camera.get(cameraProperty);
}

cv::Mat CameraArray::getFrame(int cameraIndex)
{
    cv::Mat frame;
    cameraArray[cameraIndex] >> frame;
    return frame;
}

void CameraArray::startImageCapture()
{
    timer.start();
}

void CameraArray::stopImageCapture()
{
    timer.stop();
}

void CameraArray::triggerImageCapture()
{
    cv::Mat frame = getFrame(0);
    if (!imageQueue.push(frame))
    {
        BOOST_LOG_TRIVIAL(warning) << "Queue is full. Dropping Frame.";
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Image added to queue.";
    }
}