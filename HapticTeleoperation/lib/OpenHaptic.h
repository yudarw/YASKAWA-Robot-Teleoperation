#pragma once
#ifndef __OPEN_HAPTIC__
#define __OPEN_HAPTIC__

#include "HD\hd.h"
#include "HL\hl.h"
#include "HDU\hduError.h"
#include "HDU\hduVector.h"
#include "Eigen/Dense"
#include <stdlib.h>
#include <iostream>


extern hduVector3Dd global_haptic_forces;

// Haptic State Struct:
typedef struct
{
	HDint			button;
	hduVector3Dd	position;
	hduVector3Dd	orientation;
	hduVector3Dd	force;
	hduVector3Dd	velocity;
	HDdouble		transform[16];
	hduVector3Dd	angular_velocity;

}DeviceStateStruct;

// Class OpenHaptic:
class OpenHaptic
{
public:

	int		init();
	void	close();
	void	getVelocity(double vel[6]);
	void    getPosition(double pos[6]);
	void	setForces(float forces[3]);
	int		getButtonState();

	// Haptic force effect
	void	effect_friction(float gain, float magnitude);
	void	effect_spring(hduVector3Dd position, float gain, float magnitude);
	void	effect_update(float gain, float magnitude);
	void	effect_stop();

	Eigen::MatrixXd get_matrix(double state[16]);

public:
	HHD		hHD;
	HHLRC	hHLRC;
	HLuint	gEffect;
	float	gGain;
	float	gMagnitude;
	bool	connected;
	float	forces[3];
	float	org_matrix[4][4];
	bool	inverse_frame;

};
#endif
