/* ////////////////////////////////////////////////////////////////////
File	: Yaskawa.cpp
By		: Yuda Risma Wahyudi
Modified: 2021-10-31
////////////////////////////////////////////////////////////////////*/

#include "Yaskawa.h"
#include <winsock.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream> 

using namespace std;

char* str2char(string str) {
	char* ch = 0;
	ch = new char[str.length() + 1];
	strcpy_s(ch, str.length() + 1, str.c_str());
	return ch;
}

Yaskawa::Yaskawa() 
{
	

}


// Robot Connect:
// /////////////////////////////////////////////////////////////////
int Yaskawa::tcp_connect(string ip_address)
{
	IN_ADDR IpAddress;
	struct hostent* Host;
	int  err;
	WSADATA WsaData;
	SOCKET sock;
	SOCKADDR_IN RmAddress;
	int SndSize = 256;
	int RcvSize = 256;
	err = WSAStartup(0x0101, &WsaData);
	IpAddress.s_addr = htonl(INADDR_LOOPBACK);
	Host = gethostbyname(ip_address.c_str());
	if (Host == NULL || isdigit(*ip_address.c_str())){
		IpAddress.s_addr = inet_addr(ip_address.c_str());
		if (IpAddress.s_addr == -1){
			printf("> Unknown remote host: %s\n", ip_address);
		}
	}
	else{
		CopyMemory((char *)&IpAddress, Host->h_addr, Host->h_length);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET){
		printf("DoEcho: socket failed: %ld\n", GetLastError ());
		return 0;
	}

	err = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&RcvSize,
		sizeof(RcvSize));
	if (err == SOCKET_ERROR){
		printf("DoEcho: setsockopt( SO_RCVBUF ) failed: %ld\n", GetLastError ());
		closesocket(sock);
		return 0;
	}

	err = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&SndSize,
		sizeof(SndSize));
	if (err == SOCKET_ERROR){
		printf ("DoEcho: setsockopt( SO_SNDBUF ) failed: %ld\n", GetLastError ());
		closesocket(sock);
		return 0;
	}

	ZeroMemory(&RmAddress, sizeof(RmAddress));
	RmAddress.sin_family = AF_INET;
	RmAddress.sin_port = htons(80);
	RmAddress.sin_addr = IpAddress;

	err = connect(sock, (PSOCKADDR)&RmAddress, sizeof(RmAddress));

	if (err == SOCKET_ERROR) {
		printf ("DoEcho: connect failed: %ld\n", GetLastError ());
		closesocket(sock);
		this->connected = false;
		return 0;
	}

	if (sock == 0) {
		printf("> Connection failed! \n");
		return 0;
	}

	printf("\n>>> Yaskawa connection success... \n");
	this->_sock = sock;
	this->connected = true;
	return sock;
}

void Yaskawa::disconnect() 
{
	closesocket(_sock);
	WSACleanup();
	connected = false;
}

void Yaskawa::send_command(char* pSend, char* pRecv) {
	int len = strlen(pSend);
	int check = send(this->_sock, (char*)pSend, len, 0);
	check = recv(this->_sock, (char*)pRecv, 256, 0);
	pRecv[check] = 0;
}
void Yaskawa::send_command2(char* pSend, char* pRecv) {
	int len = strlen(pSend);
	int check = send(this->_sock, (char*)pSend, len, 0);
	check = recv(this->_sock, (char*)pRecv, 256, 0);
	check = recv(this->_sock, (char*)pRecv, 256, 0);
	pRecv[check] = 0;
}

void Yaskawa::send_command(char* pSend) {
	int len = strlen(pSend);
	int check = send(_sock, (char*)pSend, len, 0);	
}

void Yaskawa::recv_command(char *pRecv) {
	int check = recv(this->_sock, (char*)pRecv, 256, 0);
	pRecv[check] = 0;
}


void Yaskawa::printPos(dataPos pos)
{
	printf("> Robot Pos: %.3f    %.3f    %.3f     %.3f     %.3f    %.3f \n",
		pos.X, pos.Y, pos.Z, pos.W, pos.P, pos.R );
}

