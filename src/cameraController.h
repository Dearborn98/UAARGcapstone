#pragma once

#include "opencv2/opencv.hpp"
#include "imageProcessing/imageProcessingPipeline.h"
#include "cameraArray.h"
#include "videoStream.h"
#include "utils/common.h"
#include <string>

/**
 * @example demo/main.cpp
 * An exampled of how to use CameraController in the context of the capstone demo.
 */

/** 
 * Public API for interfacing with camera array and 
 * cotrolling video streaming and image captures.
 */
class CameraController
{
public:
    StreamSettings settings;
    FILE* streamPipe;

    /**
     * @param devicePath The string paths to the cameras.
     */
    CameraController(vector<string> devicePaths);

    /**
     * @param devicePath The index of the video cameras under /dev/video.
     */
    CameraController(vector<int> cameraIndex);

    /**
     * Start capturing images on the selected camera.
     * Stops any video capture on the selected camera if it is ongoing.
     */
    void startImageCapture();

    /**
     * Stop image capture on the selected camera.
     */
    void stopImageCapture();


    /**
     * Start ffmpeg video stream on the selected camera.
     * Stops any image capture on the selected camera if it is ongoing.
     */
    void startVideoStream();
    
    /**
     * Stop ffmpeg video capture on the selected camera.
     */
    void stopVideoStream();

    /**
     * Set the camera used for capture/streaming operations.
     */
    void selectCamera(int index);

    /**
     * Set the camera resolution for capture/streaming operations.
     */
    void setResolution(int width, int height);

    /**
     * Set the camera frameRate
     */
    void setFrameRate(int rate);

    /**
     * Set the IP address for video stream
     */
    void setIPAddress(string IP, string Port);

    /**
     * Set Camera Property using OpenCV's backend.
     * 
     * @param prop The enum for the property to set.
     * @param value The value to set the property. 
     */
    void setCameraProperty(cv::VideoCaptureProperties prop, float value);

    /**
     * Get the value of a Camera Property using OpenCV's backend.
     * 
     * @param prop The enum for the property to get.
     */
    float getCameraProperty(cv::VideoCaptureProperties prop);

    /**
     * Provides access to the imaging pipeline to modify processing
     * operations dynamically.
     * 
     * @see ImageProcessingPipeline
     */
    ImageProcessingPipeline& getImagePipeline();

private:

    // Members for image processing
    ImageQueue imageQueue;
    CameraArray cameraArray;
    ImageProcessingPipeline imagePipeline;
};