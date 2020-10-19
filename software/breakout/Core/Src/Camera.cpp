/*
 * Camera.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
 */

#include "Camera.hpp"
#include "EPC901.hpp"
#include "Leds.hpp"
#include "Shell.hpp"
#include "CmdExposure.hpp"
#include "CmdCapture.hpp"
#include "CmdTransfer.hpp"
#include <cstring>

#include "CmdBurst.hpp"

Frame::Frame() {
	number = 0;
	timestamp = 0;
	exposure_time = 0;
	memset(pixels, 0, sizeof(pixels));
}

Camera::Camera() {
	_sensor = 0;
	_state = ST_IDLE;
	_exposure_time = 25;
	_frame_buffer = 0;
	_burst_enable = 0;
	_burst_fast = 0;
	_burst_interval = 0;
	_burst_frames = 2;
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
	default:
		_state = ST_IDLE;
		break;
	}

	return;
}

Camera::state Camera::getState(void) {
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

	if (_frame_count == 0) {
		_capture_start_time = HAL_GetTick();
	}

	if (_burst_enable && _burst_fast) {
		_state = ST_CAPTURE_FAST;
	} else {
		_state = ST_CAPTURE;
	}
	_capture_frame_time = HAL_GetTick();
	_capture_frame = 0;
	_capture_pending = 1;

	_sensor->_powerUp();
	_sensor->_clear();
	_sensor->_exposeImage(_exposure_time);

	return 0;
}

void Camera::abort(void) {
	if (!_sensor || _state == ST_IDLE) {
		return;
	}

	_sensor->_powerDown();
	_state = ST_IDLE;
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
			_sensor->_powerDown();
			_state = ST_IDLE;
		}
		_commitFrame(_capture_frame_time);
		_capture_frame++;

		if (!_burst_enable || _capture_frame >= _burst_frames) {
			// last frame, we're done
			_sensor->_powerDown();
			_state = ST_IDLE;
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
	if (_sensor->_dataReady()) {
		pixels = _sensor->_readImage(_frame_buffer[_frame_write].pixels);
		if (!pixels) {
			// abort if error reading out pixels
			_sensor->_powerDown();
			_state = ST_IDLE;
		}
		_commitFrame(prev_frame_time);
	}

	if (_capture_pending == 0) {
		// last frame, we're done
		_sensor->_powerDown();
		_state = ST_IDLE;
		return;
	}
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
