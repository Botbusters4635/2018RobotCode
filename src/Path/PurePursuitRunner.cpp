//
// Created by Alberto on 11/26/2017.
//

#include "PurePursuitRunner.h"

//TODO Fix this

PurePursuitRunner::PurePursuitRunner(TankState &inState, const PathContainer &inPathContainer, double lookAheadDistance)
		: state(inState),
		  pathContainer(inPathContainer) {

	if (pathContainer.generatedPoints.empty()) {
		throw std::logic_error("Tried to run an empty path!");
	}

	currentTargetWaypoint = 0;
	speedIndex = 0;

	this->lookAheadDistance = lookAheadDistance;

	settings.setCurrentGroup("PurePursuitRunner");

	//How many steps to divide the time to accelerate in
	speedTimeSteps = settings.getNumber<unsigned int>("SpeedTimeStep");

	//How many seconds to accelerate for
	secsToAccel = settings.getNumber("SecsToAccel");

	//Proportional value to use for turns
	turnProportional = settings.getNumber("TurnProportional");

	//Minimum and maximum speed multiplier for turns
	minimumSpeedMultiplier = settings.getNumber("MinimumSpeedMultiplier"); //Usually 1.0
	maximumSpeedMultiplier = settings.getNumber("MaximumSpeedMultiplier");

	//Checks if speed multipliers have been defined in settings in order to prevent "accidents"
	if (minimumSpeedMultiplier == -1.0 || maximumSpeedMultiplier == -1.0)
		throw logic_error(
				"Minimum speed multiplier or maximum speed multiplier for PurePursuitRunner not defined in settings!");

	reduceSpeedWithAngleError = settings.getBool("ReduceSpeedWithAngleError");
}

/**
 * Gets the current time since path was started
 * @return
 */
double PurePursuitRunner::getTime() {
	return timer.getTime() - startTime;
}

/**
 * Runs the given path
 * @param driveTrain
 * @param speed
 */
void PurePursuitRunner::runPath(EctoTank &driveTrain, double speed) {

	//Run once
	if (!started) {
		cout << "Started" << endl;
		cout << "PathName " << pathContainer.pathName << endl;
		log->info("Started running path: {} on: {} ", pathContainer.pathName, driveTrain.getName());
		cout << "her" << endl;

		startTime = timer.getTime();

		double targetSpeed = 0, targetTime = 0;

		double speedStep = speed / speedTimeSteps;
		double timeStep = secsToAccel / speedTimeSteps;

		for (int i = 0; i < speedTimeSteps; ++i) {
			accelerationProfile.emplace_back(make_pair(targetTime, targetSpeed));

			targetTime += timeStep;
			targetSpeed += speedStep;
		}

		cout << "Ended" << endl;

		started = true;
		finished = false;
	}

	//Limits speed to max possible speed
	//if (speed > driveTrain.getMaxSpeed())
	//	speed = driveTrain.getMaxSpeed();

	//True until it reaches the final waypoint, when it does, finished = true
	if (currentTargetWaypoint + 1 < pathContainer.generatedPoints.size()) {
		//Last reported robot state
		RobotPose2D lastState = state.getLastMeasurement().second;

		//Waypoint to move to
		RobotPose2DCurvature targetPose = pathContainer.generatedPoints.at(currentTargetWaypoint);

		//Distance between current position and target
		double distanceTarget = Point2D::getDistance(lastState, targetPose.getPoint());

		//Iterate over path until a point which is within the lookAheadDistance is found
		while (distanceTarget < lookAheadDistance && currentTargetWaypoint + 1 < pathContainer.generatedPoints.size()) {
			targetPose = pathContainer.generatedPoints.at(currentTargetWaypoint);
			distanceTarget = Point2D::getDistance(lastState, targetPose.getPoint());
			currentTargetWaypoint++;
		}

		double angleToTarget = EctoUtils::radToDeg(Point2D::getAngleFromPoints(lastState, targetPose.getPoint()));

		double robotAngle = driveTrain.getYaw();
		double angleToTurn = EctoUtils::shortestAngleTurn(robotAngle, angleToTarget);

		cout << "AngleThingToTurn " << angleToTurn << endl;

		double speedMultiplier = 1.0;

		if (reduceSpeedWithAngleError) {
			//Adjust motor velocity proportionally based on how much angle is needed to turn
			speedMultiplier = turnProportional * std::abs(angleToTurn);

			//Adjusts speed multiplier based on the proportional calculation
			speedMultiplier = 1.0 - speedMultiplier;

			///Apply boundaries to value
			speedMultiplier = EctoUtils::limit(speedMultiplier, minimumSpeedMultiplier, maximumSpeedMultiplier);
		}

		//If currently during acceleration period, get speed from pre-calculated trapezoidal profile
		if (getTime() < secsToAccel) {
			currentTargetSpeed = accelerationProfile[speedIndex].second;

			if (accelerationProfile[speedIndex].first < getTime() && speedIndex + 1 < accelerationProfile.size())
				speedIndex++;
		} else {
			currentTargetSpeed = speed;
		}

		driveTrain.driveFollowingAngle(currentTargetSpeed * speedMultiplier,
		                               driveTrain.getYaw() + angleToTurn);

	} else {
		finished = true;
	}

	//If finished running path, stop the drivetrain
	if (finished)
		driveTrain.stop();
}

bool PurePursuitRunner::isFinished() {
	return finished;
}

/**
 * Returns the current status of path (in percentage)
 * @return
 */
double PurePursuitRunner::getPercentCompleted() const {
	return ((double) currentTargetWaypoint / (double) pathContainer.generatedPoints.size()) * 100.0;
}

