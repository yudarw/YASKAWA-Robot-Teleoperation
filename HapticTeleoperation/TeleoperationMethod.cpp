#include "TeleoperationMethod.h"

#define time_sampling_interval 100

bool haptic_jog_mode = true;

void thread_haptic_teleoperation(void*) {
	
	double dT = time_sampling_interval;
	double Ts = dT / 1000;

	Vector3d Xr, Xh, Xs, Xm, Xs0, Xh0, Xcs;
	Vector3d Xr, Xh, Xs, Xm, Xs0, Xh0, Xcs;
	Vector3d Xr_rot, Xh_rot, Xcs_rot;
	Vector3d Xm_vel, Xs_vel, Xm_prev, prev_Xr, Xr_prev, prev_Xh;
	Vector3d Fcs, Fcm, Fe, Fext, prev_Fe;
	Vector3d master_position_control;
	Vector3d master_force_control;
	Vector3d master_sum_integral;
	Vector3d slave_position_control;
	Vector3d slave_force_control;
	Vector3d slave_force_integral;

	double	haptic_pos[6];
	float	robot_pos[6];

	// Set initial value
	prev_Xh.setZero();
	prev_Xr.setZero();
	Xm_prev.setZero();
	Xr_prev.setZero();

	while (haptic_jog_mode) {
		
		// Read the robot and haptic 


	}
}