// Set float to datapos:
dataPos Yaskawa::setDataPos(double pos[9])
{
	dataPos d_pos;
	d_pos.X = pos[0];
	d_pos.Y = pos[1];
	d_pos.Z = pos[2];
	d_pos.W = pos[3];
	d_pos.P = pos[4];
	d_pos.R = pos[5];
	d_pos.COORD = pos[6];
	d_pos.TYPE = pos[7];
	d_pos.TOOL = pos[8];
	return d_pos;
}


void Yaskawa::read_state()
{
	if (!connected) { return;  }

	int data1 = 0, data2 = 0;	
	int next;

	send_command2("HOSTCTRL_REQUEST RSTATS 0\r",pRecv);
	string str = string(pRecv);

	if (str == "") return;

	stringstream ss, ss2;
	ss << str.substr(0, str.find(","));
	ss >> data1;
	ss2 << str.substr(str.find(",") + 1);
	ss2 >> data2;
	
	next = 0;
	// Reset "status" array
	for (int i = 0; i < 16; i++) status[i] = 0;
	while (data1 != 0) {
		status[next] = (data1 % 2 == 0 ? 0 : 1);
		data1 /= 2;
		next++;
	}
	next = 8;
	while (data2 != 0) {
		status[next] = (data2 % 2 == 0 ? 0 : 1);
		data2 /= 2;
		next++;
	}

	//for (int i = 0; i < 16; i++) cout << status[i];
	//cout << endl;
}

// Turns servo power supply OFF:
void Yaskawa::SVOFF()
{
	if (this->connected == false) {
		printf("> Servo OFF failed! Robot is not connected! \n");
		return;
	}

	read_state();
	if (status[m_Remote] == 0) {
		printf("> Error: Select the mode to remote mode. \n");
		return;
	}

	char pRecv[512];
	send_command("HOSTCTRL_REQUEST SVON 2\r\n", pRecv);
	send_command("0\r\n", pRecv);
	read_state(); // update status of the servo
	printf("> SERVO OFF \n");
	this->servoIsActive = false;
}

// Turns servo power supply ON:
void Yaskawa::SVON()
{
	if (this->connected == false) {
		printf("> Servo ON failed! Robot is not connected! \n");
		return;
	}
	read_state();
	if (status[m_Remote] == 0) {
		printf("> Error: Select the mode to remote mode. \n");
		return;
	}

	char pRecv[512];
	send_command("HOSTCTRL_REQUEST SVON 2\r\n", pRecv);
	send_command("1\r\n", pRecv);
	read_state();	//Update status of the servo;
	printf("> SERVO ON \n");
	this->servoIsActive = true;
}

// Reads the current position in a specified coordinate system:
dataPos Yaskawa::read_cartesianPos(int coordinate_system)
{
	char temp[100];
	float value[8];
	int current = 0;
	int next = 0;
	char pRecv[512];
	dataPos pos;
	
	if (!connected) {
		return pos;
	}

	sprintf(temp, "%i,0\r",coordinate_system);
	send_command("HOSTCTRL_REQUEST RPOSC 4\r\n",pRecv);
	send_command(temp, pRecv);
	string str(pRecv);
	if (str == "") {
		printf("> Read position error. String = nullptr \n");
		return pos;
	}

	char *token = strtok(const_cast<char*>(str.c_str()), ",");
	while (token != nullptr) {
		stringstream ss;
		ss << token;
		ss >> value[next];
		token = strtok(nullptr, ",");
		next++;
	}

	pos.X = value[0];
	pos.Y = value[1];
	pos.Z = value[2];
	pos.W = value[3];
	pos.P = value[4];
	pos.R = value[5];
	pos.TYPE = (int)value[6];
	pos.TOOL = (int)value[7];
	pos.COORD = coordinate_system;

	this->global_pos = pos;
	//cout << "> Get position: " << str << endl;

	//read_state();

	return pos;
}

