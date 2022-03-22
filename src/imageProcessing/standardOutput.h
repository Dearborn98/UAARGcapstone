#pragma once

#include "imageProcessing/imageProcessingPipeline.h"
#include "opencv2/core.hpp"

class FileSystemOutput : public ImageProcessingOutput {
    void operator() (cv::Mat in);
};