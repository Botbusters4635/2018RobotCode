//
// Created by Abiel on 12/9/18.
//

#include "GyroService.h"

GyroService::GyroService() : EctoSubsystem("GyroService") {
	;
}

void GyroService::initRobot() {
	navx.ResetDisplacement();
	navx.ClearError();
	navx.ZeroYaw();

	watchdog = new EctoWatchdog("GyroService", std::chrono::milliseconds(2000), std::chrono::milliseconds(10),
	                            std::bind(&GyroService::loop, this));
	watchdog->start();
}

void GyroService::sendInitialTelemetry() const {
	SmartDashboard::PutString("NavXFirmwareVersion", navx.GetFirmwareVersion());
}

void GyroService::sendTelemetry() const {
	SmartDashboard::PutNumber("GyroYaw", data.getData<double>("GyroYaw"));
	SmartDashboard::PutNumber("GyroFusedHeading", data.getData<double>("GyroFusedHeading"));
}

void GyroService::loop() {
	double yaw = navx.GetYaw();

	data.addData("GyroYaw", (double) navx.GetYaw());
	//data.addData("GyroYawDegree", (double) EctoMath::radiansToDegrees(yaw));
	data.addData("GyroFusedHeading", (double) navx.GetFusedHeading());
	data.addData("GyroIsMagneticDisturbance", (double) navx.IsMagneticDisturbance());
}