/*
 * NMEALocationCalculatorTest.h
 *
 *  Created on: May 28, 2017
 *      Author: dat
 */

#ifndef TEST_GPS_BREAKOUT_BOARD_TEST_NMEALOCATIONCALCULATORTEST_H_
#define TEST_GPS_BREAKOUT_BOARD_TEST_NMEALOCATIONCALCULATORTEST_H_

#include <cxxtest/TestSuite.h>
#include <string>

#include "NMEAParser.h"

class MyTestSuite: public CxxTest::TestSuite
{
private:

public:
	void setUp()
	{

	}

	void testDistanceSuccess()
	{
		NMEALocation location1, location2;

		location1.lat_deg = 44;
		location1.lat_min = 33.1;
		location1.lon_deg = 33;
		location1.lon_min = 44.1;

		location2.lat_deg = 44;
		location2.lat_min = 33.099;
		location2.lon_deg = 33;
		location2.lon_min = 44.1;

		TS_ASSERT_DELTA(NMEAGetDistance(&location1, &location2), 0.0019, 0.0001);
	}
};

#endif /* TEST_GPS_BREAKOUT_BOARD_TEST_NMEALOCATIONCALCULATORTEST_H_ */
