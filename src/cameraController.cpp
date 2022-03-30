#include "cameraController.h"

void setCameraDefaults(CameraArray &cameraArray) {
    cameraArray.setResolution(1920, 1080);
    cameraArray.setCameraProperty(cv::CAP_PROP_BRIGHTNESS, 50);
    cameraArray.setCameraProperty(cv::CAP_PROP_HUE, 0);
    cameraArray.setCameraProperty(cv::CAP_PROP_AUTO_EXPOSURE, 0);
    cameraArray.setCameraProperty(cv::CAP_PROP_CONTRAST, 20);
    cameraArray.setCameraProperty(cv::CAP_PROP_FPS, 5);
    cameraArray.setCameraProperty(cv::CAP_PROP_AUTOFOCUS, 1);
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
}

void CameraController::stopVideoStream()
{
}

void CameraController::selectCamera(int index)
{
    cameraArray.selectCamera(index);
}

ImageProcessingPipeline &CameraController::getImagePipeline()
{
    return imagePipeline;
}