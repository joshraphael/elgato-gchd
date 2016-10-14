/**
 * Copyright (c) 2016 Scott Dossey <seveirein@yahoo.com>
 *
 * This source file is part of Game Capture HD Linux driver and is distributed
 * under the MIT License. For more information, see LICENSE file.
 */

#include <exception>
#include <cmath>
#include "../gchd.hpp"

void GCHD::readSignalInformation(bool blockA, bool blockB,
				 unsigned &sum6463, unsigned &countSum6463,
				 unsigned &sum6665, unsigned &countSum6665, bool &rgbBit)
{
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	if( blockA ) {
		unsigned value6665  = readDevice0x9DCD(0x66) <<8;
		value6665 |= readDevice0x9DCD(0x65);
		sum6665 += value6665;
		countSum6665 += 1;
	}
	if( blockB ) {
		readDevice0x9DCD(0x6a); //EXPECTED 0xcd
	}
	unsigned value6463=readDevice0x9DCD(0x64) <<8;
	value6463 |= readDevice0x9DCD(0x63);
	sum6463 += value6463;
	countSum6463 += 1;

	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0xce} );
	uint8_t value=readDevice0x9DCD(0x34);
	rgbBit=(value >>2) & 1;
	readDevice0x9DCD(0x16); //EXPECTED 0xb5
	readDevice0x9DCD(0x17); //EXPECTED 0x32
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
}

