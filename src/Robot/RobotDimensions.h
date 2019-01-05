//
// Created by Abiel on 10/8/18.
//

#ifndef BOTBUSTERSREBIRTH_ROBOTDIMENSIONS_H
#define BOTBUSTERSREBIRTH_ROBOTDIMENSIONS_H

class EctoSettings;

class RobotDimensions {
public:
	RobotDimensions(double robotWidth, double robotHeight, double robotWheelBase, double wheelDiameter);

	RobotDimensions();

	void setRobotWidth(double width);

	double getRobotWidth() const;

	void setRobotHeight(double height);

	double getRobotHeight() const;

	void setRobotWheelBase(double wheelbase);

	double getRobotWheelBase();

	void setWheelDiameter(double wheelDiameter);

	double getWheelDiameter() const;

	void setWheelRadius(double wheelRadius);

	double getWheelRadius() const;

	double getWheelCircumference() const;

	static RobotDimensions getDimensionsFromSettings(EctoSettings &settings);

private:
	double robotWidth, robotHeight;
	double robotWheelBase;

	double wheelDiameter;
	double wheelRadius;

	double wheelCircumference;
};

#endif //BOTBUSTERSREBIRTH_ROBOTDIMENSIONS_H
