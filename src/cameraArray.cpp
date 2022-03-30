#include "cameraArray.h"

#include <stdexcept>
#include <string>
#include <boost/log/trivial.hpp>

#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <fcntl.h>

CameraArray::CameraArray(vector<string> const devices, ImageQueue &queue)
    : devicePaths(),
      timer(std::bind(&CameraArray::triggerImageCapture, this), 1000),
      imageQueue(queue),
      selectedCamera(0)
{
    for (string path : devices)
    {
        // Check that device is a camera
        int fd = open(path.c_str(), O_RDWR | O_NONBLOCK);
        if (fd < 0)
        {
            string error = "Error opening camera at: " + path + ". " + strerror(errno);
            throw runtime_error(error);
        }
        int check = ioctl(fd, VIDIOC_QUERYCAP, V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING);
        close(fd);
        if (fd < 0)
        {
            string error = "Error closing camera at: " + path + ". " + strerror(errno);
            throw runtime_error(error);
        }

        if (check == EINVAL)
        {
            string error = "connected device cannot video capture or stream";
            throw runtime_error(error);
        }
        else
        {
            devicePaths.push_back(path);
            BOOST_LOG_TRIVIAL(info) << "Registered camera: " << path;
        }
    }

    selectCamera(0);
}

CameraArray::CameraArray(vector<int> const devices, ImageQueue &queue)
    : devicePaths(),
      timer(std::bind(&CameraArray::triggerImageCapture, this), 1000),
      imageQueue(queue),
      selectedCamera(0)
{
    vector<string> devicePathsToProccess;
    for (int device : devices)
    {
        devicePathsToProccess.push_back("/dev/video" + to_string(device));
    }

    for (string path : devicePathsToProccess)
    {
        // Check that device is a camera
        int fd = open(path.c_str(), O_RDWR | O_NONBLOCK);
        if (fd < 0)
        {
            string error = "Error opening camera at: " + path + ". " + strerror(errno);
            throw runtime_error(error);
        }
        int check = ioctl(fd, VIDIOC_QUERYCAP, V4L2_CAP_VIDEO_CAPTURE | V4L2_CAP_STREAMING);
        close(fd);
        if (fd < 0)
        {
            string error = "Error closing camera at: " + path + ". " + strerror(errno);
            throw runtime_error(error);
        }

        if (check == EINVAL)
        {
            string error = "connected device cannot video capture or stream";
            throw runtime_error(error);
        }
        else
        {
            devicePaths.push_back(path);
            BOOST_LOG_TRIVIAL(info) << "Registered camera: " << path;
        }
    }

    selectCamera(0);
}

void CameraArray::setCameraProperty(cv::VideoCaptureProperties cameraProperty, float value)
{
    lock_guard<mutex> guard(cameraMutex);
    camera.set(cameraProperty, value);
    cameraProperties[cameraProperty] = value;
}

float CameraArray::getCameraProperty(cv::VideoCaptureProperties cameraProperty)
{
    cv::VideoCapture camera = devicePaths.front();
    float value = camera.get(cameraProperty);
    cameraProperties[cameraProperty] = value;
    return value;
}

void CameraArray::setResolution(int width, int height)
{
    setCameraProperty(cv::CAP_PROP_FRAME_WIDTH, width);
    setCameraProperty(cv::CAP_PROP_FRAME_HEIGHT, height);
}

cv::Mat CameraArray::getFrame()
{
    lock_guard<mutex> guard(cameraMutex);
    cv::Mat frame;
    bool result = camera.read(frame);
    if (!result)
    {
        BOOST_LOG_TRIVIAL(error) << "Failed to grab a valid frame.";
    }
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
    cv::Mat frame = getFrame();
    if (!imageQueue.push(frame))
    {
        BOOST_LOG_TRIVIAL(warning) << "Queue is full. Dropping Frame.";
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Image added to queue.";
    }
}

void CameraArray::selectCamera(uint index)
{
    lock_guard<mutex> guard(cameraMutex);
    if (devicePaths.size() <= index)
    {
        string error = "Invalid camera index when selecting. Number of cameras: " + std::to_string(devicePaths.size());
        throw runtime_error(error);
    }
    selectedCamera = index;
    camera.release();
    camera = cv::VideoCapture(devicePaths[index]);

    // Carry over previous set camera properties.
    for (auto optionPair : cameraProperties)
    {
        BOOST_LOG_TRIVIAL(info) << "Reloading Property: " << optionPair.first << " : " << optionPair.second;
        camera.set(optionPair.first, optionPair.second);
    }
}