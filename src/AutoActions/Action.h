//
// Created by Abiel on 8/25/18.
//

#ifndef BOTBUSTERSREBIRTH_ACTION_H
#define BOTBUSTERSREBIRTH_ACTION_H


class Action {
public:
	/**
	 * Ran at the start of the action
	 */
	virtual void init() = 0;

	/**
	 * Ran periodically as long as isFinished is false
	 */
	virtual void update() = 0;

	/**
	 * Returns true when the action has started
	 * @return
	 */
	virtual bool isFinished() const {
		return finished;
	};

	/**
	 * Ran once when isFinished returns true
	 * @return
	 */
	virtual void finish() = 0;

protected:
	bool finished = false;
};


#endif //BOTBUSTERSREBIRTH_ACTION_H
