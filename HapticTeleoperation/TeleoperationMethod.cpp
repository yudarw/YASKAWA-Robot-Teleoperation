#include "TeleoperationMethod.h"

#define time_sampling_interval	100
#define robot_speed				50

bool haptic_jog_mode = true;

// Global declaration
void setRobotPosition(Vector3d pos, Vector3d rot);
void setHapticForces(Vector3d fh);


// =======================================================
//			Haptic teleoperation method
// =======================================================
void thread_haptic_teleoperation(void*) {
	
	double dT = time_sampling_interval;
	double Ts = dT / 1000;

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

	bool teleoperationIsActive	= false;
	bool orientationIsLocked	= true;

	haptic_jog_mode = true;

	while (haptic_jog_mode) {
		
		// Read the robot and haptic position
		haptic.getPosition(haptic_pos);
		dataPos pos = robot.read_cartesianPos(0);

		// Normalize the position data variable
		Xr		<< pos.X, pos.Y, pos.Z;
		Xr_rot	<< pos.W, pos.P, pos.R;
		Xh		<< haptic_pos[0], haptic_pos[1], haptic_pos[2];
		Xh_rot	<< haptic_pos[3], haptic_pos[4], haptic_pos[5];
		Xm_vel	= (Xh - prev_Xh) / Ts;		// Haptic velocity 
		Xs_vel	= (Xr - prev_Xr) / Ts;		// Robot velocity

		// Check button state
		int haptic_button = haptic.getButtonState();
		if (haptic_button == 2) {
			Sleep(300);
			if (teleoperationIsActive == false) {
				teleoperationIsActive = true;
				Xs0 = Xr;
				Xh0 = Xh;
			}
			else {
				teleoperationIsActive = false;
				setHapticForces(Vector3d(0, 0, 0));
			}
		}
		else if (haptic_button == 1) {
			Sleep(300);
			if (orientationIsLocked == false) {
				orientationIsLocked = true;
			}
			else {
				orientationIsLocked = false;
			}
		}
		
		// Start the teleoperation control
		if (teleoperationIsActive) {

			// Teleoperation parameters:
			double Km, Bm, Ks, Bs, Kg, Bg;

			Km = 0.01;
			Bm = 0.0001;
			Ks = 0.01;
			Bs = 0.0001;

			Xm = Xs0 + (Xh - Xh0);
			Xs = Xr;
			
			Fcm = ((Xs - Xm) * Km) + ((Xs_vel - Xm_vel) * Bm);
			Fcs = ((Xm - Xs) * Ks) + ((Xm_vel - Xs_vel) * Bs);

			// Slave Cartesian Controller:
			Xcs = Xs + Fcs;

			// Set the output:
			setHapticForces(Fcm);
			setRobotPosition(Xcs, Xr_rot);
		}

		// Update the (t-1) variables for next iteration
		prev_Xh = Xh;
		prev_Xr = Xr;

		Sleep(dT); // Sampling rate
	}
}


// -- Set Haptic Feedback Force -- //
void setHapticForces(Vector3d fh) {
	float f[3] = { fh[0], fh[1], fh[2] };
	haptic.setForces(f);
}

// -- Set Robot Position -- //
void setRobotPosition(Vector3d pos, Vector3d rot) {
	dataPos goalPos;
	goalPos.X = pos[0];
	goalPos.Y = pos[1];
	goalPos.Z = pos[2];
	goalPos.W = rot[0];
	goalPos.P = rot[1];
	goalPos.R = rot[2];
	goalPos.TOOL = 8;
	goalPos.TYPE = 1;
	robot.moveL(goalPos, robot_speed);
}