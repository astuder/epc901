/*
 * Camera.hpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
 */

#ifndef SRC_CAMERA_HPP_
#define SRC_CAMERA_HPP_

#include <cstddef>
#include <inttypes.h>

class EPC901;
class Shell;

class Frame {
public:
	Frame();
	static const uint16_t PIXEL_COUNT = 1024;
	uint32_t number;
	uint32_t timestamp;
	uint32_t exposure_time;
	uint16_t pixels[PIXEL_COUNT];
private:
};

class Camera {
public:
	Camera();

	void init(EPC901 *sensor, Shell *shell, Frame* frame_buffer, uint16_t frame_buf_size);

	void setExposureTime(uint32_t time_us);
	uint32_t getExposureTime(void);
	void setBurstEnable(uint8_t enable);
	uint8_t getBurstEnable(void);
	void setBurstFrames(uint16_t frames);
	uint16_t getBurstFrames(void);
	void setBurstInterval(uint16_t);
	uint16_t getBurstInterval(void);

	uint8_t capture(void);
	void burst(uint16_t frame_count);

	Frame* readFrame(void);			// read next available frame, returns 0 if no frame available
	uint16_t getRemainingFrames();

private:
	EPC901* _sensor;
	uint32_t _exposure_time;
	uint32_t _start_time;

	uint8_t _burst_enable;
	uint16_t _burst_frames;
	uint16_t _burst_interval;		// time per frame in ms, 0=best effort

	Frame* _frame_buffer;
	uint16_t _frame_buf_size;
	uint16_t _frame_count;
	uint16_t _frame_read;
	uint16_t _frame_write;
};

#endif /* SRC_CAMERA_HPP_ */
