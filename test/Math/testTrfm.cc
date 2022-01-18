#include "t.h"
#include "constants.h"
#include "tools.h"
#include "trfm3D.h"

FILE *fh;

static Vector3 TransformPointTrfm3D_OK( const Trfm3D & TT, const Vector3 & P) {
	Vector3 res;
	float M[16];
	TT.getGLMatrix(&M[0]);
	Vector3 r1(M[0], M[4], M[8]);
	Vector3 r2(M[1], M[5], M[9]);
	Vector3 r3(M[2], M[6], M[10]);
	Vector3 T(M[12], M[13], M[14]);
	res[0] = r1.dot(P) + T[0];
	res[1] = r2.dot(P) + T[1];
	res[2] = r3.dot(P) + T[2];
	return res;
}

// static Vector3 TransformVectorTrfm3D_OK( const Trfm3D *thisTrfm3D, const Vector3 &P) {
//	Vector3 res;
//	float M[16];
//	thisTrfm3D->getGLMatrix(&M[0]);
//	Vector3 r1(M[0], M[4], M[8]);
//	Vector3 r2(M[1], M[5], M[9]);
//	Vector3 r3(M[2], M[6], M[10]);
//	Vector3 T(M[12], M[13], M[14]);
//	res[0] = r1.dot(P);
//	res[1] = r2.dot(P);
//	res[0] = r3.dot(P);
//	return res;
// }

// static void SetRotVecTrfm3D_OK(Trfm3D *thisTrfm, float rx, float ry, float rz,
//                                float theta ) {

//	float costheta,sintheta;

//	costheta = cos(theta);
//	sintheta = sin(theta);

//	thisTrfm->r1X = (costheta + (1 - costheta) * rx * rx);
//	thisTrfm->r2X = ((1 - costheta) * rx * ry - rz * sintheta);
//	thisTrfm->r3X = ((1 - costheta) * rx * rz + ry * sintheta);

//	thisTrfm->r1Y = ((1 - costheta) * rx * ry + rz * sintheta);
//	thisTrfm->r2Y = (costheta + (1 - costheta) * ry * ry);
//	thisTrfm->r3Y = ((1 - costheta) * ry * rz - rx * sintheta);

//	thisTrfm->r1Z = ((1 - costheta) * rx * rz - ry * sintheta);
//	thisTrfm->r2Z = ((1 - costheta) * ry * rz + rx * sintheta);
//	thisTrfm->r3Z = (costheta + (1 - costheta) * rz * rz);
//	thisTrfm->scl = 1.0f;
//	thisTrfm->trX = 0.0f;
//	thisTrfm->trY = 0.0f;
//	thisTrfm->trZ = 0.0f;
//	thisTrfm->dX  = 0.0f;
//	thisTrfm->dY  = 0.0f;
//	thisTrfm->dZ  = 0.0f;
//	thisTrfm->dW  = 1.0f;

// }

void t_testSetRotAxisTrfm3D(float rx, float ry, float rz,
							float px, float py, float pz,
							float x, float y, float z,
							float angle) {
	Trfm3D T;
	Vector3 nr(rx, ry, rz);
	nr = nr.normalize();
	T.setRotAxis(nr,
				 Vector3(px, py, pz),
				 angle * Constants::degree_to_rad);
	Vector3 P(x, y, z);
	Vector3 n = TransformPointTrfm3D_OK(T, P);
	fprintf(fh,"{r =(%.3f, %.3f, %.3f), %.3f deg, p =(%.3f, %.3f, %.3f)}  (%.3f, %.3f, %.3f)->(%.3f, %.3f, %.3f)\n", rx, ry, rz, angle, px, py, pz, x, y, z, n[0], n[1], n[2]);
}

void setRotAxis() {

	BFUN;
	t_testSetRotAxisTrfm3D(1, 1, 1,
						   1, 0, 0,
						   0, 1, 0,
						   90);
	t_testSetRotAxisTrfm3D(1, 1, 1,
						   1, 0, 1,
						   0, 1, 0,
						   45);
	t_testSetRotAxisTrfm3D(1, 1, 1,
						   0, 1, 0,
						   1, 1, 1,
						   45);
}

void t_testAddRotAxisTrfm3D(Trfm3D &T,
							float rx, float ry, float rz,
							float px, float py, float pz,
							float x, float y, float z,
							float angle) {
	Vector3 nr(rx, ry, rz);
	nr = nr.normalize();
	T.addRotAxis(nr,
				 Vector3(px, py, pz),
				 angle * Constants::degree_to_rad);
	Vector3 P(x, y, z);
	Vector3 n = TransformPointTrfm3D_OK(T, P);
	fprintf(fh,"{r =(%.3f, %.3f, %.3f), %.3f deg, p =(%.3f, %.3f, %.3f)}  (%.3f, %.3f, %.3f)->(%.3f, %.3f, %.3f)\n", rx, ry, rz, angle, px, py, pz, x, y, z, n[0], n[1], n[2]);
}

void testAddRotAxisTrfm3D() {

	Trfm3D T;

	BFUN;
	t_testAddRotAxisTrfm3D(T,
						   1, 1, 1,
						   1, 0, 0,
						   0, 1, 0,
						   90);
	t_testAddRotAxisTrfm3D(T,
						   1, 1, 1,
						   1, 0, 1,
						   0, 1, 0,
						   45);
	t_testAddRotAxisTrfm3D(T,
						   1, 1, 1,
						   0, 1, 0,
						   1, 1, 1,
						   45);
}

