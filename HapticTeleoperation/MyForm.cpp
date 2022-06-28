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
			label_haptic->Text = "CONNECTED";
			label_haptic->ForeColor = Color::Green;
			btn_haptic_connect->Text = "Haptic Disconnect";
		}
		else {
			label_haptic->Text = "Connection Failed";
			label_haptic->ForeColor = Color::Red;
		}
	}

	else {
		haptic.close();
		btn_haptic_connect->Text = "Haptic Connect";
		label_haptic->Text = "DISCONNECTED";
		label_haptic->ForeColor = Color::Black;
	}

}

// Robot Connection
void MyForm::button_robot_connect() {
	if (!robot.isConnected) {
		if (robot.tcp_connect("192.168.171.222") != 0) {
			label_robot->Text = "CONNECTED";
			label_robot->ForeColor = Color::Green;
			btn_robot->Text = "Yaskawa Disconnected";
		}
		else {
			label_haptic->Text = "Connection Failed";
			label_haptic->ForeColor = Color::Red;
		}
	}

	else {
		robot.disconnect();
		btn_robot->Text = "Yaskawa Disconnected";
		label_robot->Text = "DISCONNECTED";
		label_robot->ForeColor = Color::Black;
	}
}