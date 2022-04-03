#include "standardFilter.h"

cv::Mat EdgeDetectFilter::operator()(cv::Mat in)
{

    cv::Mat img_gray;
    cv::cvtColor(in, img_gray, cv::COLOR_BGR2GRAY);
    // Blur the image for better edge detection
    cv::Mat img_blur;
    cv::GaussianBlur(img_gray, img_blur, cv::Size(3, 3), 0);

    cv::Mat edges;
    cv::Canny(img_blur, edges, 10, 20, 3, false);
    return edges;
}