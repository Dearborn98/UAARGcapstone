#include "cameraArray.h"

#include <stdexcept>
#include <string>
#include <boost/log/trivial.hpp>

#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <fcntl.h>

CameraArray::CameraArray(vector<string> const devices, ImageQueue &queue)
    : devicePaths(),
      // TODO: Make timer delay customizable
      timer(std::bind(&CameraArray::triggerImageCapture, this), 500),
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
}

CameraArray::CameraArray(vector<int> const devices, ImageQueue &queue)
    : devicePaths(),
      timer(std::bind(&CameraArray::triggerImageCapture, this), 3000),
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
}

void CameraArray::setCameraProperty(cv::VideoCaptureProperties cameraProperty, float value)
{
    lock_guard<mutex> guard(cameraMutex);
    bool cameraOpen = camera.isOpened();
    if (!cameraOpen) {
        camera = cv::VideoCapture(devicePaths[selectedCamera], cv::CAP_V4L2);
    }
    camera.set(cameraProperty, value);
    cameraProperties[cameraProperty] = value;
    if (!cameraOpen) {
        camera.release();
    }
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
    camera.read(frame);
    return frame;
}

void CameraArray::startImageCapture()
{
    selectCamera(selectedCamera);
    timer.start();
}

void CameraArray::stopImageCapture()
{
    timer.stop();
    deselectCamera(); // Free up camera for other tasks.
}

void CameraArray::triggerImageCapture()
{
    cv::Mat frame = getFrame();
    if (frame.empty()) {
        // Occurs if the camera is not yet ready. (errno 11: Resource Temp. Unavailable)
        BOOST_LOG_TRIVIAL(warning) << "Empty frame received. Dropping.";
        return;
    }
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
    
    // Preserve Timer State
    bool isTimerRunning = timer.isRunning();
    if (isTimerRunning) {
        timer.stop();
    }

    lock_guard<mutex> guard(cameraMutex);

    if (devicePaths.size() <= index)
    {
        string error = "Invalid camera index when selecting. Number of cameras: " + std::to_string(devicePaths.size());
        throw runtime_error(error);
    }
    selectedCamera = index;
    camera = cv::VideoCapture(devicePaths[index], cv::CAP_V4L2);

    if (!camera.isOpened()) {
        throw runtime_error("Failed to open camera");
    }

    // Carry over previous set camera properties.
    for (auto optionPair : cameraProperties)
    {
        BOOST_LOG_TRIVIAL(info) << "Reloading Property: " << optionPair.first << " : " << optionPair.second;
        camera.set(optionPair.first, optionPair.second);
    }

    if (isTimerRunning) {
        timer.start();
    }
}

void CameraArray::deselectCamera() {
    camera.release();
}