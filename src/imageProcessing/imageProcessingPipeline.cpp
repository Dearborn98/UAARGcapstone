#include "imageProcessingPipeline.h"
#include "utils/common.h"

ImageProcessingPipeline::ImageProcessingPipeline(ImageQueue &queue) 
    : imageQueue(queue)
{
}

void ImageProcessingPipeline::registerFilter(unique_ptr<ImageProcessingFilter> filter) {
    filters.push_back(move(filter));
}

void ImageProcessingPipeline::registerOutput(unique_ptr<ImageProcessingOutput> output) {
    outputs.push_back(move(output));
}

void ImageProcessingPipeline::run() {
    cv::Mat frame;
    for (;;) {
        if (!imageQueue.pop(&frame)) {
            sleep(1); // Hardcode sleep before polling again
            continue;
        }

        // Process frame through all the filters.
        for (auto &filter : filters) {
            frame = (*filter)(frame);
        }

        // Output frame
        for (auto &output : outputs) {
            (*output)(frame);
        }
    }
}
