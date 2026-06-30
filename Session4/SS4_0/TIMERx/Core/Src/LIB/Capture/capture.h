	/*
	 * capture.h
	 *
	 *  Created on: Jun 27, 2026
	 *      Author: quocvi
	 */

	#ifndef SRC_LIB_PWM_CAPTURE_CAPTURE_H_
	#define SRC_LIB_PWM_CAPTURE_CAPTURE_H_

	void capture_init();
	uint32_t capture_read(void);
	uint32_t capture_pulse_width1();
	uint32_t capture_pulse_width2();
	uint32_t capture_pulse_width3();
	uint32_t capture_pulse_width4();


	#endif /* SRC_LIB_PWM_CAPTURE_CAPTURE_H_ */
