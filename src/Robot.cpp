/***********************************
EctoRebirth 2019 By:               *
Abiel Fernandez                    *
Alberto Jahuey                     *
Karen Rodriguez                    *
Carrillo Josephino                 *
 ***********************************/

/*
class Robot : public EctoRobot {
private:
   NavX &gyro = NavX::getInstance();
   EctoInput &stick = EctoInput::getInstance();
   std::shared_ptr<spdlog::logger> log = spd::get("log");
   MasterOfPuppets &masterOfPuppets = MasterOfPuppets::getInstance();
   CubeIntake &cubeIntake = CubeIntake::getInstance();
   CubeWinch &cubeWinch = CubeWinch::getInstance();
   TankState &state = TankState::getInstance();

   int times = 0;

   //FeedForwardCalculator &calculator = FeedForwardCalculator::getInstance();

public:
   void RobotInit() override {
	   log->info("Robot Init");
	   gyro.Reset();
	   gyro.ZeroYaw();
	   //manager->initTelemetry();
	   manager->printRegisteredSubsystems();
	   cout << "stick" << endl;
	   EctoInput::getInstance().initialize();
	   stick.getButtonState("X");

	   auto cameraTable1 = NetworkTable::GetTable("/CameraPublisher/PiCamera1");
	   auto cameraTable2 = NetworkTable::GetTable("/CameraPublisher/PiCamera2");

	   string cameraKey1[] = {"mjpeg:http://10.46.35.111:1181/?action=stream"};
	   string cameraKey2[] = {"mjpeg:http://10.46.35.111:1182/?action=stream"};

	   //NetworkTable::GetKeys();

	   cameraTable1->PutStringArray("streams", cameraKey1);
	   cameraTable2->PutStringArray("streams", cameraKey2);
	   //NetworkTable::GetInstance().GetSubTable("/CameraPublisher/PiCamera/streams").;
   }

   void RobotPeriodic() override {
	   RobotPose2D &current = *state.getLastState();
	   SmartDashboard::PutNumber("x", current.getX());
	   SmartDashboard::PutNumber("y", current.getY());
	   SmartDashboard::PutNumber("angle", current.getTheta());

	   //cout << current.getX() << ',' << current.getY() << ',' << NavX::getInstance().GetYaw() << endl;

	   manager->updateTelemetry();
   }

   void DisabledInit() override {
	   log->info("Disabled Init");
	   manager->initDisabled();

   }

   void DisabledPeriodic() override {
	   manager->updateDisabled();
   }

   void AutonomousInit() override {

	   //settings.refreshFile();

	   log->info("Autonomous Init");

	   gyro.ZeroYaw();

	   manager->initAuto();
   }

   void AutonomousPeriodic() override {
	   manager->updateAuto();
	   //auto &motors = EctoMotors::getInstance();

	   //motors.getMotor("ElevatorRobotMaster")->Set(.1);
   }

   void TeleopInit() override {
	   log->info("Teleop Init");

	   //EctoInput::getInstance().initialize();
	   settings.refreshFile();
	   manager->initTeleop();

	   EctoInput::getInstance().initialize();
   }

   void TeleopPeriodic() override {
	   auto sticks = EctoInput::getInstance();

	   if (times < 10) {
		   //sticks.initialize();
		   sticks.initializedInput = true;
		   times++;
	   }

	   manager->updateTeleop();
   }

   void TestInit() override {
	   //EctoPCM &pcm = EctoPCM::getInstance();

	   //pcm.setPiston("ClawSwivel", true);
   }

};
*/

#include "2018/Robots/Chappie.h"

//TODO Find a nice way to select the robot class

#ifndef RUNNING_FRC_TESTS

int main() { return frc::StartRobot<Chappie>(); }

#endif