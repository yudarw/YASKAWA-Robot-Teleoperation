#include "MyForm.h"
#include "lib/OpenHaptic.h";
#include "lib/HomogeneousTransform.h"
#include "lib/Yaskawa.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace HapticTeleoperation;

OpenHaptic	haptic;
Yaskawa		robot;



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
		if (robot.isConnected) {
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
	if (!robot.isConnected) {
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


// Robot test connection:
void MyForm::btn_robot_test_connection() {
	dataPos pos;

	if (robot.isConnected) {
		pos = robot.read_cartesianPos(0);
		printf("Robot Position: %.3f, %.3f, %.3f, %.3f, %.3f, %.3f \n", pos.X, pos.Y, pos.Z, pos.W, pos.P, pos.R);
	}
}