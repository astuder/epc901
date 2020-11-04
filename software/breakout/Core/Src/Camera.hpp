/*
 * Camera application logic.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
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

	void reset(void);
	void init(EPC901 *sensor, Shell *shell, Frame* frame_buffer, uint16_t frame_buf_size);
	void loop(void);

	enum state_t {
		ST_IDLE = 0,
		ST_CAPTURE = 1,
		ST_CAPTURE_FAST = 2,
		ST_TRIGGER = 3,
		ST_TRIGGER_DELAY = 4
	};

	enum trig_src_t {
		TRIG_EXTERNAL = 0,
		TRIG_LEVEL = 1,
		TRIG_ZONE = 2
	};

	enum trig_dir_t {
		TRIG_RISING = 0,
		TRIG_FALLING = 1
	};

	struct trig_zone_t {
		uint16_t x1, y1, x2, y2;
	};

	state_t getState(void);

	void setExposureTime(uint32_t time_us);
	uint32_t getExposureTime(void);
	uint32_t getMaxExposureTime(void);

	void setBurstEnable(uint8_t enable);
	uint8_t getBurstEnable(void);
	void setBurstFast(uint8_t enable);
	uint8_t getBurstFast(void);
	void setBurstFrames(uint16_t frames);
	uint16_t getBurstFrames(void);
	void setBurstInterval(uint16_t);
	uint16_t getBurstInterval(void);

	void setTriggerEnable(uint8_t enable);
	uint8_t getTriggerEnable(void);
	void setTriggerSource(trig_src_t source);
	trig_src_t getTriggerSource(void);
	void setTriggerDelay(uint16_t delay);
	uint16_t getTriggerDelay(void);
	void setTriggerDirection(trig_dir_t direction);
	trig_dir_t getTriggerDirection(void);
	void setTriggerLevel(uint16_t level);
	uint16_t getTriggerLevel(void);
	void setTriggerZone(trig_zone_t zone);
	trig_zone_t getTriggerZone(void);
	void externalTrigger(void);

	uint8_t capture(void);
	void abort(void);

	Frame* readFrame(void);			// read next available frame, returns 0 if no frame available
	uint16_t getRemainingFrames();

private:
	void _captureLoop(void);
	void _captureLoopFast(void);
	void _triggerLoop(void);
	void _triggerDelayLoop(void);
	void _startCapture(uint8_t pre_trigger);
	void _stopCapture(void);
	void _commitFrame(uint32_t timestamp);
	void _setupExternalTrigger(void);

	EPC901* _sensor;

	state_t _state;

	uint32_t _exposure_time;

	uint8_t _burst_enable;
	uint8_t _burst_fast;
	uint16_t _burst_frames;
	uint16_t _burst_interval;		// time per frame in ms, 0=best effort

	uint8_t _trigger_enable;
	uint32_t _trigger_time;
	trig_dir_t _trigger_direction;
	trig_src_t _trigger_source;
	uint16_t _trigger_delay;
	uint16_t _trigger_level;
	trig_zone_t _trigger_zone;
	uint8_t _external_trigger;

	uint32_t _capture_start_time;
	uint32_t _capture_frame_time;
	uint16_t _capture_frame;
	uint8_t _capture_pending;

	Frame* _frame_buffer;
	uint16_t _frame_buf_size;
	uint16_t _frame_count;
	uint16_t _frame_read;
	uint16_t _frame_write;
};

#endif /* SRC_CAMERA_HPP_ */
