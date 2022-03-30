#pragma once

#include "imageProcessing/imageProcessingPipeline.h"
#include "opencv2/opencv.hpp"

/**
 * Implements a stage that converts images to edges.
 */
class EdgeDetectFilter : public ImageProcessingFilter
{
public:
    cv::Mat operator()(cv::Mat in);
};