void GCHD::configureHDMI()
{
	mailWrite( 0x33, VC{0x94, 0x41, 0x37} );
	mailWrite( 0x33, VC{0x94, 0x4a, 0xaf} );
	mailWrite( 0x33, VC{0x94, 0x4b, 0xaf} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	readDevice0x9DCD(0x94); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0xab, 0x4c} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0xce} );
	mailWrite( 0x4e, VC{0x1b, 0x33} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	readDevice0x9DCD(0x88); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0xb7, 0xce} );
	mailWrite( 0x4e, VC{0xb8, 0xdc} );
	mailWrite( 0x4e, VC{0xb8, 0xcc} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0xce} );
	mailWrite( 0x4e, VC{0x07, 0x38} );
	mailWrite( 0x4e, VC{0x07, 0xc8} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	mailWrite( 0x4e, VC{0x51, 0x45} );
	readDevice0x9DCD(0x88); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0xb7, 0xcc} );

	//Do nothing enable. No idea what it really checks
	doEnable( 0x0000, 0x0000); //state 031e->031e, enable 000a->000a
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0xce} );
	readDevice0x9DCD(0x3e); //EXPECTED 0xd3
	mailWrite( 0x4e, VC{0x01, 0xad} );
	readDevice0x9DCD(0x3b); //EXPECTED 0xb3
	mailWrite( 0x4e, VC{0x04, 0xcd} );
	mailWrite( 0x4e, VC{0x06, 0xc4} );
	readDevice0x9DCD(0x36); //EXPECTED 0xba
	mailWrite( 0x4e, VC{0x09, 0xe4} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	readDevice0x9DCD(0x6b); //EXPECTED 0x82
	mailWrite( 0x4e, VC{0x54, 0xec} );
	readDevice0x9DCD(0x93); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0xac, 0x4c} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0x4c} );
	readDevice0x9DCD(0x3f); //EXPECTED 0x32
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	readDevice0x9DCD(0xf1); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0xce, 0x4c} );
	readDevice0x9DCD(0xf0); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0xcf, 0xce} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2

	unsigned sum6665=0;
	unsigned sum6463=0;
	unsigned countSum6665=0;
	unsigned countSum6463=0;
	bool rgbBit;

	readSignalInformation( false, true, sum6463, countSum6463, sum6665, countSum6665, rgbBit);
	for( unsigned i=0; i<14; ++i ) {
		readSignalInformation( true, false, sum6463, countSum6463, sum6665, countSum6665, rgbBit);
	}
	//Reset sums. Hackish. Wish I knew the lock logic. Likely wait for bit 11, which may
	//or may not be a lock bit. At any rate, continues to read for some time after lock
	//if that is a lock bit.
	sum6665=0;
	sum6463=0;
	countSum6665=0;
	countSum6463=0;

	readSignalInformation( true, true, sum6463, countSum6463, sum6665, countSum6665, rgbBit);
	readSignalInformation( true, true, sum6463, countSum6463, sum6665, countSum6665, rgbBit);

	interlaced_=false;
	if (settings_->getInputResolution() == Resolution::Unknown) {
		//Okay time to figure out signal.
		double value6665=((double)sum6665) / countSum6665;
		if(std::abs( value6665 - 0xba95 )>10.0) {
			//probably not appropriate way to handle.
			throw runtime_error( "Mode detection failed, does not appear to be 1080 or 720 signal on HDMI.");
		}

		double value6463=((double)sum6463) / countSum6463;

		if(std::abs( value6463 - 0xb05c )<10.0) { //Allow for error.
			//720p
			settings_->setInputResolution( Resolution::HD720 );
		} else if(fabs( value6463 - 0xb6d7 )<10.0) { //Allow for error.
			//1080p
			settings_->setInputResolution( Resolution::HD1080 );
		} else if(fabs( value6463 - 0xb081 )<10.0) {
			//1080i
			settings_->setInputResolution( Resolution::HD1080 );
			interlaced_=true;
		} else {
			//Still not appropriate way to handle.
			throw runtime_error( "Mode detection failed, does not appear to be 1080 or 720 signal on HDMI.");
		}
	}
	switch( settings_->getInputResolution() ) {
		case Resolution::HD1080:
			refreshRate_=60; //Strictly speaking, full frame refresh is 30 for 1080i. Hmm.
			break;
		case Resolution::HD720:
			refreshRate_=60;
			break;
		default:
			throw runtime_error( "Cannot use set input resolution with HDMI currently.");
			break;
	}
	mailWrite( 0x4e, VC{0x00, 0xcc} );

	//Mystery setup difference. May be more appropriate to switch on reading back 0x95 instead of 0x97
	//Based on input resolution.
	if (settings_->getInputResolution() == Resolution::HD720) {
		mailWrite( 0x4e, VC{0xb2, 0xcc} );
		mailWrite( 0x4e, VC{0xb5, 0xcc} );
	} else { //Assumed 1080.
		readDevice0x9DCD(0x8d); //EXPECTED 0xb2
		mailWrite( 0x4e, VC{0xb2, 0xc4} );
		readDevice0x9DCD(0x8a); //EXPECTED 0xb2
		mailWrite( 0x4e, VC{0xb5, 0xd0} );
	}
	mailWrite( 0x4e, VC{0x00, 0xce} );
	mailWrite( 0x4e, VC{0x1b, 0x30} );

	mailWrite( 0x4e, VC{0x1f, 0xdc} );
	readDevice0x9DCD(0x29); //EXPECTED 0xba
	mailWrite( 0x4e, VC{0x1f, 0xcc} );
	readDevice0x9DCD(0x3b); //EXPECTED 0xb3
	mailWrite( 0x4e, VC{0x04, 0xcf} );
	mailWrite( 0x4e, VC{0x04, 0xcd} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	mailWrite( 0x4e, VC{0x40, 0xcc} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0xcd} );

	mailWrite( 0x4e, VC{0x00, 0xcc} );
	mailWrite( 0x4e, VC{0xb0, 0xe8} );
	mailWrite( 0x4e, VC{0xb1, 0x0c} );
	mailWrite( 0x4e, VC{0xad, 0xc9} );
	readDevice0x9DCD(0x8f); //EXPECTED 0x96
	mailWrite( 0x4e, VC{0xb0, 0xe9} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2

	mailWrite( 0x4e, VC{0x00, 0xcc} );
	mailWrite( 0x4e, VC{0xab, 0xcc} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x82}
	mailWrite( 0x33, VC{0x99, 0x89, 0xfd} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6f}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x82}
	mailWrite( 0x33, VC{0x99, 0x89, 0xfc} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf3} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0x0c, 0x89} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x82}
	mailWrite( 0x4c, VC{0x0e, 0x65} );
	mailWrite( 0x4c, VC{0x0e, 0x64} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xfd} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0x88} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x6e, 0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x6e, 0x22, 0x43, 0xa3, 0x6b, 0x6e, 0x6e, 0x6e, 0x6e, 0x7c, 0x7d, 0x6f, 0x6d, 0xee, 0x7e, 0x67, 0x16, 0x44, 0x0e, 0x2f, 0xc8, 0x38, 0x24, 0xf2, 0x4b}
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0xa8} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x7c, 0x3e, 0x3a, 0x4d, 0x66, 0x6e, 0xc7, 0x2e, 0xef, 0xee, 0xef, 0x2e, 0xef, 0x6e, 0xfb, 0x6e, 0xdd, 0x6e, 0x6f, 0x6f, 0x6f, 0x6f, 0x6c, 0x54, 0xee, 0x76, 0x1f, 0x56, 0x43, 0x2e, 0x36, 0x42}
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0xc8} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x2b, 0x6e, 0xce, 0x34, 0x6e, 0x6e, 0x6e, 0x70, 0x6f, 0x73, 0x6e, 0x1c, 0x3f, 0xbe, 0x70, 0x4e, 0x00, 0x46, 0x3b, 0x6e, 0xce, 0x34, 0x6e, 0x6e, 0x6e, 0x70, 0x6e, 0x6e, 0x6e, 0x93, 0x6e, 0x5c}
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0xe8} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x52, 0x70, 0x3f, 0x7f, 0x6e, 0x64, 0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x4e, 0x6e, 0x6e, 0x6e, 0x92, 0x6e, 0x3d, 0x17, 0x00, 0x0d, 0x23, 0x0f, 0x1d, 0x1a, 0x0b, 0x1c, 0x64, 0x4e, 0x4e, 0x6f, 0xfa}

	mailWrite( 0x33, VC{0x99, 0x89, 0xfd} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0x08} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x6c, 0x6d, 0x72, 0x9f, 0x26, 0xfe, 0x6a, 0x71, 0x6b, 0x7a, 0x7d, 0x6d, 0x7c, 0x4d, 0x67, 0x69, 0x69, 0xed, 0x6f, 0x6e, 0x6e, 0x08, 0x6d, 0x62, 0x6e, 0x7e, 0x6e, 0xee, 0x6f, 0x73, 0xee, 0xbe}
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0x28} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x1c, 0x72, 0x78, 0x4e, 0x7e, 0x42, 0x4b, 0xee, 0xce, 0x34, 0x6e, 0x6e, 0x6e, 0xf0, 0x6f, 0x73, 0xee, 0x76, 0x1f, 0x72, 0x78, 0x4e, 0x36, 0x42, 0x4b, 0x6e, 0xce, 0x34, 0x6e, 0x6e, 0x6e, 0xf0}
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0x48} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x6f, 0x73, 0x6e, 0xd2, 0x3c, 0xbe, 0x70, 0x4e, 0xd6, 0x46, 0x3b, 0x2e, 0xce, 0x34, 0x6e, 0x6e, 0x6e, 0x70, 0xe2, 0x64, 0xbe, 0xfe, 0x4e, 0x2e, 0x5f, 0x4e, 0x62, 0x2e, 0x3b, 0x6e, 0xce, 0x34}
	mailWrite( 0x4c, VC{0x15, 0x81} );
	mailWrite( 0x4c, VC{0x65, 0x8a} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0x28} );
	mailWrite( 0x4c, VC{0x12, 0x68} );
	mailWrite( 0x4c, VC{0x13, 0xa8} );
	mailWrite( 0x4c, VC{0x14, 0x88} );
	mailWrite( 0x4c, VC{0x15, 0x8b} );
	pollOn0x9989ED();
	readFrom0x9989EC(32); //EXPECTED {0x6e, 0x6e, 0x6e, 0x76, 0xe2, 0x64, 0xbe, 0xe4, 0x4e, 0x8e, 0x43, 0x7e, 0x7e, 0x50, 0xf8, 0x6e, 0xce, 0x34, 0x6e, 0x6e, 0x6e, 0x76, 0x6e, 0x6e, 0x6e, 0x6e, 0x6e, 0x6e, 0x6e, 0x6e, 0x6e, 0x28}

	if ( deviceType_ == DeviceType::GameCaptureHD ) {
		mailWrite( 0x33, VC{0xaa, 0xb8, 0x29, 0xf6} );
		mailWrite( 0x33, VC{0xa1, 0x08, 0x73} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0xa6, 0x33, 0xc0, 0x4d, 0xda, 0x67, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x7b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xf2, 0xb8, 0xcd, 0x3f, 0xb2, 0x25, 0x98, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x63} );
		mailRead( 0x33, 8 ); //EXPECTED {0xf7, 0x4e, 0xcd, 0x3c, 0x32, 0x25, 0x9a, 0x73}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x6b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xfc, 0x96, 0xb8, 0x9c, 0xe5, 0x69, 0x28, 0x28}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x53} );
		mailRead( 0x33, 8 ); //EXPECTED {0xef, 0x11, 0x80, 0x3f, 0xb2, 0x25, 0x19, 0xcb}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x5b} );
		mailRead( 0x33, 8 ); //EXPECTED {0x37, 0x99, 0xcd, 0x3e, 0xb3, 0x24, 0x99, 0x0a}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x43} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe7, 0x58, 0xcd, 0x3e, 0xb3, 0x24, 0x9a, 0x31}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x4b} );
		mailRead( 0x33, 8 ); //EXPECTED {0x66, 0x41, 0xbd, 0x07, 0x9f, 0x65, 0xc0, 0x27}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x33} );
		mailRead( 0x33, 8 ); //EXPECTED {0xa3, 0x59, 0x08, 0xb1, 0x93, 0x25, 0x98, 0x15}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x3b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x59, 0xcc, 0x27, 0xb2, 0x39, 0x8e, 0x2b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x23} );
		mailRead( 0x33, 8 ); //EXPECTED {0xbe, 0x75, 0xe9, 0x3f, 0x76, 0xab, 0xb9, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x2b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0xc7, 0xcc, 0x3f, 0xb2, 0xd9, 0x98, 0x4e}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x13} );
		mailRead( 0x33, 8 ); //EXPECTED {0x8a, 0x3e, 0xad, 0x4b, 0xdd, 0x2f, 0xb8, 0x2b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x1b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xc6, 0x79, 0xec, 0x1f, 0xb2, 0x25, 0x98, 0xf6}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x03} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x4e, 0xf1, 0x26, 0xf4, 0x2a, 0x98, 0x01}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x0b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xc6, 0x79, 0xec, 0x1f, 0x92, 0x05, 0x99, 0xee}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xf3} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe4, 0x5a, 0xea, 0xcb, 0xe3, 0xa0, 0x9c, 0x08}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xfb} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe4, 0x4b, 0xdf, 0xab, 0xa4, 0x22, 0x9e, 0x1a}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xe3} );
		mailRead( 0x33, 8 ); //EXPECTED {0xf3, 0xf8, 0x6e, 0x18, 0xad, 0x35, 0xbb, 0x02}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xeb} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe1, 0x58, 0x4f, 0x3e, 0xb2, 0x25, 0xff, 0x08}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xd3} );
		mailRead( 0x33, 8 ); //EXPECTED {0xea, 0x59, 0xdc, 0x3f, 0x92, 0x08, 0x14, 0x01}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xdb} );
		mailRead( 0x33, 8 ); //EXPECTED {0x46, 0x4d, 0x9d, 0xcf, 0xa4, 0x25, 0xbe, 0x77}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xc3} );
		mailRead( 0x33, 8 ); //EXPECTED {0xa5, 0x59, 0x08, 0xb1, 0x93, 0x25, 0x98, 0x93}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xcb} );
		mailRead( 0x33, 8 ); //EXPECTED {0x6a, 0x53, 0x1c, 0xb5, 0x92, 0xc5, 0xb5, 0x1b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xb3} );
		mailRead( 0x33, 8 ); //EXPECTED {0xf6, 0x67, 0x5a, 0x3f, 0x76, 0xab, 0xb9, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xbb} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x40, 0xcd, 0x22, 0xb2, 0x57, 0xc9, 0xdb}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xa3} );
		mailRead( 0x33, 8 ); //EXPECTED {0xf8, 0x79, 0xa2, 0x17, 0xe7, 0x25, 0x5c, 0x85}
		mailWrite( 0x33, VC{0xa1, 0x08, 0xab} );
		mailRead( 0x33, 8 ); //EXPECTED {0xc7, 0x59, 0xcc, 0x20, 0xb3, 0x38, 0x18, 0x13}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x93} );
		mailRead( 0x33, 8 ); //EXPECTED {0x97, 0x45, 0xda, 0x1f, 0xea, 0x09, 0xbd, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x9b} );
		mailRead( 0x33, 8 ); //EXPECTED {0x22, 0xd7, 0xed, 0x3f, 0xb2, 0xbb, 0x98, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x83} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x59, 0xcc, 0x3f, 0xb2, 0x25, 0x98, 0x0b}
		mailWrite( 0x33, VC{0xa1, 0x08, 0x8b} );
		mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x59, 0xcc, 0x3f, 0xb2, 0x25, 0x98, 0x04}
		mailWrite( 0x33, VC{0xaa, 0xb8, 0x29, 0xe7} );
	}
	mailWrite( 0x4c, VC{0x65, 0x88} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0x10, 0x89} );
	mailWrite( 0x4c, VC{0x11, 0xfc} );
	mailWrite( 0x4c, VC{0x12, 0xc8} );
	mailWrite( 0x4c, VC{0x13, 0x8b} );
	mailWrite( 0x4c, VC{0x15, 0x88} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xed} );
	mailRead( 0x33, 1 ); //EXPECTED {0xec}
	mailWrite( 0x33, VC{0x99, 0x8a, 0xbf} );
	mailRead( 0x33, 2 ); //EXPECTED {0x6e, 0xe1}
	mailWrite( 0x33, VC{0x99, 0x89, 0xb8} );

	std::vector<uint8_t> readValue=mailRead( 0x33, 1 );
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x02}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x02}
	mailWrite( 0x33, VC{0x10, 0x00, 0x33} );

	//I have a theory that this is set to ceil( .95 * Theory read point above
	uint8_t setValue=std::ceil(.95 * readValue[0]);
	mailWrite( 0x33, VC{0x10, 0x01, setValue} );
	mailWrite( 0x33, VC{0x10, 0x02, 0x71} );
	mailWrite( 0x33, VC{0x10, 0x03, 0x72} );

	//Not sure if I should subtract 80 to min zero, or
	//just clear the bit. We'll just clear it.
	setValue &= ~0x80;
	mailWrite( 0x33, VC{0x10, 0x01, setValue} );

	mailWrite( 0x4c, VC{0x04, 0x95} );

	if ( deviceType_ != DeviceType::GameCaptureHDNew ) {
		mailWrite( 0x4c, VC{0x0f, 0x88} );
		mailWrite( 0x4c, VC{0xc1, 0x89} );
		mailWrite( 0x4c, VC{0xc6, 0x8b} );
	}
	mailWrite( 0x33, VC{0x99, 0x89, 0x8b} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0x70, 0xc8} );
	mailWrite( 0x4c, VC{0x90, 0x88} );
	mailWrite( 0x4c, VC{0x91, 0x77} );
	mailWrite( 0x4c, VC{0x92, 0x77} );
	mailWrite( 0x4c, VC{0x93, 0x77} );
	mailWrite( 0x4c, VC{0x94, 0x77} );
	mailWrite( 0x4c, VC{0x95, 0x77} );
	mailWrite( 0x4c, VC{0x96, 0x77} );
	mailWrite( 0x4c, VC{0x97, 0x77} );
	mailWrite( 0x4c, VC{0x98, 0x77} );
	mailWrite( 0x4c, VC{0x99, 0x77} );
	mailWrite( 0x4c, VC{0x9a, 0x77} );
	mailWrite( 0x4c, VC{0x9b, 0x77} );
	mailWrite( 0x4c, VC{0x9c, 0x77} );
	mailWrite( 0x4c, VC{0x9d, 0x77} );
	mailWrite( 0x4c, VC{0x9e, 0x77} );
	mailWrite( 0x4c, VC{0x9f, 0x77} );
	mailWrite( 0x4c, VC{0xa0, 0x77} );
	mailWrite( 0x4c, VC{0xa1, 0x77} );
	mailWrite( 0x4c, VC{0xa2, 0x77} );
	mailWrite( 0x4c, VC{0xa3, 0x77} );
	mailWrite( 0x33, VC{0x99, 0x89, 0x6b} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0xc0, 0x80} );
	mailWrite( 0x4c, VC{0xa2, 0x77} );
	mailWrite( 0x33, VC{0x99, 0x89, 0x58} );
	mailRead( 0x33, 1 ); //EXPECTED {0x91}
	mailWrite( 0x4c, VC{0xc0, 0x77} );
	mailWrite( 0x33, VC{0x99, 0x89, 0x89} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0x72, 0x88} );
	mailWrite( 0x4c, VC{0xc0, 0x89} );
	mailWrite( 0x4c, VC{0x61, 0x98} );
	mailWrite( 0x4c, VC{0x63, 0x89} );

	//Mystery setting difference.
	//I have a theory that if you take the number of scan lines, add 30, and then divide
	//by 5, you get what is supposed to be programmed here.
	if (settings_->getInputResolution() == Resolution::HD720) {
		mailWrite( 0x4c, VC{0x64, 0x96} );
	} else { //Assumed 1080
		mailWrite( 0x4c, VC{0x64, 0xde} );
	}
	mailWrite( 0x4c, VC{0x65, 0x88} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xff} );
	mailRead( 0x33, 1 ); //EXPECTED {0x73}
	mailWrite( 0x4c, VC{0x04, 0x8d} );

	//Mystery setting difference.
	if (settings_->getInputResolution() == Resolution::HD720) {
		mailWrite( 0x4c, VC{0x62, 0x91} );
	} else { //Assumed 1080
		mailWrite( 0x4c, VC{0x62, 0x01} );
	}
	mailWrite( 0x4c, VC{0x0f, 0x89} );
	mailWrite( 0x4c, VC{0x58, 0xd8} );
	mailWrite( 0x4c, VC{0x59, 0x20} );
	mailWrite( 0x4c, VC{0x5a, 0x88} );

	//Mystery setting difference.
	if (settings_->getInputResolution() == Resolution::HD720) {
		mailWrite( 0x4c, VC{0x5b, 0x8c} );
	} else { //Assumed 1080
		mailWrite( 0x4c, VC{0x5b, 0x98} );
	}
	mailWrite( 0x4c, VC{0x5c, 0x88} );
	mailWrite( 0x4c, VC{0x5e, 0x88} );
	mailWrite( 0x4c, VC{0x5f, 0x88} );
	mailWrite( 0x4c, VC{0x60, 0x88} );
	mailWrite( 0x4c, VC{0x61, 0x88} );
	mailWrite( 0x4c, VC{0x62, 0x88} );
	mailWrite( 0x4c, VC{0x63, 0x88} );
	mailWrite( 0x4c, VC{0x64, 0x88} );
	mailWrite( 0x4c, VC{0x65, 0x88} );
	//Mystery setting difference.

	uint8_t value;
	if (settings_->getInputResolution() == Resolution::HD720) {
		value = 0xfb;
	} else { //Assumed 1080
		value = 0xef;
	}
	mailWrite( 0x4c, VC{0x5d, value} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0xcd, 0x8b} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xff} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6b}
	mailWrite( 0x4c, VC{0x04, 0x8d} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	while (( mailRead( 0x33, 1 )[0] & 0x90) == 0) //EXPECTED {0x02}
		mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x02}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x92}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x92}
	mailWrite( 0x33, VC{0x99, 0x89, 0xfd} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x33, VC{0x99, 0x89, 0xfc} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf3} );
	mailRead( 0x33, 1 ); //EXPECTED {0x7e}
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x92}
	mailWrite( 0x4c, VC{0x0d, 0xc8} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xf5} );
	mailRead( 0x33, 1 ); //EXPECTED {0x92}
	mailWrite( 0x4c, VC{0x0e, 0x75} );
	mailWrite( 0x4c, VC{0x0e, 0x74} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0xc1, 0x89} );
	mailWrite( 0x4c, VC{0xc6, 0x8b} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0x61, 0x88} );

	mailWrite( 0x33, VC{0x10, 0x00, 0x33} );
	mailWrite( 0x33, VC{0x10, 0x01, 0xe3} );
	mailWrite( 0x33, VC{0x10, 0x02, 0x71} );
	mailWrite( 0x33, VC{0x10, 0x03, 0x72} );
	mailWrite( 0x33, VC{0x10, 0x01, 0x63} );
	mailWrite( 0x4c, VC{0x0f, 0x89} );
	//Mystery setting difference.
	if (settings_->getInputResolution() == Resolution::HD720) {
		mailWrite( 0x4c, VC{0x33, 0x08} );
		mailWrite( 0x4c, VC{0x34, 0xa5} );
	} else { //Assumed 1080
		mailWrite( 0x4c, VC{0x33, 0x88} );
		mailWrite( 0x4c, VC{0x34, 0x90} );
	}
	mailWrite( 0x4c, VC{0x35, 0x88} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0xc5, 0x88} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xa3} );
	mailRead( 0x33, 1 ); //EXPECTED {0x7f}
	mailWrite( 0x4c, VC{0x58, 0x9d} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xff} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6b}
	mailWrite( 0x4c, VC{0x04, 0x8d} );
	mailWrite( 0x4c, VC{0xe2, 0x6c} );
	mailWrite( 0x4c, VC{0xe3, 0x88} );
	mailWrite( 0x4c, VC{0xe4, 0x80} );
	mailWrite( 0x4c, VC{0xe0, 0x49} );
	mailWrite( 0x4c, VC{0xe1, 0x89} );
	mailWrite( 0x4c, VC{0x0f, 0x89} );
	mailWrite( 0x4c, VC{0x91, 0x88} );
	mailWrite( 0x4c, VC{0x92, 0x88} );
	mailWrite( 0x4c, VC{0x93, 0x89} );
	mailWrite( 0x4c, VC{0x94, 0xa9} );
	mailWrite( 0x4c, VC{0x95, 0xcb} );
	mailWrite( 0x4c, VC{0x96, 0xed} );
	mailWrite( 0x4c, VC{0x97, 0x0f} );
	mailWrite( 0x4c, VC{0x98, 0x8a} );
	mailWrite( 0x4c, VC{0x99, 0x53} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xff} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6b}
	mailWrite( 0x4c, VC{0x04, 0x8c} );
	mailWrite( 0x4c, VC{0x0f, 0x89} );
	mailWrite( 0x4c, VC{0x68, 0x89} );
	mailWrite( 0x4c, VC{0x6b, 0x88} );
	mailWrite( 0x4c, VC{0x6c, 0x88} );
	mailWrite( 0x4c, VC{0x6d, 0xf8} );
	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0xce, 0x8b} );
	mailWrite( 0x33, VC{0x99, 0x89, 0xff} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6a}
	mailWrite( 0x4c, VC{0x04, 0x88} );

	mailWrite( 0x4c, VC{0x0f, 0x88} );
	mailWrite( 0x4c, VC{0xc1, 0x88} );
	mailWrite( 0x4c, VC{0xc6, 0x8b} );
	readDevice0x9DCD(0x3f); //EXPECTED 0xb2
	mailWrite( 0x4e, VC{0x00, 0xce} );
	readDevice0x9DCD(0x6a); //EXPECTED 0xb3
	readDevice0x9DCD(0xa7); //EXPECTED 0xb2
	readDevice0x9DCD(0x69); //EXPECTED 0xb2
	readDevice0x9DCD(0x3f); //EXPECTED 0xb0
	mailWrite( 0x4e, VC{0x00, 0xcc} );
	readDevice0x9DCD(0x66); //EXPECTED 0xba
	readDevice0x9DCD(0x65); //EXPECTED 0x97
	mailWrite( 0x4c, VC{0x0f, 0x89} );
	mailWrite( 0x33, VC{0x99, 0x89, 0x5b} );
	mailRead( 0x33, 1 ); //EXPECTED {0x7c}
	mailWrite( 0x33, VC{0x99, 0x89, 0x5a} );
	mailRead( 0x33, 1 ); //EXPECTED {0x2a}
	mailWrite( 0x33, VC{0x99, 0x89, 0x59} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6c}

	mailWrite( 0x33, VC{0x99, 0x89, 0xc8} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x33, VC{0x99, 0x89, 0xcf} );
	mailRead( 0x33, 1 ); //EXPECTED {0x76}
	mailWrite( 0x33, VC{0x99, 0x89, 0xce} );
	mailRead( 0x33, 1 ); //EXPECTED {0x6e}
	mailWrite( 0x4c, VC{0x0f, 0x88} );

	mailWrite( 0x33, VC{0xaa, 0xb8, 0x29, 0xf6} );
	mailWrite( 0x33, VC{0xa1, 0x08, 0x73} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0xa6, 0x33, 0xc0, 0x4d, 0xda, 0x67, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x7b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xf2, 0xb8, 0xcd, 0x3f, 0xb2, 0x25, 0x98, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x63} );
	mailRead( 0x33, 8 ); //EXPECTED {0xf7, 0x4e, 0xcd, 0x3c, 0x32, 0x25, 0x9a, 0x73}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x6b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xfc, 0x96, 0xb8, 0x9c, 0xe5, 0x69, 0x28, 0x28}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x53} );
	mailRead( 0x33, 8 ); //EXPECTED {0xef, 0x11, 0x80, 0x3f, 0xb2, 0x25, 0x19, 0xcb}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x5b} );
	mailRead( 0x33, 8 ); //EXPECTED {0x37, 0x99, 0xcd, 0x3e, 0xb3, 0x24, 0x99, 0x0a}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x43} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe7, 0x58, 0xcd, 0x3e, 0xb3, 0x24, 0x9a, 0x31}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x4b} );
	mailRead( 0x33, 8 ); //EXPECTED {0x66, 0x41, 0xbd, 0x07, 0x9f, 0x65, 0xc0, 0x27}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x33} );
	mailRead( 0x33, 8 ); //EXPECTED {0xa3, 0x59, 0x08, 0xb1, 0x93, 0x25, 0x98, 0x15}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x3b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x59, 0xcc, 0x27, 0xb2, 0x39, 0x8e, 0x2b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x23} );
	mailRead( 0x33, 8 ); //EXPECTED {0xbe, 0x75, 0xe9, 0x3f, 0x76, 0xab, 0xb9, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x2b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0xc7, 0xcc, 0x3f, 0xb2, 0xd9, 0x98, 0x4e}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x13} );
	mailRead( 0x33, 8 ); //EXPECTED {0x8a, 0x3e, 0xad, 0x4b, 0xdd, 0x2f, 0xb8, 0x2b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x1b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xc6, 0x79, 0xec, 0x1f, 0xb2, 0x25, 0x98, 0xf6}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x03} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x4e, 0xf1, 0x26, 0xf4, 0x2a, 0x98, 0x01}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x0b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xc6, 0x79, 0xec, 0x1f, 0x92, 0x05, 0x99, 0xee}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xf3} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe4, 0x5a, 0xea, 0xcb, 0xe3, 0xa0, 0x9c, 0x08}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xfb} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe4, 0x4b, 0xdf, 0xab, 0xa4, 0x22, 0x9e, 0x1a}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xe3} );
	mailRead( 0x33, 8 ); //EXPECTED {0xf3, 0xf8, 0x6e, 0x18, 0xad, 0x35, 0xbb, 0x02}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xeb} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe1, 0x58, 0x4f, 0x3e, 0xb2, 0x25, 0xff, 0x08}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xd3} );
	mailRead( 0x33, 8 ); //EXPECTED {0xea, 0x59, 0xdc, 0x3f, 0x92, 0x08, 0x14, 0x01}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xdb} );
	mailRead( 0x33, 8 ); //EXPECTED {0x46, 0x4d, 0x9d, 0xcf, 0xa4, 0x25, 0xbe, 0x77}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xc3} );
	mailRead( 0x33, 8 ); //EXPECTED {0xa5, 0x59, 0x08, 0xb1, 0x93, 0x25, 0x98, 0x93}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xcb} );
	mailRead( 0x33, 8 ); //EXPECTED {0x6a, 0x53, 0x1c, 0xb5, 0x92, 0xc5, 0xb5, 0x1b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xb3} );
	mailRead( 0x33, 8 ); //EXPECTED {0xf6, 0x67, 0x5a, 0x3f, 0x76, 0xab, 0xb9, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xbb} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x40, 0xcd, 0x22, 0xb2, 0x57, 0xc9, 0xdb}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xa3} );
	mailRead( 0x33, 8 ); //EXPECTED {0xf8, 0x79, 0xa2, 0x17, 0xe7, 0x25, 0x5c, 0x85}
	mailWrite( 0x33, VC{0xa1, 0x08, 0xab} );
	mailRead( 0x33, 8 ); //EXPECTED {0xc7, 0x59, 0xcc, 0x20, 0xb3, 0x38, 0x18, 0x13}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x93} );
	mailRead( 0x33, 8 ); //EXPECTED {0x97, 0x45, 0xda, 0x1f, 0xea, 0x09, 0xbd, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x9b} );
	mailRead( 0x33, 8 ); //EXPECTED {0x22, 0xd7, 0xed, 0x3f, 0xb2, 0xbb, 0x98, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x83} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x59, 0xcc, 0x3f, 0xb2, 0x25, 0x98, 0x0b}
	mailWrite( 0x33, VC{0xa1, 0x08, 0x8b} );
	mailRead( 0x33, 8 ); //EXPECTED {0xe6, 0x59, 0xcc, 0x3f, 0xb2, 0x25, 0x98, 0x04}
	mailWrite( 0x33, VC{0xaa, 0xb8, 0x29, 0xe7} );


	readSignalInformation( true, true, sum6463, countSum6463, sum6665, countSum6665, rgbBit);

	if( settings_->getColorSpace()==ColorSpace::Unknown ) {
		if( rgbBit ) {
			settings_->setColorSpace(ColorSpace::RGB);
		} else {
			settings_->setColorSpace(ColorSpace::YUV);
		}
	}

	if( settings_->getColorSpace()==ColorSpace::YUV ) {
		//Set up color space.
		mailWrite( 0x4e, VC{0x00, 0xcc} );
		mailWrite( 0x4e, VC{0x92, 0xaa} );
		mailWrite( 0x4e, VC{0x93, 0xdc} );
		mailWrite( 0x4e, VC{0x94, 0xcc} );
		mailWrite( 0x4e, VC{0x95, 0xcc} );
		mailWrite( 0x4e, VC{0x96, 0xcc} );
		mailWrite( 0x4e, VC{0x97, 0xcc} );
		mailWrite( 0x4e, VC{0x98, 0xcc} );
		mailWrite( 0x4e, VC{0x99, 0xcc} );
		mailWrite( 0x4e, VC{0x9a, 0xcc} );
		mailWrite( 0x4e, VC{0x9b, 0xdc} );
		mailWrite( 0x4e, VC{0x9c, 0xcc} );
		mailWrite( 0x4e, VC{0x9d, 0xcc} );
		mailWrite( 0x4e, VC{0x9e, 0xcc} );
		mailWrite( 0x4e, VC{0x9f, 0xcc} );
		mailWrite( 0x4e, VC{0xa0, 0xcc} );
		mailWrite( 0x4e, VC{0xa1, 0xcc} );
		mailWrite( 0x4e, VC{0xa2, 0xcc} );
		mailWrite( 0x4e, VC{0xa3, 0xdc} );
		mailWrite( 0x4e, VC{0xa4, 0xcc} );
		mailWrite( 0x4e, VC{0xa5, 0xec} );
		mailWrite( 0x4e, VC{0xa6, 0xcc} );
		mailWrite( 0x4e, VC{0xa7, 0xc8} );
		mailWrite( 0x4e, VC{0xa8, 0xcd} );
		mailWrite( 0x4e, VC{0xa9, 0xec} );
		mailWrite( 0x4e, VC{0xaa, 0xcc} );
	} else {
		mailWrite( 0x4e, VC{0x00, 0xcc} );
		mailWrite( 0x4e, VC{0x92, 0x8c} );
		mailWrite( 0x4e, VC{0x93, 0xcb} );
		mailWrite( 0x4e, VC{0x94, 0xca} );
		mailWrite( 0x4e, VC{0x95, 0xce} );
		mailWrite( 0x4e, VC{0x96, 0x22} );
		mailWrite( 0x4e, VC{0x97, 0xb2} );
		mailWrite( 0x4e, VC{0x98, 0xae} );
		mailWrite( 0x4e, VC{0x99, 0xb5} );
		mailWrite( 0x4e, VC{0x9a, 0x52} );
		mailWrite( 0x4e, VC{0x9b, 0xc5} );
		mailWrite( 0x4e, VC{0x9c, 0x1f} );
		mailWrite( 0x4e, VC{0x9d, 0xb6} );
		mailWrite( 0x4e, VC{0x9e, 0x54} );
		mailWrite( 0x4e, VC{0x9f, 0xb3} );
		mailWrite( 0x4e, VC{0xa0, 0x90} );
		mailWrite( 0x4e, VC{0xa1, 0xcc} );
		mailWrite( 0x4e, VC{0xa2, 0x32} );
		mailWrite( 0x4e, VC{0xa3, 0xcb} );
		mailWrite( 0x4e, VC{0xa4, 0xca} );
		mailWrite( 0x4e, VC{0xa5, 0xec} );
		mailWrite( 0x4e, VC{0xa6, 0xcc} );
		mailWrite( 0x4e, VC{0xa7, 0xc8} );
		mailWrite( 0x4e, VC{0xa8, 0xcd} );
		mailWrite( 0x4e, VC{0xa9, 0xec} );
		mailWrite( 0x4e, VC{0xaa, 0xcc} );
	}
	mailWrite( 0x4e, VC{0xb2, 0xcc} );
	mailWrite( 0x4e, VC{0xb5, 0xc4} );


	transcoderFinalConfigure();
	transcoderSetup();

	scmd(SCMD_INIT, 0xa0, 0x0000);
	uint16_t state=read_config<uint16_t>(SCMD_STATE_READBACK_REGISTER); //EXPECTED=0x0001
	scmd(SCMD_STATE_CHANGE, 0x00, SCMD_STATE_START);
	completeStateChange(state, SCMD_STATE_START); //EXPECTED 0x0001
}




