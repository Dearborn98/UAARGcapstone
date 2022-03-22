#ifndef imageProcessingPipeline_H
#define imageProcessingPipeline_H

#include <vector>
#include <atomic>
#include <opencv2/core.hpp>
#include <memory>
#include <thread>
#include "utils/common.h"
#include <boost/lockfree/spsc_queue.hpp>

using namespace std;

/**
 * Represents a stage in the image processing pipeline.
 */
class ImageProcessingFilter
{
public:
    virtual ~ImageProcessingFilter() = default;
    // Override in concrete implementations.
    virtual cv::Mat operator()(cv::Mat in) = 0;
};

/**
 * Represents an output location for processed matrices.
 */
class ImageProcessingOutput
{
public:
    virtual ~ImageProcessingOutput() = default;
    // Override in concrete implementations.
    virtual void operator()(cv::Mat) = 0;
};

class ImageProcessingPipeline
{
public:
    ImageProcessingPipeline(ImageQueue &queue);

    /**
     * Adds a new filter stage to the processing pipeline.
     *
     * @example
     * unique_ptr<ImageProcessingFilter> filter = unique_ptr<ImageProcessingFilter>(new EdgeDetectFilter());
     * pipeline.registerFilter(move(filter));
     */
    void registerFilter(unique_ptr<ImageProcessingFilter> filter);

    /**
     * Adds a new output location for processed matrices.
     *
     * @example
     * unique_ptr<ImageProcessingOutput> output = unique_ptr<ImageProcessingOutput>(new FileSystemOutput());
     * pipeline.registerOutput(move(output));
     */
    void registerOutput(unique_ptr<ImageProcessingOutput> outputs);

    void run();
    void stop();

private:
    vector<unique_ptr<ImageProcessingFilter>> filters;
    vector<unique_ptr<ImageProcessingOutput>> outputs;
    ImageQueue &imageQueue;
    thread pipelineThread;

    atomic<bool> running;

    void run_();
};

#endif