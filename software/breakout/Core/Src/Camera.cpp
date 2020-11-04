/*
 * Camera application logic.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#include "main.h"
#include "Camera.hpp"
#include "EPC901.hpp"
#include "Leds.hpp"
#include "Shell.hpp"
#include "CmdExposure.hpp"
#include "CmdCapture.hpp"
#include "CmdTransfer.hpp"
#include "CmdBurst.hpp"
#include "CmdTrigger.hpp"
#include <cstring>

Frame::Frame() {
	number = 0;
	timestamp = 0;
	exposure_time = 0;
	memset(pixels, 0, sizeof(pixels));
}

Camera::Camera() {
	_sensor = 0;
	_frame_buffer = 0;
	reset();
}

void Camera::reset(void) {
	// set sensible defaults
	_state = ST_IDLE;
	_exposure_time = 25;
	_burst_enable = 0;
	_burst_fast = 0;
	_burst_interval = 0;
	_burst_frames = 2;
	_trigger_enable = 0;
	_trigger_source = TRIG_EXTERNAL;
	_trigger_delay = 0;
	_trigger_direction = TRIG_RISING;
	_trigger_level = 1500;
	_external_trigger = 0;
	memset(&_trigger_zone, 0, sizeof(trig_zone_t));
}

void Camera::init(EPC901 *sensor, Shell *shell, Frame* frame_buffer, uint16_t frame_buf_size) {
	_sensor = sensor;

	// register camera commands with shell
	if (shell) {
		static CmdExposure cmd_exposure(this);
		shell->registerCmd(&cmd_exposure);
		static CmdCapture cmd_capture(this);
		shell->registerCmd(&cmd_capture);
		static CmdTransfer cmd_transfer(this);
		shell->registerCmd(&cmd_transfer);
		static CmdBurst cmd_burst(this);
		shell->registerCmd(&cmd_burst);
		static CmdTrigger cmd_trigger(this);
		shell->registerCmd(&cmd_trigger);
	}

	// setup frame buffer
	// we need space for at least 1 frame, may be extended later for functionality like HDR
	if (frame_buffer && frame_buf_size) {
		_frame_buffer = frame_buffer;
		_frame_buf_size = frame_buf_size;
	} else {
		_frame_buffer = 0;
		_frame_buf_size = 0;
	}
	_frame_read = 0;
	_frame_write = 0;
	_frame_count = 0;
}

void Camera::loop(void) {
	switch (_state) {
	case ST_IDLE:
		break;
	case ST_CAPTURE:
		_captureLoop();
		break;
	case ST_CAPTURE_FAST:
		_captureLoopFast();
		break;
	case ST_TRIGGER:
		_triggerLoop();
		break;
	case ST_TRIGGER_DELAY:
		_triggerDelayLoop();
		break;
	default:
		_state = ST_IDLE;
		break;
	}

	return;
}

Camera::state_t Camera::getState(void) {
	return _state;
}

void Camera::setExposureTime(uint32_t time_us) {
	if (time_us == 0) {
		// TODO: determine exposure automatically
	} else {
		if (time_us > 0 && time_us <= EPC901::getMaxExposure()) {
			_exposure_time = time_us;
		} else {
			// ignore invalid values
		}
	}
}

uint32_t Camera::getExposureTime(void) {
	return _exposure_time;
}

uint32_t Camera::getMaxExposureTime(void) {
	return EPC901::getMaxExposure();
}

void Camera::setBurstEnable(uint8_t enable) {
	if (0 == enable) {
		_burst_enable = 0;
	} else {
		_burst_enable = 1;
	}
}

uint8_t Camera::getBurstEnable(void) {
	return _burst_enable;
}

void Camera::setBurstFast(uint8_t fast) {
	if (0 == fast) {
		_burst_fast = 0;
	} else {
		_burst_fast = 1;
	}
}

uint8_t Camera::getBurstFast(void) {
	return _burst_fast;
}

void Camera::setBurstFrames(uint16_t frames) {
	if (frames > _frame_buf_size) {
		_burst_frames = _frame_buf_size;
	} else {
		_burst_frames = frames;
	}
}

uint16_t Camera::getBurstFrames(void) {
	return _burst_frames;
}

void Camera::setBurstInterval(uint16_t interval) {
	_burst_interval = interval;
}

uint16_t Camera::getBurstInterval(void) {
	return _burst_interval;
}

void Camera::setTriggerEnable(uint8_t enable) {
	if (0 == enable) {
		_trigger_enable = 0;
	} else {
		_trigger_enable = 1;
	}
}

uint8_t Camera::getTriggerEnable(void) {
	return _trigger_enable;
}

void Camera::setTriggerSource(trig_src_t source) {
	_trigger_source = source;
}

Camera::trig_src_t Camera::getTriggerSource(void) {
	return _trigger_source;
}

void Camera::setTriggerDelay(uint16_t delay) {
	_trigger_delay = delay;
}

uint16_t Camera::getTriggerDelay(void) {
	return _trigger_delay;
}

void Camera::setTriggerDirection(trig_dir_t direction) {
	_trigger_direction = direction;
}

Camera::trig_dir_t Camera::getTriggerDirection(void) {
	return _trigger_direction;
}

void Camera::setTriggerLevel(uint16_t level) {
	_trigger_level = level;
}

uint16_t Camera::getTriggerLevel(void) {
	return _trigger_level;
}

void Camera::setTriggerZone(trig_zone_t zone) {
	if (zone.x1 >= 1024 || zone.x2 >= 1024 ||
		zone.y1 >= 4096 || zone.y2 >= 4096) {
		// zone values out of bound
		return;
	}

	if (zone.x1 <= zone.x2) {
		_trigger_zone.x1 = zone.x1;
		_trigger_zone.x2 = zone.x2;
	} else {
		_trigger_zone.x1 = zone.x2;
		_trigger_zone.x2 = zone.x1;
	}
	if (zone.y1 <= zone.y2) {
		_trigger_zone.y1 = zone.y1;
		_trigger_zone.y2 = zone.y2;
	} else {
		_trigger_zone.y1 = zone.y2;
		_trigger_zone.y2 = zone.y1;
	}
}

Camera::trig_zone_t Camera::getTriggerZone(void)
{
	return _trigger_zone;
}

void Camera::externalTrigger(void) {
	if (_state == ST_TRIGGER) {
		_external_trigger = 1;
	}
}

uint8_t Camera::capture(void) {
	if (!_sensor) {
		return 1;
	}

	if (_frame_count >= _frame_buf_size) {
		// no more space in frame buffer
		return 2;
	}

	if (_burst_enable) {
		if (_frame_count + _burst_frames > _frame_buf_size) {
			// no space to store all burst frames in frame buffer
			return 2;
		}
	}

	if (_trigger_enable) {
		_state = ST_TRIGGER;
		if (_trigger_source == TRIG_EXTERNAL) {
			_setupExternalTrigger();
		} else {
			_startCapture(1);
		}
		return 0;
	}

	_startCapture(0);

	return 0;
}

void Camera::abort(void) {
	if (!_sensor || _state == ST_IDLE) {
		return;
	}

	_stopCapture();
}

Frame* Camera::readFrame(void) {
	if (!_sensor || !_frame_buffer || _frame_count == 0 || _state != ST_IDLE) {
		return 0;
	}

	Frame* frame = &_frame_buffer[_frame_read];

	_frame_count--;
	_frame_read++;
	if (_frame_read >= _frame_buf_size) {
		_frame_read = 0;
	}

	return frame;
}

uint16_t Camera::getRemainingFrames(void) {
	return _frame_count;
}

void Camera::_captureLoop(void) {
	uint16_t pixels;

	if (_capture_pending && _sensor->_dataReady()) {
		_capture_pending = 0;
		pixels = _sensor->_readImage(_frame_buffer[_frame_write].pixels);
		if (!pixels) {
			// abort if error reading out pixels
			_stopCapture();
		}
		_commitFrame(_capture_frame_time);
		_capture_frame++;

		if (!_burst_enable || _capture_frame >= _burst_frames) {
			// last frame, we're done
			_stopCapture();
			return;
		}
	}

	if (!_capture_pending) {
		// start exposure of next frame in burst when time is up
		if (HAL_GetTick() - _capture_frame_time >= _burst_interval) {
			_capture_frame_time = HAL_GetTick();
			_sensor->_exposeImage(_exposure_time);
			_capture_pending = 1;
		}
	}
}

void Camera::_captureLoopFast(void) {
	uint16_t pixels;

	if (HAL_GetTick() - _capture_frame_time < _burst_interval) {
		// don't do anything until next frame is due
		return;
	}

	if (_sensor->_dataReady()) {
		// preserve time stamp of previous frame
		uint32_t prev_frame_time = _capture_frame_time;
		_capture_frame++;

		// start next capture
		if (_capture_frame < _burst_frames) {
			_capture_frame_time = HAL_GetTick();
			_sensor->_exposeImage(_exposure_time);
			_capture_pending = 1;
		} else {
			_capture_pending = 0;
		}

		// read previous frame
		pixels = _sensor->_readImage(_frame_buffer[_frame_write].pixels);
		if (!pixels) {
			// abort if error reading out pixels
			_stopCapture();
		}
		_commitFrame(prev_frame_time);
	}

	if (_capture_pending == 0) {
		// last frame, we're done
		_stopCapture();
		return;
	}
}

void Camera::_triggerLoop(void) {
	if (TRIG_EXTERNAL == _trigger_source) {
		// trigger on external pin
		if (_external_trigger) {
			if (_trigger_delay == 0) {
				_startCapture(0);
			} else {
				_trigger_time = HAL_GetTick();
				_state = ST_TRIGGER_DELAY;
			}
		}
	} else {
		// trigger based on image content
		uint8_t triggered = 0;
		if (_sensor->_dataReady()) {
			uint16_t pixels = _sensor->_readImage(_frame_buffer[_frame_write].pixels);
			if (!pixels) {
				// abort if error reading out pixels
				_stopCapture();
			} else {
				if (TRIG_LEVEL == _trigger_source) {
					// trigger on pixel exceeding level
					while (pixels > 0) {
						pixels--;
						if (_frame_buffer[_frame_write].pixels[pixels] >= _trigger_level) {
							triggered = 1;
							break;
						}
					}
					// if direction falling, reverse logic (trigger if all pixels below threshold, don't trigger if any pixel above)
					if (TRIG_FALLING == _trigger_direction) {
						triggered ^= 1;
					}
				} else if (TRIG_ZONE == _trigger_source) {
					// trigger on pixel in zone
					pixels = _trigger_zone.x1;
					while (pixels <= _trigger_zone.x2) {
						uint16_t p = _frame_buffer[_frame_write].pixels[pixels];
						if (p >= _trigger_zone.y1 && p <= _trigger_zone.y2) {
							triggered = 1;
							break;
						}
						pixels++;
					}
					// if direction falling, reverse logic (trigger if all pixels outside zone, don't trigger if any pixel inside)
					if (TRIG_FALLING == _trigger_direction) {
						triggered ^= 1;
					}
				}

				if (triggered) {
					if (_trigger_delay == 0) {
						_commitFrame(_capture_frame_time);
						if (_burst_enable && _burst_frames > 0) {
							// capture remaining frames
							_startCapture(0);
							_capture_frame = 1;
						} else {
							// only 1 frame requested, we're already done
							_stopCapture();
						}
					} else {
						_trigger_time = HAL_GetTick();
						_state = ST_TRIGGER_DELAY;
					}
				} else {
					// capture another frame to check for trigger condition
					_startCapture(1);
				}
			}
		}
	}
}

void Camera::_triggerDelayLoop(void) {
	if (HAL_GetTick() - _trigger_time < _trigger_delay) {
		return;
	}

	_startCapture(0);
}

void Camera::_startCapture(uint8_t pre_trigger) {
	if (_frame_count == 0) {
		_capture_start_time = HAL_GetTick();
	}

	_capture_frame_time = HAL_GetTick();
	_capture_frame = 0;
	_capture_pending = 1;

	_sensor->_exposeImage(_exposure_time);

	if (pre_trigger == 0) {
		if (_burst_enable && _burst_fast) {
			_state = ST_CAPTURE_FAST;
		} else {
			_state = ST_CAPTURE;
		}
	}
}

void Camera::_stopCapture(void) {
	_sensor->_powerDown();
	_state = ST_IDLE;
}

void Camera::_commitFrame(uint32_t timestamp) {
	_frame_buffer[_frame_write].number = _frame_count;
	_frame_buffer[_frame_write].exposure_time = _exposure_time;
	_frame_buffer[_frame_write].timestamp = timestamp - _capture_start_time;

	_frame_count++;
	_frame_write++;
	if (_frame_write >= _frame_buf_size) {
		_frame_write = 0;
	}
}

void Camera::_setupExternalTrigger(void) {
	uint32_t pin_bits = TRIGGER_Pin | B1_Pin;
	uint32_t pin_mask = ~pin_bits;
	if (_trigger_direction == TRIG_RISING) {
		EXTI->FTSR1 &= pin_mask;
		EXTI->RTSR1 |= pin_bits;
	} else {
		EXTI->FTSR1 |= pin_bits;
		EXTI->RTSR1 &= pin_mask;
	}
	_external_trigger = 0;
}
