#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <opencv2/core.hpp>

typedef boost::lockfree::spsc_queue<cv::Mat, boost::lockfree::capacity<128>> ImageQueue;