// Reads the current position in joint coordinate system:
dataPos Yaskawa::read_jointPos()
{
	dataPos pos;

	return pos;
}


// Moves a manipulator to a specified coordinate position in linear motion
// ///////////////////////////////////////////////////////////////////////
void Yaskawa::moveL(dataPos pos, double speed)
{
	moveL(pos, speed, false);
}

void Yaskawa::moveL(double pos[9], double speed)
{
	dataPos d_pos = setDataPos(pos);
	moveL(d_pos, speed, false);
}

void Yaskawa::moveL(dataPos pos, double speed, bool wait) {
	char	temp[512];
	string  command_str;
	string	data_str;
	int		data_size = 0;
	int		MSS = 0;	// Motion Speed Selection. 0:V(speed)  1:VR(posture speed)

	pos.COORD = 0;

	if (!connected) {
		printf("> Error: Robot is not connected! ");
	}

	// Check motor status:
	if (!servoIsActive) {
		printf("> Error: Servo is OFF \n");
		return;
	}

	read_state();
	if (status[m_Remote] == 0) {
		printf("> Error: Select the mode to remote mode. \n");
		return;
	}

	if (status[m_Alarm] == 1) {
		printf("> Alarm Error!!!! \n");
		alarmIsActive = true;
		return;
	}
	else {
		alarmIsActive = false;
	}

	sprintf(temp, "%i,%f,%i,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%i,%i,%i,%i,%i,%i,%i,%i\r",
		MSS, speed, pos.COORD, pos.X, pos.Y, pos.Z, pos.W, pos.P, pos.R, pos.TYPE, pos.TOOL,
		0, 0, 0, 0, 0, 0);

	data_str = string(temp);
	data_size = data_str.length();


	sprintf(temp, "HOSTCTRL_REQUEST MOVL %i\r", data_size);
	send_command(temp, pRecv);
	send_command(str2char(data_str), pRecv);

	//printf("> Robot moving... speed = %i mm/s \n", speed);
	if(wait) waitMoving(pos);
}

// Moves a manipulator to a specified pulse position in joint motion. Motion Speed 0.01 - 100 %
// //////////////////////////////////////////////////////////////////
void Yaskawa::moveJ(dataPos pos, double speed)
{
	char	temp[512];
	string  command_str;
	string	data_str;
	int		data_size = 0;

	// Check motor status:
	if (!servoIsActive) {
		printf("> Error: Servo is OFF \n");
		return;
	}

	read_state();
	if (status[m_Remote] == 0) {
		printf("> Error: Select the mode to remote mode. \n");
		return;
	}


	//Command format: MOVJ -> Motion Speed, Coord, x, y, z, w, p, r, type, tool
	sprintf(temp, "%f,%i,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%i,%i,%i,%i,%i,%i,%i,%i\r",
		speed, pos.COORD, pos.X, pos.Y, pos.Z, pos.W, pos.P, pos.R, pos.TYPE, pos.TOOL, 0, 0, 0, 0, 0, 0 );

	data_str = string(temp);
	data_size = data_str.length();
	//command_str = "HOSTCTRL_REQUEST MOVJ " + to_string(data_size) + "\r";

	sprintf(temp, "HOSTCTRL_REQUEST MOVJ %d\r", data_size);

	send_command(temp, pRecv);
	send_command(str2char(data_str), pRecv);

	//waitMoving(pos);
}

void Yaskawa::moveJ(double pos[9], double speed)
{
	dataPos d_pos = setDataPos(pos);
	moveJ(d_pos, speed);
}


// Convert datapos to array
void Yaskawa::pos2array(dataPos pos1, double pos2[6]) {
	pos2[0] = pos1.X;
	pos2[1] = pos1.Y;
	pos2[2] = pos1.Z;
	pos2[3] = pos1.W;
	pos2[4] = pos1.P;
	pos2[5] = pos1.R;
}

