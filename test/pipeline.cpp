#include "gtest/gtest.h"
#include "imageProcessing/imageProcessingPipeline.h"
#include "utils/common.h"
#include <unistd.h>

TEST(pipeline, smoke_test) {
    ImageQueue q;
    ImageProcessingPipeline pipeline(q);

    pipeline.run();
    sleep(1);
    pipeline.stop();
}