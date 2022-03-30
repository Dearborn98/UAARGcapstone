#include "standardOutput.h"
#include <ctime>
#include <string>
#include <stdio.h>
#include <opencv2/imgcodecs.hpp>
#include "boost/log/trivial.hpp"

using namespace std;
void FileSystemOutput::operator()(cv::Mat in)
{
    char buffer[60];
    time_t now = time(0);
    tm *ltm = localtime(&now);
    strftime(buffer,60,"%b-%d-%G_%H-%M-%S.jpg", ltm);
    BOOST_LOG_TRIVIAL(info) << "Saving image to: " << buffer;
    cv::imwrite(buffer, in);
}