void t_testSetRotVecTrfm3D(float rx, float ry, float rz,
						   float x, float y, float z,
						   float angle) {
	Trfm3D T;
	Vector3 nr(rx, ry, rz);
	nr = nr.normalize();
	T.setRotVec(nr,
				angle * Constants::degree_to_rad);
	Vector3 P(x, y, z);
	Vector3 n = TransformPointTrfm3D_OK(T, P);
	fprintf(fh,"{r =(%.3f, %.3f, %.3f), %.3f deg} (%.3f, %.3f, %.3f)->(%.3f, %.3f, %.3f)\n", rx, ry, rz, angle, x, y, z, n[0], n[1], n[2]);
}

void testSetRotVecTrfm3D() {

	BFUN;
	t_testSetRotVecTrfm3D(1, 1, 1,
						  0, 1, 0,
						  90);
	t_testSetRotVecTrfm3D(1, 1, 1,
						  0, 1, 0,
						  45);
	t_testSetRotVecTrfm3D(1, 1, 1,
						  1, 1, 1,
						  45);
}

void t_testAddRotVecTrfm3D(Trfm3D &T,
						   float rx, float ry, float rz,
						   float x, float y, float z,
						   float angle) {
	Vector3 nr(rx, ry, rz);
	nr = nr.normalize();
	T.addRotVec(nr,
				angle * Constants::degree_to_rad);
	Vector3 P(x, y, z);
	Vector3 n = TransformPointTrfm3D_OK(T, P);
	fprintf(fh,"{r =(%.3f, %.3f, %.3f), %.3f deg} (%.3f, %.3f, %.3f)->(%.3f, %.3f, %.3f)\n", rx, ry, rz, angle, x, y, z, n[0], n[1], n[2]);
}

void testAddRotVecTrfm3D() {

	Trfm3D T;

	BFUN;
	t_testAddRotVecTrfm3D(T,
						  1, 1, 1,
						  0, 1, 0,
						  90);
	t_testAddRotVecTrfm3D(T,
						  1, 1, 1,
						  0, 1, 0,
						  45);
	t_testAddRotVecTrfm3D(T,
						  1, 1, 1,
						  1, 1, 1,
						  45);
}


void testTransformPointTrfm3D() {

	Trfm3D T;
	Vector3 n;

	BFUN;
	Vector3 p(1.0, 1.0, 1.0);
	n = T.transformPoint(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 1) -> (%.3f, %.3f, %.3f, 1)\n", p[0], p[1], p[2], n[0], n[1], n[2]);

	T.setTrans(Vector3(1, 1, 1));
	n = T.transformPoint(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 1) -> (%.3f, %.3f, %.3f, 1)\n", p[0], p[1], p[2], n[0], n[1], n[2]);
	n = T.transformVector(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 0) -> (%.3f, %.3f, %.3f, 0)\n", p[0], p[1], p[2], n[0], n[1], n[2]);

	T.addScale(2.0);
	n = T.transformPoint(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 1) -> (%.3f, %.3f, %.3f, 1)\n", p[0], p[1], p[2], n[0], n[1], n[2]);
	n = T.transformVector(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 0) -> (%.3f, %.3f, %.3f, 0)\n", p[0], p[1], p[2], n[0], n[1], n[2]);

	T.setTrans(Vector3(1, 1, 1));
	T.addRotY(45 * Constants::degree_to_rad);
	n = T.transformPoint(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 1) -> (%.3f, %.3f, %.3f, 1)\n", p[0], p[1], p[2], n[0], n[1], n[2]);
	n = T.transformVector(p);
	fprintf(fh,"(%.3f, %.3f, %.3f, 0) -> (%.3f, %.3f, %.3f, 0)\n", p[0], p[1], p[2], n[0], n[1], n[2]);

	// p = Vector3(7.0, -2.0, -5.0);
	// Vector3 r(1.0, 1.0, 1.0);
	// float theta = 80 * Constants::degree_to_rad;
	// float alpha, beta;
	// //r.normalize();
	// r.sphereCoordinates(alpha, beta);
	// fprintf(fh, "%.4f, %.4f\n", alpha, beta);
	// T.setRotY(alpha);
	// T.addRotX(-beta);
	// T.addRotZ(theta);
	// T.addRotX(beta);
	// T.addRotY(-alpha);
	// n = T.transformPoint(p);
	// fprintf(fh,"(%.3f, %.3f, %.3f, 1) -> (%.3f, %.3f, %.3f, 1)\n", p[0], p[1], p[2], n[0], n[1], n[2]);

	// T.setRotVec(r, theta);
	// n = T.transformPoint(p);
	// fprintf(fh,"(%.3f, %.3f, %.3f, 1) -> (%.3f, %.3f, %.3f, 1)\n", p[0], p[1], p[2], n[0], n[1], n[2]);
}

int main (int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "[E]: need an output file\n");
		exit(1);
	}

	if ((fh = fopen(argv[1], "w")) == NULL) {
		fprintf(stderr, "[E]  can't create %s\n", argv[1]);
		exit(1);
	}

	//testSetRotVecTrfm3D();
	//testAddRotVecTrfm3D();
	setRotAxis();
	//testAddRotAxisTrfm3D();
	testTransformPointTrfm3D();
	return 0;
}

// Local Variables:
// compile-command: "make TH_DIR=../ MG_DIR=../.."
// End:
