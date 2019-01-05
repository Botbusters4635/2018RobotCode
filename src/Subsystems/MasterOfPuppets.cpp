//
// Created by Abiel on 2/7/18.
//

//
//#include "MasterOfPuppets.h"
//
//
//MasterOfPuppets::MasterOfPuppets() : EctoSubsystem("Master of Puppets") {
//	log->info("Initializing MasterOfPuppets...");
//	initialPosChooser = new SendableChooser<string>();
//	initialPosChooser->AddDefault("Center Pos", "Center");
//	initialPosChooser->AddObject("Left Pos", "Left");
//	initialPosChooser->AddObject("Right Pos", "Right");
//	SmartDashboard::PutData("StartPos Chooser", initialPosChooser);
//	SmartDashboard::PutBoolean("Switch", overrideTrySwitch);
//	SmartDashboard::PutBoolean("Scale", overrideTryScale);
//	log->info("Done");
//
//}
//
//void MasterOfPuppets::limitTankSpeedWithWinch() {
//	double normalizedHeight = cubeWinch.getCurrentHeight() / cubeWinch.getMaxHeight();
//	double speedMultiplier = 1.0 - 0.4 * pow(normalizedHeight, 3.0);
//	//TODO Limit speed
//	//ectoTank.setSpeedMultiplier(speedMultiplier);
//}
//
//void MasterOfPuppets::initTeleop() {
//
//}
//
/*
void MasterOfPuppets::updateTeleop() {
	limitTankSpeedWithWinch();
}

void MasterOfPuppets::initAuto() {
	settings.setCurrentGroup("MasterOfPuppets");

	pathSpeed = settings.getNumber("Speed");
	lookAheadDistance = settings.getNumber("LookAheadDistancePath");
	outSpeedIntakeScale = settings.getNumber("OutSpeedIntakeScale");
	outSpeedIntakeSwitch = settings.getNumber("OutSpeedIntakeSwitch");
	intakeSpeed = settings.getNumber("IntakeSpeed");

	simpleMovementSpeed = settings.getNumber("SimpleMovementSpeed");
	simpleMovementAcceleration = settings.getNumber("SimpleMovementAcceleration");

	distanceToMoveBackFromSwitch = settings.getNumber("DistanceToMoveBackFromSwitch");
	distanceToMoveFromSwitchToCenterCube = settings.getNumber("DistanceToMoveFromSwitchToCenterCube");
	distanceToGrabCube = settings.getNumber("DistanceToGrabCube");

	timeToWaitToGrabCube = settings.getNumber("TimeToWaitToGrabCube");

	posStart = initialPosChooser->GetSelected();

	trySwitch = SmartDashboard::GetBoolean("Switch", overrideTrySwitch);
	tryScale = SmartDashboard::GetBoolean("Scale", overrideTryScale);

	log->info("Starting auto...");
	log->info("Starting pos: {}, trying Switch: {}, trying Scale: {}", posStart, trySwitch, tryScale);
}

void MasterOfPuppets::updateAuto() {
	log->info("Do Switch: {} Do Scale: {} Position: {}, Position Override: {}", trySwitch || overrideTrySwitch,
	          tryScale || overrideTryScale,
	          posStart, overridePosition);

	limitTankSpeedWithWinch();

	MatchData::OwnedSide switchSide = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_NEAR);
	MatchData::OwnedSide scaleSide = MatchData::get_owned_side(MatchData::GameFeature::SCALE);

	if (!hasSetup) {
		startTime = timer.getTime();

		///Wait until info is received, timeout to prevent infinite loop
		while (switchSide == MatchData::OwnedSide::UNKNOWN
		       && timer.getTime() - startTime < 1) {
			MatchData::OwnedSide switchSide = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_NEAR);
			MatchData::OwnedSide scaleSide = MatchData::get_owned_side(MatchData::GameFeature::SCALE);
		}

		if (trySwitch || overrideTrySwitch) {
			switch (switchSide) {
				case MatchData::OwnedSide::LEFT:
					if (posStart == "Center" || overridePosition == "Center") {
						pathName = "Prim_Center_Switch_Left";
					} else if (posStart == "Left" || overridePosition == "Left") {
						pathName = "Prim_Left_Switch_Left";
						//pathName = "Baseline";
					} else if (posStart == "Right" || overridePosition == "Right") {
						pathName = "Prim_Right_Switch_Left";
						//pathName = "Baseline";
					} else {
						pathName = "Prim_Center_Switch_Left";
					}
					break;

				case MatchData::OwnedSide::RIGHT:
					if (posStart == "Center" || overridePosition == "Center") {
						pathName = "Prim_Center_Switch_Right";
					} else if (posStart == "Left" || overridePosition == "Left") {
						//pathName = "Prim_Left_Switch_Right";
						pathName = "Baseline";
					} else if (posStart == "Right" || overridePosition == "Right") {
						pathName = "Prim_Right_Switch_Right";
						//pathName = "Baseline";
					}
					break;


				case MatchData::OwnedSide::UNKNOWN:
					pathName = "Baseline";
					break;

			}
		} else if (tryScale || overrideTryScale) {
			switch (scaleSide) {
				case MatchData::OwnedSide::LEFT:
					if (posStart == "Center" || overridePosition == "Center") {
						//pathName = "Prim_Center_Scale_Left";
						pathName = "Baseline";
					} else if (posStart == "Left" || overridePosition == "Left") {
						pathName = "Prim_Left_Scale_Left";
						//pathName = "Baseline";
					} else if (posStart == "Right" || overridePosition == "Right") {
						pathName = "Prim_Right_Scale_Left";
						//pathName = "Baseline";
					}
					break;

				case MatchData::OwnedSide::RIGHT:
					if (posStart == "Center" || overridePosition == "Center") {
						//pathName = "Prim_Center_Scale_Right";
						pathName = "Baseline";
					} else if (posStart == "Left" || overridePosition == "Left") {
						//pathName = "Prim_Left_Scale_Right";
						pathName = "Prim_Left_Scale_Right";
					} else if (posStart == "Right" || overridePosition == "Right") {
						//pathName = "Prim_Right_Switch_Right";
						pathName = "Prim_Right_Scale_Right";
					}
					break;


				case MatchData::OwnedSide::UNKNOWN:
					cerr << "UNK" << endl;
					pathName = "Baseline";
					break;
			}
		} else {
			pathName = "Baseline";
		}

		*/
/*
		if(scaleSide == MatchData::OwnedSide::RIGHT){
			pathName = "Prim_Right_Scale_Right";
		} else {
			pathName = "Baseline";
		}
		*//*


		path = parser.getPath(pathName);


		runner.reset(new PurePursuitRunner(*path, lookAheadDistance));
		hasSetup = true;
	}

	hasFinished = runner->isFinished();

	if (!hasFinished) {
		//cout << pathName << endl;

		if (runner->getPercentCompleted() > 30) {
			//TODO Determine correct height
			//cubeWinch.setPosition(ScaleHeight);

			if (trySwitch || overrideTrySwitch)
				cubeWinch.setPosition(SwitchHeight);
			else if (tryScale || overrideTryScale)
				cubeWinch.setPosition(ScaleHeight);
		}

		if (runner->getPercentCompleted() > 98.5) {
			if (pathName != "BaseLine") {
				if (trySwitch || overrideTrySwitch) {
					cubeIntake.eject(outSpeedIntakeSwitch);
				} else if (tryScale || overrideTryScale) {
					cubeIntake.eject(outSpeedIntakeScale);
				}
			}
		}

		runner->runPath(ectoTank, pathSpeed);
	} else if (trySecondCube) {
		if (!movedBack) {
			log->info("Moving back");
			//Move in reverse from switch

			//ectoTank.driveDistanceMagicMotion(simpleMovementSpeed, simpleMovementAcceleration,
			//                                 -2.0);
			cubeWinch.setPosition(CurrentPosition::CubeHeight);

			movedBack = true;
		} else if (!pilePathSetup && !wentToCubePath) {
			string pathName;

			log->info("Setting up pile path");

			double zeroTime = timer.getTime();

			while (timer.getTime() - zeroTime < 1.5) {
				cubeWinch.setPosition(CurrentPosition::CubeHeight);
				cubeIntake.intake(intakeSpeed);
			}

			if (switchSide == MatchData::OwnedSide::RIGHT) {
				pathName = "BackSwitchRight_CenterPile";
			} else {
				pathName = "BackSwitchLeft_CenterPile";
			}

			path = parser.getPath(pathName);
			runner.reset(new PurePursuitRunner(*path, lookAheadDistance));

			pilePathSetup = true;
		} else if (!wentToCubePath) {
			log->info("Running Cube Path");

			if (!runner->isFinished()) {
				cubeIntake.intake(intakeSpeed);
				runner->runPath(ectoTank, pathSpeed);
			} else {
				wentToCubePath = true;
			}
		} else if (!droveBack) {
			log->info("Driving back");

			//ectoTank.driveDistanceMagicMotion(simpleMovementSpeed, simpleMovementAcceleration,
			// -1.5);
			droveBack = true;
		} else if (!returnToSwitchSetup && !returnedToSwitch) {
			log->info("Setting up return to switch");

			if (switchSide == MatchData::OwnedSide::RIGHT)
				pathName = "ReturnToSwitch_Right";
			else
				pathName = "ReturnToSwitch_Left";

			path = parser.getPath(pathName);
			runner.reset(new PurePursuitRunner(*path, lookAheadDistance));

			returnToSwitchSetup = true;
		} else if (!returnedToSwitch) {
			log->info("Returning to switch");

			if (!runner->isFinished()) {
				cubeIntake.intake(intakeSpeed);
				cubeWinch.setPosition(CurrentPosition::SwitchHeight);

				runner->runPath(ectoTank, pathSpeed);
			} else {
				cubeIntake.eject(outSpeedIntakeSwitch);
				wentToCubePath = true;
			}
		}
	} else if (onlyReturn) {
		//ectoTank.driveDistanceMagicMotion(simpleMovementSpeed, simpleMovementAcceleration,
		//                                  distanceToMoveBackFromSwitch);

		cubeIntake.stop();

		cubeWinch.setPosition(CubeHeight);

		onlyReturn = false;
	}
*/



