//#include "stdafx.h"
#include "OpenHaptic.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "HomogeneousTransform.h"

HDdouble haptic_pos[3];
HDdouble haptic_ori[3];
HDdouble haptic_transform[16];
HDdouble haptic_velocity[3];
HDint	 haptic_button;
HDdouble haptic_forces[3];
HDdouble angular_velocity[3];

hduVector3Dd global_haptic_forces;

// Haptic Effect :
void OpenHaptic::effect_friction(float gain, float magnitude)
{
	effect_stop();
	hlBeginFrame();
	hlStartEffect(HL_EFFECT_FRICTION, gEffect);
	hlEndFrame();
	effect_update(gain, magnitude);
}
// Set the initial position and generate spring effect to the haptic
void OpenHaptic::effect_spring(hduVector3Dd position, float gain, float magnitude)
{
	effect_stop();
	hlBeginFrame();
	hlEffectdv(HL_EFFECT_PROPERTY_POSITION, position);
	hlStartEffect(HL_EFFECT_SPRING, gEffect);
	hlEndFrame();
	effect_update(gain, magnitude);
}
// Stop the haptic attached effect
void OpenHaptic::effect_stop()
{
	hlBeginFrame();
	HLboolean bActive = false;
	hlGetEffectbv(gEffect, HL_EFFECT_PROPERTY_ACTIVE, &bActive);
	if (bActive)
		hlStopEffect(gEffect);
	hlEndFrame();
}
void OpenHaptic::effect_update(float gain, float magnitude)
{
	hlBeginFrame();
	HLboolean bActive = false;
	hlGetEffectbv(gEffect, HL_EFFECT_PROPERTY_ACTIVE, &bActive);
	if (bActive) {
		hlEffectd(HL_EFFECT_PROPERTY_GAIN, gain);
		hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, magnitude);
		hlUpdateEffect(gEffect);
	}
	hlEndFrame();
}

// New method using Eigen lib :
MatrixXd OpenHaptic::get_matrix(double h_transform[16])
{
	MatrixXd M(3, 3);
	M(0, 0) = h_transform[0];
	M(0, 1) = h_transform[1];
	M(0, 2) = h_transform[2];
	M(1, 0) = h_transform[4];
	M(1, 1) = h_transform[5];
	M(1, 2) = h_transform[6];
	M(2, 0) = h_transform[8];
	M(2, 1) = h_transform[9];
	M(2, 2) = -h_transform[10];
	return M;
}


// -- Callback function -- 
// /////////////////////////////////////////////////////////////
HDCallbackCode HDCALLBACK getStateCallback(void * data)
{
	hduVector3Dd feedback_forces;

	// Start frame
	hdBeginFrame(hdGetCurrentDevice());

	// Get haptic data:
	hdGetDoublev(HD_CURRENT_POSITION,		haptic_pos);
	hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES,	haptic_ori);
	hdGetDoublev(HD_CURRENT_TRANSFORM,		haptic_transform);
	hdGetIntegerv(HD_CURRENT_BUTTONS,		&haptic_button);
	hdGetDoublev(HD_CURRENT_VELOCITY,		haptic_velocity);
	hdGetDoublev(HD_CURRENT_ANGULAR_VELOCITY, angular_velocity);

	// Set force feedback:
	feedback_forces[0] = global_haptic_forces[0];
	feedback_forces[1] = global_haptic_forces[1];
	feedback_forces[2] = global_haptic_forces[2];

	hdSetDoublev(HD_CURRENT_FORCE, haptic_forces);

	// End frame
	hdEndFrame(hdGetCurrentDevice());
	return HD_CALLBACK_CONTINUE;
}

// -- Haptic get position --
// //////////////////////////////////////////////////////////////
void OpenHaptic::getPosition(double pos[6])
{
	Matrix3d R_inv;
	R_inv <<
		-1, 0, 0,
		0, -1, 0,
		0, 0, 1;

	MatrixXd RA = get_matrix(haptic_transform);
	Vector3d Pos(-haptic_pos[2], -haptic_pos[0], haptic_pos[1]);
	MatrixXd R_XYZ = getEulerMatrix(Vector3d(90, 0, 90));
	MatrixXd RB = RA * R_XYZ;

	if (inverse_frame) {
		RB = RB * R_inv;
		Pos = R_inv * Pos;
	}
	Vector3d tetha = getRotationAngle(RB);

	// Convert from euler to Rotation matrix:
	tetha = euler2rot(tetha);

	pos[0] = Pos(0);
	pos[1] = Pos(1);
	pos[2] = Pos(2);
	pos[3] = tetha(0);
	pos[4] = tetha(1);
	pos[5] = tetha(2);
}

// -- Haptic Get Button State --
// /////////////////////////////////////////////////////////////
int OpenHaptic::getButtonState() {
	int btn = haptic_button;
	return btn;
}

// -- Haptic get velocity --
// /////////////////////////////////////////////////////////////
void OpenHaptic::getVelocity(double vel[6]) {
	vel[0] = -haptic_velocity[2];
	vel[1] = -haptic_velocity[0];
	vel[2] = haptic_velocity[1];
	vel[3] = angular_velocity[2];
	vel[4] = angular_velocity[0];
	vel[5] = angular_velocity[1];

	//printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f \n", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
}

// -- Haptic set force feedback --
// /////////////////////////////////////////////////////////////
void OpenHaptic::setForces(float real_forces[3]) {
	haptic_forces[2] = -real_forces[0];
	haptic_forces[0] = -real_forces[1];
	haptic_forces[1] = real_forces[2];
}


// -- Haptic Initialization --
// /////////////////////////////////////////////////////////////
int OpenHaptic::init()
{
	HDErrorInfo error;
	hHD = hdInitDevice(HD_DEFAULT_DEVICE);

	// Initialize the default haptic device
	if (HD_DEVICE_ERROR(error = hdGetError())) {
		hduPrintError(stderr, &error, "Failed to initialize haptic device. ");
		printf("> ___HAPTIC___ : Failed to connect to haptic device \n");
		return -1;
	}
	printf("> ___HAPTIC___ :Connected to a haptic device. Device name: %s \n", hdGetString(HD_DEVICE_MODEL_TYPE));

	// Start the servo scheduler and enable forces
	hdScheduleAsynchronous(getStateCallback, 0, HD_MAX_SCHEDULER_PRIORITY);

	//hHLRC = hlCreateContext(hHD);
	//hlMakeCurrent(hHLRC);
	//hdMakeCurrentDevice(hHD);
	//gEffect = hlGenEffects(1);
	hdEnable(HD_FORCE_OUTPUT);
	hdStartScheduler();

	this->connected = true;
	return 1;
}

// Disconnect haptic connection
void OpenHaptic::close()
{
	//hlDeleteEffects(gEffect, 1);
	//hlMakeCurrent(NULL);
	//hlDeleteContext(hHLRC);
	hdStopScheduler();
	hdDisableDevice(hHD);
	this->connected = false;
}















