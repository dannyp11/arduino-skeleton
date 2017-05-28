#include <cxxtest/TestSuite.h>
#include <string>

#include "NMEAParser.h"

using std::string;

class MyTestSuite : public CxxTest::TestSuite
{
private:
	// expect a == b
	void confirmData(NMEAData a, NMEAData b)
	{
		TS_ASSERT_EQUALS(a.isValid, b.isValid);
		TS_ASSERT_EQUALS(a.date, b.date);
		TS_ASSERT_EQUALS(a.speed, b.speed);
		TS_ASSERT_EQUALS(a.timestamp, b.timestamp);

		TS_ASSERT_EQUALS(a.location.lat_deg, b.location.lat_deg);
		TS_ASSERT_EQUALS(a.location.lat_min, b.location.lat_min);
		TS_ASSERT_EQUALS(a.location.lon_deg, b.location.lon_deg);
		TS_ASSERT_EQUALS(a.location.lon_min, b.location.lon_min);
	}

public:
	void testAddition( void )
	{
		TS_ASSERT( 1 + 1 > 1 );
		TS_ASSERT_EQUALS( 1 + 1, 2 );
	}

	void setUp()
	{

	}

	void testRMCSuccess()
	{
		const string input1 = "$GPRMC,192826.000,A,4448.9261,N,09337.4366,W,0.02,123.77,270517,,,D*72";
		const string input2 = "$GPRMC,191903.000,A,4448.9275,N,09337.4360,W,0.57,184.90,270517,,,A*75";

		NMEAData data1, data2; // expected data
		NMEAData actual1, actual2;

		data1.timestamp = 192826;
		data1.isValid = 1;
		data1.date = 270517;
		data1.speed = .02;
		data1.location.lat_deg = 44;
		data1.location.lat_min = 48.9261;
		data1.location.lon_deg = -93;
		data1.location.lon_min = 37.4366;

		TS_ASSERT_EQUALS(NMEAParserParseString(input1.c_str(), &actual1), 0);
		TS_ASSERT_EQUALS(NMEAParserParseString(input2.c_str(), &actual2), 0);

		confirmData(data1, actual1);
		confirmData(data2, actual2);
	}
};

