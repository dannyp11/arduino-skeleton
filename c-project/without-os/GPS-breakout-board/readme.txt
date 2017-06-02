# This is gps tracker app that records distance traveled, angle to initial location, 
#        and distance to initial location. Initial location is determined when turned on


* Setup
  1. connect SCL, SDA from arduino to SCL, SDA of 4x20 LCD
  2. connect GPS TX to arduino RX (0)
  3. connect to arduino serial port

* Features
  1. Added NMEA Parser, supported message:
     - GPRMC
     - GPGGA
  2. NMEA Parser unit test added
  3. NMEA Location calculation unit test added 