// if (posStart == Center) {
// 	if (tryCross == false) {
// 		tryScale = false;
// 	}
// }
// if ((trySwitch && tryScale) && !tryCube) {
// 	tryCube = true;
// }
// if (hasRun == false){
// 	if (trySwitch){
// 		switch (switchSide){
// 			case UNKNOWN:

// 			break;
// 			case LEFT:
// 				switch(posStart){
// 					case Center:
// 						currentPath = *parser.getPath("Prim_Center_Switch_Left");
// 					break;
// 					case Left:
// 						currentPath = *parser.getPath("Prim_Left_Switch_Left");
// 					break;
// 					case Right:
// 						currentPath = *parser.getPath("Prim_Right_Switch_Left");
// 					break;
// 				}
// 			break;
// 			case RIGHT:
// 				switch(posStart){
// 					case Center:
// 						currentPath = *parser.getPath("Prim_Center_Switch_Right");
// 					break;
// 					case Left:
// 						currentPath = *parser.getPath("Prim_Left_Switch_Right");
// 					break;
// 					case Right:
// 						currentPath = *parser.getPath("Prim_Right_Switch_Right");
// 					break;
// 				}
// 			break;
// 		}
// 		runPath (*currentPath, pathSpeed);

// 		if (tryCube && !tryScale){
// 			switch(switchSide){
// 				case Center:

// 				break;
// 				case Left:
// 					currentPath = *parser.getPath("Sec_Switch_Left_Cube_Left");
// 				break;
// 				case Right:
// 					currentPath = *parser.getPath("Sec_Switch_Right_Cube_Right");
// 				break;
// 			}
// 		}else if (tryCube && tryScale){
// 			switch(switchSide){
// 				case UNKNOWN:

// 				break;
// 				case Left:
// 					currentPath = *parser.getPath("Sec_Switch_Left_Cube_Left");
// 				break;
// 				case Right:
// 					currentPath = *parser.getPath("Sec_Switch_Right_Cube_Right");
// 				break;
// 			}
// 			runPath (*currentPath, pathSpeed);

// 			switch(scaleSide){
// 				case Center:

// 				break;
// 				case Left:
// 					currentPath = *parser.getPath("Ter_Cube_Left_Scale_Left");
// 				break;
// 				case Right:
// 					currentPath = *parser.getPath("Ter_Cube_Right_Scale_Right");
// 				break;
// 			}
// 			runPath (*currentPath, pathSpeed);
// 		}
// 	}

// 	else if (tryScale){
// 		switch(scaleSide){
// 			case UNKNOWN:
// 			break;
// 			case LEFT:
// 				switch(posStart){
// 					case Center:
// 						currentPath = *parser.getPath("Prim_Center_Scale_Left");
// 					break;
// 					case Left:
// 						currentPath = *parser.getPath("Prim_Left_Scale_Left");
// 					break;
// 					case Right:
// 						currentPath = *parser.getPath("Prim_Right_Scale_Left");
// 					break;
// 				}
// 			break;
// 			case RIGHT:
// 				switch(posStart){
// 					case Center:
// 						currentPath = *parser.getPath("Prim_Center_Scale_Right");
// 					break;
// 					case Left:
// 						currentPath = *parser.getPath("Prim_Left_Scale_Right");
// 					break;
// 					case Right:
// 						currentPath = *parser.getPath("Prim_Right_Scale_Right");
// 					break;
// 				}
// 			break;
// 		}
// 		runPath (*currentPath, pathSpeed);
// 	}

// 	else if (onlyBaseline){

// 	}
// 	hasRun = true;
// }

// if (hasRun == true){

// }
//}

//bool MasterOfPuppets::getIntakingStatus() const {
//	return cubeIntake.getIntakingStatus();
//}
//
//void MasterOfPuppets::initDisabled() {
//
//}
//
//void MasterOfPuppets::updateDisabled() {
//	log->info("Do Switch: {} Position: {}", trySwitch, posStart);
//}
//
//void MasterOfPuppets::sendTelemetry() const {
//
//}
//
//void MasterOfPuppets::sendInitialTelemetry() const {
//
//}