// Wait the robot until finish the movement
void Yaskawa::waitMoving(dataPos target_pos)
{
	int		time = 0;
	bool	tmp = false;
	int		counter, count = 0;
	dataPos curPos;
	double	p1[6], p2[6];

	target_pos.W = target_pos.W + 180;
	target_pos.P = target_pos.P + 180;
	target_pos.R = abs(target_pos.R);

	isMoving = true;
	while (isMoving && connected) {
		read_state();
		global_pos = read_cartesianPos(0);
		curPos	 = global_pos;
		curPos.W = curPos.W + 180;
		curPos.P = curPos.P + 180;
		curPos.R = abs(curPos.R);

		pos2array(target_pos, p1);
		pos2array(curPos, p2);

		for (int i = 0; i < 6; i++) {
			if (abs(p1[i] - p2[i]) < 2) counter++;
		}
		if (counter == 6) isMoving = false;
		counter = 0;
		Sleep(10);
	}
	isMoving = false;
}



// Alarm reset
void Yaskawa::reset_alarm() {
	send_command("HOSTCTRL_REQUEST RESET 0\r",pRecv);
}

// Incremental Move
void Yaskawa::IMOVE(double step_val[6], double speed) {
	char	temp[512];
	string  strData;
	string	strCommand;
	int		strSize = 0;

	int coord = global_pos.COORD;
	int tool  = global_pos.TOOL;
	
	// Check motor status:
	if (!servoIsActive) {
		printf("> Error: Servo is OFF \n");
		return;
	}

	read_state();
	if (status[m_Remote] == 0) {
		printf("> Error: Select the mode to remote mode. \n");
		return;
	}
	sprintf(temp, "0,%f,%i,%f,%f,%f,%f,%f,%f,0,%i,0,0,0,0,0,0\r",
		speed,coord,step_val[0], step_val[1], step_val[2], step_val[3], step_val[4], step_val[5],tool);
	strData = string(temp);

	sprintf(temp, "HOSTCTRL_REQUEST IMOV %i\r", strData.length());
	strCommand = string(temp);
	
	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}

void Yaskawa::read_IO() {
	char	temp[512];
	string  strData;
	string	strCommand;

	int data1 = 10010;
	int data2 = 8;

	/*
	// Check motor status:
	if (!servoIsActive) {
		printf("> Error: Servo is OFF \n");
		return;
	}

	read_state();
	if (status[m_Remote] == 0) {
		printf("> Error: Select the mode to remote mode. \n");
		return;
	}
	*/
	// Set Data String:
	sprintf(temp, "%i,%i\r", data1, data2);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST IOREAD %i\r", strData.length());
	strCommand = string(temp);

	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);

	printf("\n> Read IO test: \n");
	printf(pRecv);
}

void Yaskawa::writeIO() {
	char	temp[512];
	string  strData;
	string	strCommand;

	int data1 = 27010;
	int data2 = 8;
	int data3 = 54;

	// Set Data String:
	//sprintf(temp, "%i,%i,%i\r", data1, data2,data3);
	sprintf(temp, "%d,%d,%d\r", data1, data2, data3);
	//sprintf(temp, "27010,24,1,2,3\r");
	strData = string(temp);
	cout << strData << " Data Length: " << strData.length() << endl;
	sprintf(temp, "HOSTCTRL_REQUEST IOWRITE 15\r", strData.length());
	strCommand = string(temp);
	cout << strCommand << endl;
	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);

	printf("> Write IO test: ");
	printf(pRecv);
	printf("\n");
}


/* /////////////////////////////////////////////
	Set variable register
	Command: LOADV
	Data-1 : Type of varibles. 0:byte, 1:int, 2:double, 3:real, 7:string
	Data-2 : Variable no
	Data-3 : Value
  ///////////////////////////////////////////// */

void Yaskawa::set_varByte(int num, int val) {
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp, "%i,%i,%i\r", 0, num, val);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST LOADV %i\r", strData.length());
	strCommand = string(temp);

	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}

void Yaskawa::set_varInt(int num, int val) {
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp, "%i,%i,%i\r", 1, num, val);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST LOADV %i\r", strData.length());
	strCommand = string(temp);

	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}

