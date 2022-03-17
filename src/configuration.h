#ifdef STANDALONE_TEST

#ifndef WALDO_CAMERA_CONFIGURATION_H
#define WALDO_CAMERA_CONFIGURATION_H

#define CAMERA_TEST 0
#define CAMERA_SPINNAKER 1
#define FILE_PATH_CAPTURED      "/dev/shm"
#define CAPTURED_IMAGE_FORMAT    "jpg"
#define MAX_CAPTURE_FPS    5                 //in Hz
#define CAMERA_MODULE CAMERA_TEST

#endif

#else /* waldo configuration */
//#include "../../src/configuration.h"      -- I think this is using the config from the waldo module

#endif