#include "HomogeneousTransform.h"
#include <iostream>
#include <stdio.h>

MatrixXd get4x4TransformMatrix(MatrixXd Rot, Vector3d Pos)
{
	MatrixXd M(4, 4);
	M(3, 3) = 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			M(i, j) = Rot(i, j);
		M(i, 3) = Pos[i];
		M(3, i) = 0;
	}
	return M;
}
MatrixXd getRotationMatrix(Vector3d rot)
{
	MatrixXd M(3, 3);
	float gamma = rot[0] * 3.141592 / 180;	// rx
	float beta  = rot[1] * 3.141592 / 180;	// ry
	float alpha = rot[2] * 3.141592 / 180;	// rz
	M(0, 0) = cos(alpha) * cos(beta);
	M(1, 0) = sin(alpha) * cos(beta);
	M(2, 0) = -sin(beta);
	M(0, 1) = cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
	M(1, 1) = sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
	M(2, 1) = cos(beta) * sin(gamma);
	M(0, 2) = cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);
	M(1, 2) = sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);
	M(2, 2) = cos(beta) * cos(gamma);
	return M;
}
MatrixXd getEulerMatrix(Vector3d rot)
{
	MatrixXd M(3, 3);
	float gamma = rot[0] * 3.141592 / 180;	// rx
	float beta = rot[1] * 3.141592 / 180;	// ry
	float alpha = rot[2] * 3.141592 / 180;	// rz
	M(0, 0) = cos(beta) * cos(gamma);
	M(0, 1) = -cos(beta) * sin(gamma);
	M(0, 2) = sin(beta);
	M(1, 0) = (cos(alpha) * sin(gamma)) + (cos(gamma) * sin(alpha) * sin(beta));
	M(1, 1) = (cos(alpha) * cos(gamma)) - (sin(alpha) * sin(beta) * sin(gamma));
	M(1, 2) = -cos(beta) * sin(alpha);
	M(2, 0) = (sin(alpha) * sin(gamma)) - (cos(alpha)*cos(gamma)*sin(beta));
	M(2, 1) = (cos(gamma) * sin(alpha)) + (cos(alpha)*sin(beta)*sin(gamma));
	M(2, 2) = cos(alpha) * cos(beta);
	return M;
}
Vector3d getRotationAngle(MatrixXd rotation_matrix)
{
	Vector3d rot_angle;
	double alpha, beta, gamma;
	double r31 = rotation_matrix(2, 0);
	double r11 = rotation_matrix(0, 0);
	double r12 = rotation_matrix(0, 1);
	double r22 = rotation_matrix(1, 1);
	double r21 = rotation_matrix(1, 0);
	double r32 = rotation_matrix(2, 1);
	double r33 = rotation_matrix(2, 2);

	beta = atan2(-r31, sqrt((r11 * r11) + (r21 * r21)));
	alpha = atan2(r21, r11);
	gamma = atan2(r32, r33);

	beta = beta * (180 / 3.14);

	if ((int)beta == 90) {
		alpha = 0;
		gamma = atan2(r12, r22);
	}
	else if ((int)beta == -90) {
		alpha = 0;
		beta = -atan2(r12, r22);
	}

	alpha = alpha * (180 / 3.14);
	gamma = gamma * (180 / 3.14);

	rot_angle[0] = gamma;
	rot_angle[1] = beta;
	rot_angle[2] = alpha;

	return rot_angle;
}
Vector3d getEulerAngle(MatrixXd rotation_matrix)
{
	Vector3d rot_angle;
	double alpha, beta, gamma;
	double r11 = rotation_matrix(0, 0);
	double r12 = rotation_matrix(0, 1);
	double r13 = rotation_matrix(0, 2);
	double r21 = rotation_matrix(1, 0);
	double r22 = rotation_matrix(1, 1);
	double r23 = rotation_matrix(1, 2);
	double r31 = rotation_matrix(2, 0);
	double r32 = rotation_matrix(2, 1);
	double r33 = rotation_matrix(2, 2);

	if (r13 == 1) {
		beta = 90;
		alpha = 0;
		gamma = atan2(r21, r22);
	}
	else {
		beta = asin(r13);
		alpha = atan2(-r23, r33);
		gamma = atan2(-r12, r11);
	}

	beta = beta * (180 / 3.14);
	alpha = alpha * (180 / 3.14);
	gamma = gamma * (180 / 3.14);

	rot_angle[0] = gamma;
	rot_angle[1] = beta;
	rot_angle[2] = alpha;

	return rot_angle;
}

Vector3d rot2euler(Vector3d orientation){
	MatrixXd R_XYZ = getRotationMatrix(orientation);
	Vector3d O_EULER = getEulerAngle(R_XYZ);
	Vector3d temp(O_EULER(2), O_EULER(1), O_EULER(0));
	return temp;
}

Vector3d euler2rot(Vector3d orientation){
	MatrixXd R_EULER = getEulerMatrix(orientation);
	Vector3d O_EULER = getRotationAngle(R_EULER);
	Vector3d temp(O_EULER(2), O_EULER(1), O_EULER(0));
	return temp;
}

MatrixXd extract_rot_matrix(MatrixXd T) {
	MatrixXd new_matrix(3, 3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			new_matrix(i, j) = T(i, j);
		}
	}
	return new_matrix;
}
Vector3d extract_pos_vector(MatrixXd T) {
	Vector3d pos(T(0, 3), T(1, 3), T(2, 3));
	return pos;
}
MatrixXd matrix_mul(MatrixXd Ta, MatrixXd Tb) {
	MatrixXd Ra = extract_rot_matrix(Ta);
	Vector3d Pa = extract_pos_vector(Ta);
	MatrixXd Rb = extract_rot_matrix(Tb);
	Vector3d Pb = extract_pos_vector(Tb);

	MatrixXd new_rot_matrix = Ra * Rb;
	Vector3d new_pos = (Ra * Pb) + Pa;

	MatrixXd new_matrix = get4x4TransformMatrix(new_rot_matrix, new_pos);
	//cout << endl << new_rot_matrix << endl << new_pos << endl;
	return new_matrix;
}