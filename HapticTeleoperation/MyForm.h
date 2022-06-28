#pragma once

namespace HapticTeleoperation {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void button_haptic_connect();
		void button_robot_connect();
		void on_init();
		void on_timer();
		void updateUI();
		void btn_robot_test_connection();
		void btn_start_teleoperation();
		void btn_stop_teleoperation();
		void btn_move_to_home();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:


	private: System::Windows::Forms::Button^ btn_haptic_connect;

	private: System::Windows::Forms::TabPage^ tabPage2;
	private: System::Windows::Forms::TabControl^ tabControl1;
	private: System::Windows::Forms::TabPage^ tabPage1;




	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::RadioButton^ rb_haptic;

	private: System::Windows::Forms::RadioButton^ rb_robot;

	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::TextBox^ tb_pos_r;

	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ tb_pos_p;

	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::TextBox^ tb_pos_w;

	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ tb_pos_z;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ tb_pos_y;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ tb_pos_x;
	private: System::Windows::Forms::Button^ btnTeleoperationStart;

	private: System::Windows::Forms::Button^ btn_robot;
	private: System::Windows::Forms::CheckBox^ checkBox_streaming;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Button^ btnTeleoperationStop;


	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Panel^ led_robot;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Panel^ led_haptic;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::ComponentModel::IContainer^ components;








	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->btn_haptic_connect = (gcnew System::Windows::Forms::Button());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->led_robot = (gcnew System::Windows::Forms::Panel());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->led_haptic = (gcnew System::Windows::Forms::Panel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_streaming = (gcnew System::Windows::Forms::CheckBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->tb_pos_r = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->tb_pos_p = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->tb_pos_w = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->tb_pos_z = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tb_pos_y = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tb_pos_x = (gcnew System::Windows::Forms::TextBox());
			this->rb_robot = (gcnew System::Windows::Forms::RadioButton());
			this->rb_haptic = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnTeleoperationStart = (gcnew System::Windows::Forms::Button());
			this->btn_robot = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->btnTeleoperationStop = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// btn_haptic_connect
			// 
			this->btn_haptic_connect->Location = System::Drawing::Point(170, 274);
			this->btn_haptic_connect->Name = L"btn_haptic_connect";
			this->btn_haptic_connect->Size = System::Drawing::Size(141, 30);
			this->btn_haptic_connect->TabIndex = 2;
			this->btn_haptic_connect->Text = L"Haptic Connect";
			this->btn_haptic_connect->UseVisualStyleBackColor = true;
			this->btn_haptic_connect->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Margin = System::Windows::Forms::Padding(2);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(2);
			this->tabPage2->Size = System::Drawing::Size(312, 233);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Properties";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(9, 10);
			this->tabControl1->Margin = System::Windows::Forms::Padding(2);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(320, 259);
			this->tabControl1->SizeMode = System::Windows::Forms::TabSizeMode::FillToRight;
			this->tabControl1->TabIndex = 3;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->label9);
			this->tabPage1->Controls->Add(this->led_robot);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->led_haptic);
			this->tabPage1->Controls->Add(this->groupBox1);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Margin = System::Windows::Forms::Padding(2);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(2);
			this->tabPage1->Size = System::Drawing::Size(312, 233);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Status";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(7, 15);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(101, 13);
			this->label9->TabIndex = 10;
			this->label9->Text = L"Device Connection:";
			// 
			// led_robot
			// 
			this->led_robot->BackColor = System::Drawing::Color::Red;
			this->led_robot->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->led_robot->Location = System::Drawing::Point(195, 15);
			this->led_robot->Name = L"led_robot";
			this->led_robot->Size = System::Drawing::Size(15, 15);
			this->led_robot->TabIndex = 9;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(216, 15);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(36, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Robot";
			// 
			// led_haptic
			// 
			this->led_haptic->BackColor = System::Drawing::Color::Red;
			this->led_haptic->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->led_haptic->Location = System::Drawing::Point(124, 15);
			this->led_haptic->Name = L"led_haptic";
			this->led_haptic->Size = System::Drawing::Size(15, 15);
			this->led_haptic->TabIndex = 7;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->checkBox_streaming);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->tb_pos_r);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->tb_pos_p);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->tb_pos_w);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->tb_pos_z);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->tb_pos_y);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->tb_pos_x);
			this->groupBox1->Controls->Add(this->rb_robot);
			this->groupBox1->Controls->Add(this->rb_haptic);
			this->groupBox1->Location = System::Drawing::Point(3, 51);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(302, 177);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Position";
			// 
			// checkBox_streaming
			// 
			this->checkBox_streaming->AutoSize = true;
			this->checkBox_streaming->Location = System::Drawing::Point(191, 30);
			this->checkBox_streaming->Name = L"checkBox_streaming";
			this->checkBox_streaming->Size = System::Drawing::Size(73, 17);
			this->checkBox_streaming->TabIndex = 18;
			this->checkBox_streaming->Text = L"Streaming";
			this->checkBox_streaming->UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(163, 122);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(18, 13);
			this->label6->TabIndex = 17;
			this->label6->Text = L"R:";
			// 
			// tb_pos_r
			// 
			this->tb_pos_r->Location = System::Drawing::Point(186, 119);
			this->tb_pos_r->Name = L"tb_pos_r";
			this->tb_pos_r->Size = System::Drawing::Size(78, 20);
			this->tb_pos_r->TabIndex = 16;
			this->tb_pos_r->Text = L"0.000";
			this->tb_pos_r->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(163, 96);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(17, 13);
			this->label7->TabIndex = 15;
			this->label7->Text = L"P:";
			// 
			// tb_pos_p
			// 
			this->tb_pos_p->Location = System::Drawing::Point(186, 93);
			this->tb_pos_p->Name = L"tb_pos_p";
			this->tb_pos_p->Size = System::Drawing::Size(78, 20);
			this->tb_pos_p->TabIndex = 14;
			this->tb_pos_p->Text = L"0.000";
			this->tb_pos_p->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(163, 70);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(21, 13);
			this->label8->TabIndex = 13;
			this->label8->Text = L"W:";
			// 
			// tb_pos_w
			// 
			this->tb_pos_w->Location = System::Drawing::Point(186, 67);
			this->tb_pos_w->Name = L"tb_pos_w";
			this->tb_pos_w->Size = System::Drawing::Size(78, 20);
			this->tb_pos_w->TabIndex = 12;
			this->tb_pos_w->Text = L"0.000";
			this->tb_pos_w->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(22, 122);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(17, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Z:";
			// 
			// tb_pos_z
			// 
			this->tb_pos_z->Location = System::Drawing::Point(45, 119);
			this->tb_pos_z->Name = L"tb_pos_z";
			this->tb_pos_z->Size = System::Drawing::Size(78, 20);
			this->tb_pos_z->TabIndex = 10;
			this->tb_pos_z->Text = L"0.000";
			this->tb_pos_z->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(22, 96);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(17, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Y:";
			// 
			// tb_pos_y
			// 
			this->tb_pos_y->Location = System::Drawing::Point(45, 93);
			this->tb_pos_y->Name = L"tb_pos_y";
			this->tb_pos_y->Size = System::Drawing::Size(78, 20);
			this->tb_pos_y->TabIndex = 8;
			this->tb_pos_y->Text = L"0.000";
			this->tb_pos_y->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(22, 70);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(17, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"X:";
			// 
			// tb_pos_x
			// 
			this->tb_pos_x->Location = System::Drawing::Point(45, 67);
			this->tb_pos_x->Name = L"tb_pos_x";
			this->tb_pos_x->Size = System::Drawing::Size(78, 20);
			this->tb_pos_x->TabIndex = 6;
			this->tb_pos_x->Text = L"0.000";
			this->tb_pos_x->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// rb_robot
			// 
			this->rb_robot->AutoSize = true;
			this->rb_robot->Location = System::Drawing::Point(16, 29);
			this->rb_robot->Name = L"rb_robot";
			this->rb_robot->Size = System::Drawing::Size(54, 17);
			this->rb_robot->TabIndex = 4;
			this->rb_robot->TabStop = true;
			this->rb_robot->Text = L"Robot";
			this->rb_robot->UseVisualStyleBackColor = true;
			// 
			// rb_haptic
			// 
			this->rb_haptic->AutoSize = true;
			this->rb_haptic->Location = System::Drawing::Point(82, 29);
			this->rb_haptic->Name = L"rb_haptic";
			this->rb_haptic->Size = System::Drawing::Size(56, 17);
			this->rb_haptic->TabIndex = 5;
			this->rb_haptic->TabStop = true;
			this->rb_haptic->Text = L"Haptic";
			this->rb_haptic->UseVisualStyleBackColor = true;
			this->rb_haptic->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton2_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(145, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Haptic";
			// 
			// btnTeleoperationStart
			// 
			this->btnTeleoperationStart->Location = System::Drawing::Point(171, 346);
			this->btnTeleoperationStart->Name = L"btnTeleoperationStart";
			this->btnTeleoperationStart->Size = System::Drawing::Size(140, 30);
			this->btnTeleoperationStart->TabIndex = 5;
			this->btnTeleoperationStart->Text = L"Start Teleoperation";
			this->btnTeleoperationStart->UseVisualStyleBackColor = true;
			this->btnTeleoperationStart->Click += gcnew System::EventHandler(this, &MyForm::button2_Click_1);
			// 
			// btn_robot
			// 
			this->btn_robot->Location = System::Drawing::Point(23, 274);
			this->btn_robot->Name = L"btn_robot";
			this->btn_robot->Size = System::Drawing::Size(141, 30);
			this->btn_robot->TabIndex = 6;
			this->btn_robot->Text = L"Yaskawa Connect";
			this->btn_robot->UseVisualStyleBackColor = true;
			this->btn_robot->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_1);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// btnTeleoperationStop
			// 
			this->btnTeleoperationStop->Enabled = false;
			this->btnTeleoperationStop->Location = System::Drawing::Point(24, 346);
			this->btnTeleoperationStop->Name = L"btnTeleoperationStop";
			this->btnTeleoperationStop->Size = System::Drawing::Size(141, 30);
			this->btnTeleoperationStop->TabIndex = 7;
			this->btnTeleoperationStop->Text = L"Stop Teleoperation";
			this->btnTeleoperationStop->UseVisualStyleBackColor = true;
			this->btnTeleoperationStop->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_2);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(171, 310);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(140, 30);
			this->button1->TabIndex = 8;
			this->button1->Text = L"Home Position";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_3);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(23, 310);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(140, 30);
			this->button2->TabIndex = 9;
			this->button2->Text = L"Servo ON/OFF";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(336, 385);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->btnTeleoperationStop);
			this->Controls->Add(this->btn_robot);
			this->Controls->Add(this->btnTeleoperationStart);
			this->Controls->Add(this->btn_haptic_connect);
			this->Controls->Add(this->tabControl1);
			this->Name = L"MyForm";
			this->Text = L"Haptic Teleoperation";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		on_init();
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		button_haptic_connect();
	}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void radioButton2_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
	button_robot_connect();
}
private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
	on_timer();
}
private: System::Void button1_Click_2(System::Object^ sender, System::EventArgs^ e) {
	//btn_robot_test_connection();
	btn_stop_teleoperation();
}
private: System::Void button2_Click_1(System::Object^ sender, System::EventArgs^ e) {
	btn_start_teleoperation();
}
private: System::Void button1_Click_3(System::Object^ sender, System::EventArgs^ e) {
	btn_move_to_home();
}
};
}
