#include "standardOutput.h"
#include <ctime>
#include <string>
#include <stdio.h>
#include <opencv2/imgcodecs.hpp>

using namespace std;
void FileSystemOutput::operator()(cv::Mat in)
{
    char buffer[60];
    time_t now = time(0);
    tm *ltm = localtime(&now);
    strftime(buffer,60,"%b-%d-%G_%H-%M-%S.jpg", ltm);
    cv::imwrite(buffer, in);
}