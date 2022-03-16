#include "iostream"
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "cameraArray.h"

int main(int, char **)
{
    CameraArray cArray({8, 10});

    // Set some settings
    cArray.setCameraProperty(cv::CAP_PROP_BRIGHTNESS, 32);
    cArray.setCameraProperty(cv::CAP_PROP_HUE, 16);

    float brightnessProperty = cArray.getCameraProperty(cv::CAP_PROP_BRIGHTNESS);
    printf("Brightness is %f\n", brightnessProperty);

    float hueProperty = cArray.getCameraProperty(cv::CAP_PROP_HUE);
    printf("hue is %f\n", hueProperty);

    // Save the images
    cv::Mat frame;
    for (int i = 0; i < 32; i++)
    {
        // capture the next frame from the webcam
        frame = cArray.getFrame(0);

        time_t t = time(NULL);
        int min = localtime(&t)->tm_min;
        int sec = localtime(&t)->tm_sec;

        char filename[128];
        snprintf(filename, 128, "test-%d:%d-%d.jpg", min, sec, i);
        cv::imwrite(filename, frame);
        sleep(0.0333);
    }

    for (int i = 0; i < 32; i++)
    {
        // capture the next frame from the webcam
        frame = cArray.getFrame(1);

        time_t t = time(NULL);
        int min = localtime(&t)->tm_min;
        int sec = localtime(&t)->tm_sec;

        char filename[128];
        snprintf(filename, 128, "test-%d:%d-%d.jpg", min, sec, i);
        cv::imwrite(filename, frame);
        sleep(0.0333);
    }
    return 0;
}