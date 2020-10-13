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

	uint16_t burst_frame = 0;
	if (_burst_enable) {
		if (_frame_count + _burst_frames > _frame_buf_size) {
			// no space to store all burst frames in frame buffer
			return 2;
		}
	}

	uint32_t frame_time, prev_frame_time = 0;
	uint16_t pixels;

	if (_frame_count == 0) {
		_start_time = HAL_GetTick();
	}

	if (!_burst_enable || _burst_frames < 2) {
		// capture a single frame
		frame_time = HAL_GetTick();
		_sensor->captureImage(_exposure_time, _frame_buffer[_frame_write].pixels);
		_commitFrame(frame_time);
	} else {
		// capture multiple frames in a burst
		// using low-level API of sensor class for maximum control
		_sensor->_powerUp();
		_sensor->_clear();

		if (_burst_fast) {
			// in fast burst, the next image is captured while the previous image is read out
			// capture first image of fast burst without reading it
			prev_frame_time = HAL_GetTick();
			_sensor->_exposeImage(_exposure_time);
			if (_burst_interval > 0) {
				while (HAL_GetTick() - prev_frame_time < _burst_interval);
			}
			burst_frame++;
		}

		do {
			if (_burst_fast) {
				// we must wait for exposure to be complete before starting a new one
				while(!_sensor->_dataReady());
				frame_time = prev_frame_time;
				prev_frame_time = HAL_GetTick();
			} else {
				frame_time = HAL_GetTick();
			}
			_sensor->_exposeImage(_exposure_time);

			pixels = _sensor->_readImage(_frame_buffer[_frame_write].pixels);

			if (pixels == 0) {
				// failed to capture image
				_sensor->_powerDown();
				return 1;
			}

			_commitFrame(frame_time);

			burst_frame++;
			// wait until it's time for the next frame
			if (_burst_interval > 0) {
				while (HAL_GetTick() - frame_time < _burst_interval);
			}
		} while (burst_frame < _burst_frames);

		if (_burst_fast) {
			// read out last image of fast burst
			pixels = _sensor->_readImage(_frame_buffer[_frame_write].pixels);
			if (pixels == 0) {
				// failed to capture image
				_sensor->_powerDown();
				return 1;
			}
			_commitFrame(prev_frame_time);
		}

		_sensor->_powerDown();
	}

	return 0;
}

void Camera::_commitFrame(uint32_t timestamp) {
	_frame_buffer[_frame_write].number = _frame_count;
	_frame_buffer[_frame_write].exposure_time = _exposure_time;
	_frame_buffer[_frame_write].timestamp = timestamp - _start_time;

	_frame_count++;
	_frame_write++;
	if (_frame_write >= _frame_buf_size) {
		_frame_write = 0;
	}
}

Frame* Camera::readFrame(void) {
	if (!_sensor || !_frame_buffer || _frame_count == 0) {
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

