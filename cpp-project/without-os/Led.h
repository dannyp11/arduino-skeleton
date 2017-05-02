/*******************************************************************************
 *
 * Created on May 1, 2017 by Dat (Daniel) Pham
 *
 * Filename: Led.h
 *
 * Description: 
 *
 * Limitations: 
 *
 * Environment: ANSI C, X-series
 *
 * Version: @(#) (Internal Only)
 *
 * History: 
 *
 ******************************************************************************/
#ifndef CPP_PROJECT_WITHOUT_OS_LED_H_
#define CPP_PROJECT_WITHOUT_OS_LED_H_

class Led
{
public:
	Led();

	void turnOn() const;
	void turnOff() const;

	/*note that there is no destructor since new and delete are not implemented in avr*/

private:


};

#endif /* CPP_PROJECT_WITHOUT_OS_LED_H_ */
