#include "camera.h"

/**
 * WaldoCamera constructor.
 */
WaldoCamera::WaldoCamera()
{
    this->capturedImages = 0;
	this->raw_img_dir = FILE_PATH_CAPTURED;
	this->img_format = CAPTURED_IMAGE_FORMAT;
	
    //default FPS
    this->fps = MAX_CAPTURE_FPS;

	this->capture_on = false;

    //creates QTimer and connects it to call captureEvent
    this->capture_timer = new QTimer(this);
	connect(capture_timer, &QTimer::timeout, this, &WaldoCamera::captureEvent);

    this->moveToThread(&this->cam_thread);
}

/**
 * WaldoCamera deconstructor.
 */
WaldoCamera::~WaldoCamera()
{
    this->toggleCapture(false);
}

void WaldoCamera::setFPS(float fps)
{
	this->fps = fps;
    if(this->capture_on)
    {
        this->capture_timer->start(1000/fps);
    }
}

/**
 * Set camera gain.
 *
 * @param gain Gain in dB
 */
void WaldoCamera::setGain(float gain)
{
    this->cam_ctrl.setGain(gain);
    this->gain = gain;
}

/**
 * Set camera auto-gain.
 *
 * @param auto_on Choose to continuously auto-calibrate gain
 */
void WaldoCamera::setAutoGain(bool auto_on)
{
    this->cam_ctrl.setAutoGain(auto_on);
    this->auto_gain = auto_on;
}



/**
 * Set camera exposure time.
 *
 * @param exposure_time Shutter time in µs
 */
void WaldoCamera::setExposureTime(float exposure_time)
{
    this->cam_ctrl.setExposureTime(exposure_time);
    this->exposure_time = exposure_time;
}

/**
 * Sets automatic camera exposure time.
 *
 * @param auto_on Boolean, on->shuttertime is continuously callibrated, off->current shuttertime remains constant
 */
void WaldoCamera::setAutoExposure(bool auto_exposure)
{
    this->cam_ctrl.setAutoExposure(auto_exposure);
    this->auto_exposure = auto_exposure;
}

void WaldoCamera::setGamma(float gamma)
{
	this->cam_ctrl.setGamma(gamma);
	this->gamma = gamma;
}

/**
 * Toggle camera runner active loop
 *
 * @param capture_on Specify if active image capture is on (otherwise freeze camera runner)
 */
void WaldoCamera::toggleCapture(bool capture_on)
{
	this->capture_on = capture_on;
    if (capture_on)
    {
        this->capture_timer->start(1000 / this->fps);
    }
    else
    {
        this->capture_timer->stop();
    }
}

/**
 * Image capture function
 *
 * When called will capture and store one image, will be called by capture_timer at given fps.
 */
void WaldoCamera::captureEvent(void)
{
	std::stringstream ss;
	ss << this->capturedImages;
    this->cam_ctrl.captureImage(this->raw_img_dir, ss.str(), this->img_format, &(this->last_img_timestamp));
    emit fwdImgtoGT(this->capturedImages, this->last_img_timestamp);
	this->capturedImages++;
}


void WaldoCamera::startCam()
{
    this->cam_thread.start();
}

void WaldoCamera::stopCam()
{
    this->cam_thread.quit();
}
