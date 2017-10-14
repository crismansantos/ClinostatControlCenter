/*
 Name:		ControlCenter.ino
 Created:	10/1/2017 12:35:29 PM
 Author:	crisp
*/

#include <ThreadController.h>
#include <Thread.h>
#include <SoftwareSerial.h>


const byte ClinostatTX;
const byte ClinostatRX;
const byte GreenHouseTX;
const byte GreenHouseRX;

SoftwareSerial ClinostatSerial = SoftwareSerial(ClinostatRX, ClinostatTX);
SoftwareSerial GreenHouseSerial = SoftwareSerial(GreenHouseRX, GreenHouseTX);

Thread ClinostatWriter;
Thread ClinostatReader;

Thread GreenHouseWriter;
Thread GreenHouseReader;

Thread ControlCenterReader;
Thread ControlCenterWriter;

Thread DisplayWriter;

ThreadController DisplayManager;
ThreadController ClinostatManager;
ThreadController GreenHouseManager;
ThreadController ControlCenterManager;
ThreadController TaskManager;

union Data16bits {
	int16_t Value;
	byte ByteArray[sizeof(int16_t)];
};

union Data32bits {
	int32_t Value;
	byte ByteArray[sizeof(int32_t)];
};

union Data64bits {
	int64_t Value;
	byte ByteArray[sizeof(int64_t)];
};

union uData16bits {
	uint16_t Value;
	byte ByteArray[sizeof(uint16_t)];
};

union uData32bits {
	uint32_t Value;
	byte ByteArray[sizeof(uint32_t)];
};

union uData64bits {
	uint64_t Value;
	byte ByteArray[sizeof(uint64_t)];
};

struct ConfigData {
	uData16bits ExperimentID;
	byte SupportType;
	byte SampleQuantity;
	byte SampleMass;
	uData16bits ExperimentTime;
	uData32bits MinGravitationalAcceleration;
	uData32bits MaxGravitationalAcceleration;
	byte RpmX;
	byte RpmY;
	byte RpmZ;
}ConfigPackage;

struct ExperimentData {
	uData16bits ExperimentID;
	uData32bits ExperimentTime;

	uData16bits Temperature;

	Data16bits AcelerometerX;
	Data16bits AcelerometerY;
	Data16bits AcelerometerZ;

	Data16bits GiroscopeX;
	Data16bits GiroscopeY;
	Data16bits GiroscopeZ;

	Data16bits ResultantAcceleration;
	Data16bits ResultantSpeed;

	Data16bits GravitationalAcceleration;
	Data16bits CentripetalAcceleration;
	Data16bits CentrifugalAcceleration;

	byte RpmX;
	byte RpmY;
	byte RpmZ;

	byte ResultantRotation;
}ExperimentPackage;


byte CalculeCheckSum(byte *PayLoad, byte PackageLenght) {
	uData32bits CheckSum;

	CheckSum.Value = 0;

	for (int i = 0; i < PackageLenght; i++)
		CheckSum.Value += PayLoad[i];

	return (byte)CheckSum.ByteArray[0];
}


void setup() {
	Serial.begin(57600);
	ClinostatSerial.begin(57600);
	GreenHouseSerial.begin(57600);

	DisplayManager.enabled = true;
	DisplayManager.add(&DisplayWriter);

	ClinostatManager.enabled = true;
	ClinostatManager.add(&ClinostatReader);
	ClinostatManager.add(&ClinostatWriter);

	GreenHouseManager.enabled = true;
	GreenHouseManager.add(&GreenHouseReader);
	GreenHouseManager.add(&GreenHouseWriter);

	ControlCenterManager.enabled = true;
	ControlCenterManager.add(&ControlCenterReader);
	ControlCenterManager.add(&ControlCenterWriter);

	TaskManager.enabled = true;
	TaskManager.add(&DisplayManager);
	TaskManager.add(&ClinostatManager);
	TaskManager.add(&GreenHouseManager);
	TaskManager.add(&ControlCenterManager);
}

void loop() {
	if (TaskManager.shouldRun()) TaskManager.run();
}