void Yaskawa::set_varDouble(int num, double val) {
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp, "%i,%i,%.3f\r", 2, num, val);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST LOADV %i\r", strData.length());
	strCommand = string(temp);
	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}
void Yaskawa::set_varString(int num, char* str) {
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp, "%i,%i,%s\r", 7, num, str);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST LOADV %i\r", strData.length());
	strCommand = string(temp);

	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}


/* ///////////////////////////////////////////////////////////////
	Get variable register
  //////////////////////////////////////////////////////////////*/
int Yaskawa::get_varByte(int num) {
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp, "%i,%i\r", 0, num);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST SAVEV %i\r", strData.length());
	strCommand = string(temp);
	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);

	int val;
	stringstream ss;
	ss << pRecv;
	ss >> val;

	return val;
}
int Yaskawa::get_varInt(int num) {
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp, "%i,%i\r", 1, num);
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST SAVEV %i\r", strData.length());
	strCommand = string(temp);
	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);

	int val;
	stringstream ss;
	ss << pRecv;
	ss >> val;

	return val;
}

/*  //////////////////////////////////////////////////////////////
	Set Position Register
	Command: LOADV
	Data-1 : type of variables
	         4: Robot axis position type variables
			 5: Base axis position type variables
			 6: Station axis position type variables
	Data-2 : Variable No.
	Data-3 : Position data type. 0: pulse, 1: Cartesian
	Data-4 : Coordinate data
	Data-5 ~ 10 : Position 
	Data-11 : type
	Data-12 : tool
   ///////////////////////////////////////////////////////////// */
void Yaskawa::set_posReg(int num, dataPos pos)
{
	char	temp[512];
	string  strData;
	string	strCommand;
	// Data:
	sprintf(temp, "%i,%i,%i,%i,%f,%f,%f,%f,%f,%f,%i,%i\r",
		4,		// Data-1 
		num,	// Data-2
		1,		// Data-3
		0,		// Data-4
		pos.X,	// Data-5
		pos.Y,	// ..
		pos.Z,  // ..
		pos.W,  // ..
		pos.P,  // ..
		pos.R,  // ..
		pos.TYPE, // ..
		pos.TOOL  // Data-12
	);
	strData = string(temp);
	// Command:
	sprintf(temp, "HOSTCTRL_REQUEST LOADV %i\r", strData.length());
	strCommand = string(temp);
	// Send:
	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}


void Yaskawa::job_start() 
{
	char	temp[256];
	string  strData;
	string	strCommand;

	sprintf(temp,"REMOTEAPI\r");		// Name of the project in pendant : DEF -> TES_IO_CONNECTION
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST START %i\r", strData.length());
	strCommand = string(temp);

	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}

void Yaskawa::job_start_2(string job_name) {
	char temp[256];
	string strData;
	string strCommand;

	sprintf(temp, "%s\r", job_name.c_str());
	strData = string(temp);
	sprintf(temp, "HOSTCTRL_REQUEST START %i\r", strData.length());
	strCommand = string(temp);

	send_command(str2char(strCommand), pRecv);
	send_command(str2char(strData), pRecv);
}

/* ////////////////////////////////////////////////////////////////
	New collaborative move
   //////////////////////////////////////////////////////////////*/

void Yaskawa::move(int type, dataPos pos, double speed, int pl) {
	// I0000 -> Trigger
	// I0001 -> Speed
	// I0002 -> PL

	set_posReg(var_pos,		pos);
	set_varInt(I0001,			speed);
	set_varInt(I0002,			pl);
	set_varInt(I0000,			type);

	while (get_varInt(0) != 0) Sleep(10);
	printf("> Moving done. \n");
}

dataPos Yaskawa::get_homePosition() {
	dataPos pos;
	pos.X = 457.417;
	pos.Y = -5.034;
	pos.Z = 293.327;
	pos.W = 178.670;
	pos.P = -0.002;
	pos.R = 0.031;
	pos.TOOL = 5;
	pos.TYPE = 1;
	return pos;
}