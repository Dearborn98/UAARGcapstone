#include "imageProcessingPipeline.h"
#include "utils/common.h"
#include "boost/log/trivial.hpp"
#include <stdexcept>

ImageProcessingPipeline::ImageProcessingPipeline(ImageQueue &queue) 
    : imageQueue(queue)
{
}

void ImageProcessingPipeline::registerFilter(unique_ptr<ImageProcessingFilter> filter) {
    filters.push_back(move(filter));
    BOOST_LOG_TRIVIAL(info) << "Added filter " << typeid(filter).name();
}

void ImageProcessingPipeline::registerOutput(unique_ptr<ImageProcessingOutput> output) {
    outputs.push_back(move(output));
    BOOST_LOG_TRIVIAL(info) << "Added output " << typeid(output).name();
}

void ImageProcessingPipeline::run() {

    if (pipelineThread.joinable()) {
        throw runtime_error("Pipeline already running.");
    }
    running = true;
    pipelineThread = thread(&ImageProcessingPipeline::run_, this);
}

void ImageProcessingPipeline::stop() {
    if (!pipelineThread.joinable()) {
        throw runtime_error("Pipeline is not running.");
    }
    running = false;
    pipelineThread.join();
}

void ImageProcessingPipeline::run_(){
    cv::Mat frame;
    for (;;) {
        if (!running) {
            break;
        }

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

