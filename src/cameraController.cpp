#include "cameraController.h"
#include "videoStream.h"

void setCameraDefaults(CameraArray &cameraArray)
{
    cameraArray.setResolution(1600, 1200);
    cameraArray.setCameraProperty(cv::CAP_PROP_BRIGHTNESS, 50);
    cameraArray.setCameraProperty(cv::CAP_PROP_HUE, 0);
    cameraArray.setCameraProperty(cv::CAP_PROP_CONTRAST, 20);
    cameraArray.setCameraProperty(cv::CAP_PROP_FPS, 30);
    cameraArray.setCameraProperty(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
}

CameraController::CameraController(vector<string> devicePaths) : imageQueue(),
                                                                 cameraArray(devicePaths, imageQueue),
                                                                 imagePipeline(imageQueue)
{
    setCameraDefaults(cameraArray);
}

CameraController::CameraController(vector<int> cameraIndex) : imageQueue(),
                                                              cameraArray(cameraIndex, imageQueue),
                                                              imagePipeline(imageQueue)
{
    setCameraDefaults(cameraArray);
}

void CameraController::startImageCapture()
{
    // Stop Video Capture
    stopVideoStream();
    cameraArray.startImageCapture();
    imagePipeline.run();
}

void CameraController::stopImageCapture()
{
    cameraArray.stopImageCapture();
    imagePipeline.stop();
}

void CameraController::startVideoStream()
{
    stopImageCapture();
    this->streamPipe = startStream(this->settings);
}

void CameraController::stopVideoStream()
{
    if (!this->streamPipe)
    {
        return;
    }
    endStream(this->streamPipe);
    this->streamPipe = nullptr;
}

void CameraController::selectCamera(int index)
{
    cameraArray.selectCamera(index);
    settings.cameraName = cameraArray.getSelectedCamera();
}

void CameraController::setResolution(int width, int height)
{
    cameraArray.setResolution(width, height);
    string resolution;
    resolution.append(to_string(width));
    resolution.append("x");
    resolution.append(to_string(height));
    settings.frameSize = resolution;
}

void CameraController::setFrameRate(int rate) 
{
    cameraArray.setCameraProperty(cv::CAP_PROP_FPS, rate);
    settings.framerate = to_string(rate);
}

void CameraController::setIPAddress(string IP, string Port){
    string IPAddress;
    IPAddress.append("udp://");
    IPAddress.append(IP);
    IPAddress.append(":");
    IPAddress.append(Port);
    IPAddress.append("?pkt_size=1316");
    settings.address = IPAddress;
}

ImageProcessingPipeline &CameraController::getImagePipeline()
{
    return imagePipeline;
}

void CameraController::setCameraProperty(cv::VideoCaptureProperties prop, float value)
{
    cameraArray.setCameraProperty(prop, value);
}