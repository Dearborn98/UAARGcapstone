#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <iostream>
#include <fstream>
#include <sstream>
#include "configuration.h"
#include <QThread>
#include <QTimer>
#include <QString>

//#if (CAMERA_MODULE == CAMERA_TEST)
//#include "cameras/test_cam.h"
//#elif (CAMERA_MODULE == CAMERA_SPINNAKER)
//#include "cameras/spinnaker_cam.h"
#endif

class WaldoCamera : public QObject
{
	Q_OBJECT
	
	private:
	
	    CameraControl cam_ctrl;
        QTimer *capture_timer;
		bool capture_on;
		
		float fps;

		float exposure_time;
		bool auto_exposure;

		float gain;
		bool auto_gain;
		float gamma;
		
		QThread cam_thread;

		unsigned int capturedImages;
		std::string raw_img_dir;
		std::string img_format;
        long long last_img_timestamp;

	public slots:
		/*
		Receives signals from Waldo to change camera parameters or to start/stop the camera.
		*/		

		void toggleCapture(bool capture_on);//triggers the cameraRunner to capture image at the current fps and stops the camera. 

		void setFPS(float fps);
        void captureEvent(); //called by capture_timer to take and store image

		void setExposureTime(float exposure_time);
        void setAutoExposure(bool auto_exposure);

		void setGain(float gain);
		void setAutoGain(bool auto_gain);
		
		void setGamma(float gamma);

		//--------New Classes-----------------------------//
		void toggleCaptureMode() //toggles between capture and video mode
		void toggleVideo() //triggers the camera to record video or to stop recording
		void processVideoCapture() //process and stream the camera output 

	signals:
		//Camera send off signal to geotagging whenever an image is taken
        void fwdImgtoGT(int img_num, long long time);

    friend class TestCameraControl;

	public:
		WaldoCamera();
        ~WaldoCamera();
		void startCam();
		void stopCam();
};


#endif
