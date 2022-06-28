#include "MyForm.h"
#include "lib/OpenHaptic.h";
#include "lib/HomogeneousTransform.h"
#include "lib/Yaskawa.h"
#include "TeleoperationMethod.h"
#include <process.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace HapticTeleoperation;

OpenHaptic	haptic;
Yaskawa		robot;

double standby_pos[6] = { 610, 153, 35, 90, 85 };
double home_pos[6] = { 377, -8, 182, 179, 0, 0 };

// Program initialization
void MyForm::on_init() {
	rb_haptic->Checked = true;

	timer1->Interval = 50;
	timer1->Enabled = true;
}


// On timer tick
void MyForm::on_timer() {

	updateUI();
}


void MyForm::updateUI() {
	double pos[6];
	dataPos rPos;

	if (rb_haptic->Checked) {
		if (haptic.connected) {
			haptic.getPosition(pos);
			tb_pos_x->Text = pos[0].ToString("N3");
			tb_pos_y->Text = pos[1].ToString("N3");
			tb_pos_z->Text = pos[2].ToString("N3");
			tb_pos_w->Text = pos[3].ToString("N3");
			tb_pos_p->Text = pos[4].ToString("N3");
			tb_pos_r->Text = pos[5].ToString("N3");
		}	
	}

	else if (rb_robot->Checked) {
		if (robot.connected && !haptic_jog_mode) {
			rPos = robot.read_cartesianPos(0);
			tb_pos_x->Text = rPos.X.ToString("N3");
			tb_pos_y->Text = rPos.Y.ToString("N3");
			tb_pos_z->Text = rPos.Z.ToString("N3");
			tb_pos_w->Text = rPos.W.ToString("N3");
			tb_pos_p->Text = rPos.P.ToString("N3");
			tb_pos_r->Text = rPos.R.ToString("N3");
		}
	}
}

// Haptic Connection
void MyForm::button_haptic_connect() {
	if (!haptic.connected) {
		if (haptic.init() != -1) {
			led_haptic->BackColor = Color::Lime;
			btn_haptic_connect->Text = "Haptic Disconnect";
		}
		else {
			led_haptic->BackColor = Color::Red;
		}
	}

	else {
		haptic.close();
		btn_haptic_connect->Text = "Haptic Connect";
		led_haptic->BackColor = Color::Red;
	}

}

// Robot Connection
void MyForm::button_robot_connect() {
	if (!robot.connected) {
		if (robot.tcp_connect("192.168.171.222") != 0) {
			robot.send_command("CONNECT Robot_access Keep-Alive:50000\r", robot.pRecv);
			led_robot->BackColor = Color::Lime;
			btn_robot->Text = "Yaskawa Disconnect";
		}
		else {
			led_haptic->BackColor = Color::Red;
		}
	}

	else {
		robot.disconnect();
		btn_robot->Text = "Yaskawa Connect";
		led_robot->BackColor = Color::Red;
	}
}

// Robot test connection
void MyForm::btn_robot_test_connection() {
	dataPos pos;
	if (robot.connected) {
		pos = robot.read_cartesianPos(0);
		printf("Robot Position: %.3f, %.3f, %.3f, %.3f, %.3f, %.3f \n", pos.X, pos.Y, pos.Z, pos.W, pos.P, pos.R);
	}
}

dataPos arrayToDatapos(double pos[6]) {
	dataPos newpos;
	newpos.X = pos[0];
	newpos.Y = pos[1];
	newpos.Z = pos[2];
	newpos.W = pos[3];
	newpos.P = pos[4];
	newpos.R = pos[5];
	newpos.TOOL = 8;
	newpos.TYPE = 1;
	return newpos;
}


// Move to home position
void MyForm::btn_move_to_home() {
	if (!robot.connected) {
		printf("Robot move position error!!! Robot is not connected. \n");
		return;
	}
	if (!robot.servoIsActive) robot.SVON();
	dataPos cmd_pos = arrayToDatapos(home_pos);
	robot.moveL(cmd_pos, 90, true);
	robot.SVOFF();
}

// Start teleoperation control
void MyForm::btn_start_teleoperation() {
	
	// First make sure that the robot and haptic device are already connected
	if (!haptic.connected && !robot.connected) {
		printf("Teleoperation control failed! Robot or haptic device is not connected");
		return;
	}

	// Turn on the YASKAWA servo motor
	if (!robot.servoIsActive) {
		robot.SVON();
	}

	// Start the teleoperation thread function
	_beginthread(thread_haptic_teleoperation, 0, NULL);

	btnTeleoperationStop->Enabled = true;
	btnTeleoperationStart->Enabled = false;
}


// Stop teleoperationc control
void MyForm::btn_stop_teleoperation() {
	haptic_jog_mode = false;
	robot.SVOFF();
	btnTeleoperationStop->Enabled = false;
	btnTeleoperationStart->Enabled = true;
}