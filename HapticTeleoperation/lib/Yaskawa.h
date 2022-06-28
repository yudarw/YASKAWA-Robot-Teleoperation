#pragma once
/* ////////////////////////////////////////////////////////////////////
	File	: Yaskawa.h
	By		: Yuda Risma Wahyudi
	Modified: 2021-10-31
////////////////////////////////////////////////////////////////////*/

#include <vector>
#include <string>
#include <iostream>
#include <winsock.h>

#define BASE_COORD	0
#define ROBOT_COORD 1

// Robot status variable:
#define m_Step		0
#define m_Cycle		1
#define m_Auto		2
#define m_Run		3
#define m_SafeOp	4
#define m_Teach		5
#define m_Play		6
#define m_Remote	7
#define m_HoldTp	9
#define m_HoldExt	10
#define m_HoldCom	11
#define m_Alarm		12
#define m_Error		13
#define m_Svon		14


// Movement:
#define var_pos		0
#define var_speed	1
#define var_pl		2
#define var_trig	3

#define I0000		0
#define I0001       1
#define I0002		2


typedef struct {
	int			COORD;			// 0:Base coordinate, 1:Robot coordinate, 2:User coordinate(1-65) 
	double		X, Y, Z, W, P, R;	// Position data

	int			TYPE;			// Robot config
	int			TOOL;			// Tool number (0-63)
}dataPos;

extern char* str2char(std::string str);


class Yaskawa
{
public:
	Yaskawa();
	// Telnet connection:
	int		tcp_connect(std::string ip_address);
	void	disconnect();
	void	send_command(char* pSend);		
	void	send_command(char* pSend, char * pRecv);
	void	send_command2(char* pSend, char* pRecv);
	void	recv_command(char *pRecv);

	void	reset_alarm();
	// Get robot data:
	dataPos read_cartesianPos(int coordinate_system);
	dataPos read_jointPos();
	void	read_state();
	void	waitMoving(dataPos pos);
	void	read_IO();
	void	writeIO();
	void	set_posReg(int num, dataPos pos);
	void	set_varByte(int num, int val);
	void	set_varInt(int num, int val);
	void	set_varDouble(int num, double val);
	void	set_varString(int num, char * str);
	void	job_start();
	void	job_start_2(std::string job_name);
	int		get_varByte(int num);
	int		get_varInt(int num);

	dataPos	get_homePosition();
	
	// Control command:
	//void	move(int type, dataPos pos, int speed);
	void	move(int type, dataPos pos, double speed, int pl);
	void	moveL(dataPos pos, double speed);	// Move in linear motion. Speed -> 0.1 - 1000 mm/s
	void	moveL(dataPos pos, double speed, bool wait);	// Move in linear motion. Speed -> 0.1 - 1000 mm/s
	void	moveL(double pos[9], double speed);
	void	moveJ(dataPos pos, double speed);	// Move in joint motion. Speed -> 0.01 - 100%
	void	moveJ(double pos[9], double speed);
	void	IMOVE(double step_val[6], double speed);
	void	SVON();								// Servo power ON
	void	SVOFF();							// Servo power OFF

	// Tools:
	dataPos setDataPos(double pos[9]);
	void	printPos(dataPos pos);
	void	pos2array(dataPos pos1, double pos2[6]);

public:
	SOCKET	_sock;
	bool	servoIsActive;
	bool	connected;
	bool	isMoving;
	bool	run;
	bool	isStreaming;
	char	pRecv[512];
	bool	alarmIsActive;
	dataPos	global_pos;
	int		status[16];
};	