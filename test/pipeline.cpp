#include "gtest/gtest.h"
#include "imageProcessing/imageProcessingPipeline.h"
#include "utils/common.h"
#include <unistd.h>

// Mock Filters and Outputs
class MockFilter : public ImageProcessingFilter
{
public:
    int num_called;
    cv::Mat operator()(cv::Mat in)
    {
        num_called++;
        return in;
    }
};

class MockOutput : public ImageProcessingOutput
{
public:
    int num_called;
    void operator()(cv::Mat in)
    {
        (void)in; // Prevent unused parameter warning
        num_called++;
    }
};

TEST(pipeline, smoke_test)
{
    ImageQueue q;
    ImageProcessingPipeline pipeline(q);

    pipeline.run();
    sleep(1);
    pipeline.stop();
}

TEST(pipeline, filter) {
    ImageQueue q;
    ImageProcessingPipeline pipeline(q);

    auto filter = new MockFilter(); // Will be deleted by unique ptr below
    pipeline.registerFilter(unique_ptr<ImageProcessingFilter>(filter));

    pipeline.run();
    q.push(cv::Mat());
    sleep(1);
    pipeline.stop();

    EXPECT_EQ(filter->num_called, 1);
}

TEST(pipeline, output) {
    ImageQueue q;
    ImageProcessingPipeline pipeline(q);

    auto output = new MockOutput(); // Will be deleted by unique ptr below
    pipeline.registerOutput(unique_ptr<ImageProcessingOutput>(output));

    pipeline.run();
    q.push(cv::Mat());
    sleep(1);
    pipeline.stop();

    EXPECT_EQ(output->num_called, 1);
}