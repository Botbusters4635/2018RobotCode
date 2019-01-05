#include "NavX.h"

NavX::NavX() : AHRS(SPI::kMXP) {
	Reset();
	ResetDisplacement();
	ZeroYaw();
	SmartDashboard::PutString("NavXFirmwareVersion", GetFirmwareVersion());
}