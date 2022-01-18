#include "t.h"
#include "line.h"
#include "plane.h"
#include "bbox.h"
#include "bsphere.h"
#include "intersect.h"
#include "tools.h"
#include "constants.h"
#include "trfm3D.h"

FILE *fh;

static const char *intersectStr(int intersect) {

	static const char *iint = "IINTERSECT";
	static const char *prej = "IREJECT";
	static const char *mrej = "-IREJECT";
	static const char *error = "IERROR";

	const char *result = error;

	switch (intersect) {
	case IINTERSECT:
		result = iint;
		break;
	case +IREJECT:
		result = prej;
		break;
	case -IREJECT:
		result = mrej;
		break;
	}
	return result;
}

BBox *createBBox_moveaxis(const Vector3 & P_min,
						  const Vector3 & P_max,
						  int axis, float step) {
	Vector3 Pmin(P_min);
	Vector3 Pmax(P_max);
	if (axis == 0) {
		// X
		Pmin[0] += step;
		Pmax[0] += step;
	}
	if (axis == 1) {
		// Y
		Pmin[1] += step;
		Pmax[1] += step;
	}
	if (axis == 2) {
		// Z
		Pmin[2] += step;
		Pmax[2] += step;
	}
	return new BBox(Pmin, Pmax);
}

void print_bbox(BBox *bba, BBox *bbb, int result) {
	fprintf(fh,
			"BBox:{(%2.2f, %2.2f, %2.2f), (%2.2f, %2.2f, %2.2f)}, BBox{(%2.2f, %2.2f, %2.2f), (%2.2f, %2.2f, %2.2f)} : %s\n",
			bba->m_min[0], bba->m_min[1], bba->m_min[2],
			bba->m_max[0], bba->m_max[1], bba->m_max[2],
			bbb->m_min[0], bbb->m_min[1], bbb->m_min[2],
			bbb->m_max[0], bbb->m_max[1], bbb->m_max[2],
			intersectStr(result));
}

void BBoxBBoxIntersect() {
	BBox *bs1, *bs2;
	int i;
	BFUN;
	bs1 = new BBox(Vector3(0, 0, 0),
				   Vector3(1, 1, 1));
	for(i=0; i < 3; i++) {
		bs2 = createBBox_moveaxis(Vector3(0, 0, 0),
								  Vector3(1, 1, 1),
								  i, 1.1);
		print_bbox(bs1, bs2, BBoxBBoxIntersect(bs1, bs2));
		delete bs2;
		bs2 = createBBox_moveaxis(Vector3(0, 0, 0),
								  Vector3(1, 1, 1),
								  i, -1.1);
		print_bbox(bs1, bs2, BBoxBBoxIntersect(bs1, bs2));
		delete bs2;
	}
	bs2 = new BBox(Vector3(0.9, 0.8, -0.7),
				   Vector3(2, 2, 2));
	print_bbox(bs1, bs2, BBoxBBoxIntersect(bs1, bs2));
	delete bs2;
	delete bs1;
}

static void print_bbox_Plane(BBox *bb, Plane *p, int result) {
	fprintf(fh,
			"BBox:{(%2.2f, %2.2f, %2.2f), (%2.2f, %2.2f, %2.2f)}, plane{N:(%.3f, %.3f, %.3f), d:%.3f}: %s\n",
			bb->m_min[0], bb->m_min[1], bb->m_min[2],
			bb->m_max[0], bb->m_max[1], bb->m_max[2],
			p->m_n[0], p->m_n[1], p->m_n[2], p->m_d,
			intersectStr(result));
}

int BBoxPlaneIntersect () {

	int ema;
	BBox * bb;
	Plane * p;

	BFUN;
	Vector3 Xmin(-1.0, -1.0, -1.0);
	Vector3 Xmax(1.0, 1.0, 1.0);
	bb = new BBox(Xmin, Xmax);
	Vector3 N(1.0, 1.0, 1.0);
	float d = 0.5;
	N = N.normalize();
	p = new Plane(N, d);
	ema = BBoxPlaneIntersect(bb, p);
	print_bbox_Plane(bb, p, ema);
	delete p;

	N[0] = 0;
	N[1] = 1.0;
	N[2] = 0;
	d = 20;
	N = N.normalize();
	p = new Plane(N, d);
	ema = BBoxPlaneIntersect(bb, p);
	print_bbox_Plane(bb, p, ema);
	delete p;

	N[0] = 0;
	N[1] = 1.0;
	N[2] = 0;
	d = -20;
	N = N.normalize();
	p = new Plane(N, d);
	ema = BBoxPlaneIntersect(bb, p);
	print_bbox_Plane(bb, p, ema);
	delete p;

	delete bb;
	return ema;
}

static void print_bsphere_bbox(BSphere *sp, BBox *bb, int result) {
	fprintf(fh,
			"BBox:{(%2.2f, %2.2f, %2.2f), (%2.2f, %2.2f, %2.2f)},  BSphere: {(%2.2f, %2.2f, %2.2f} , %2.2f} : %s\n",
			bb->m_min[0], bb->m_min[1], bb->m_min[2],
			bb->m_max[0], bb->m_max[1], bb->m_max[2],
			sp->m_centre[0], sp->m_centre[1], sp->m_centre[2], sp->m_radius,
			intersectStr(result));
}

void testBSphereBBox() {

	BBox * bb;
	BSphere *bs1;

	BFUN;
	bs1 = new BSphere(Vector3(0, 0, 0), 1);
	bb = new BBox(Vector3(-1.0, -1.0, -1.0),
				  Vector3(1.0, 1.0, 1.0));
	print_bsphere_bbox(bs1, bb, BSphereBBoxIntersect(bs1, bb));
	delete bs1;
	delete bb;
	float r = 1.0;
	Vector3 t = Vector3(1, -1, 0);
	t = t.normalize();
	bs1 = new BSphere(Vector3(1, -1, 0) + r/2.0 * t, r);
	bb = new BBox(Vector3(-1.0, -1.0, -1.0),
				  Vector3(1.0, 1.0, 1.0));
	print_bsphere_bbox(bs1, bb, BSphereBBoxIntersect(bs1, bb));
	delete bs1;
	delete bb;
	bs1 = new BSphere(Vector3(1, -1, 0) + r * t, r);
	bb = new BBox(Vector3(-1.0, -1.0, -1.0),
				  Vector3(1.0, 1.0, 1.0));
	print_bsphere_bbox(bs1, bb, BSphereBBoxIntersect(bs1, bb));
	delete bs1;
	delete bb;
}

static void print_bsphere_bsphere(BSphere *sp1, BSphere *sp2, int result) {
	fprintf(fh,
			"BSphere: {(%2.2f, %2.2f, %2.2f} , %2.2f}, BSphere: {(%2.2f, %2.2f, %2.2f} , %2.2f} : %s\n",
			sp1->m_centre[0], sp1->m_centre[1], sp1->m_centre[2], sp1->m_radius,
			sp2->m_centre[0], sp2->m_centre[1], sp2->m_centre[2], sp2->m_radius,
			intersectStr(result));
}


void testBSphereBSphere() {

	BSphere *bs1, *bs2;

	BFUN;
	bs1 = new BSphere(Vector3(0, 0, 0), 1.0);
	bs2 = new BSphere(Vector3(0.5, 1.0, 1.0), 1.0);
	print_bsphere_bsphere(bs1, bs2, BSphereBSphereIntersect(bs1, bs2));
	delete bs2;

	bs2 = new BSphere(Vector3(1.78, 0.91, 0), 1);
	print_bsphere_bsphere(bs1, bs2, BSphereBSphereIntersect(bs1, bs2));
	delete bs2;

	bs2 = new BSphere(Vector3(0.191, 0.962, 0.195), 1);
	print_bsphere_bsphere(bs1, bs2, BSphereBSphereIntersect(bs1, bs2));
	delete bs2;

	bs2 = new BSphere(Vector3(2.001, 0, 0), 1);
	print_bsphere_bsphere(bs1, bs2, BSphereBSphereIntersect(bs1, bs2));
	delete bs2;
	delete bs1;

}

static void print_bsphere_Plane(BSphere *sp1, Plane *p, int result) {

	fprintf(fh,
			"BSphere: {(%2.2f, %2.2f, %2.2f} , %2.2f}, plane{N:(%.3f, %.3f, %.3f), d:%.3f}: %s\n",
			sp1->m_centre[0], sp1->m_centre[1], sp1->m_centre[2], sp1->m_radius,
			p->m_n[0], p->m_n[1], p->m_n[2], p->m_d,
			intersectStr(result));
}


void BSpherePlaneIntersect () {

	BSphere *bs1;
	Plane *p;
	Vector3 n;
	float d;
	int ema;

	BFUN;
	bs1 = new BSphere(Vector3(0, 0, 0), 1);

	n[0] = 1;
	n[1] = 1;
	n[2] = 1;
	n = n.normalize();
	d = 5;
	p = new Plane(n, d);
	ema = BSpherePlaneIntersect(bs1, p);
	print_bsphere_Plane(bs1, p, ema);
	delete p;

	n[0] = 1;
	n[1] = 1;
	n[2] = 1;
	d = .5;
	n = n.normalize();

	p = new Plane(n, d);
	ema = BSpherePlaneIntersect(bs1, p);
	print_bsphere_Plane(bs1, p, ema);
	delete p;
	delete bs1;
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

	BBoxBBoxIntersect();
	BBoxPlaneIntersect();
	//testBSphereBSphere();
	BSpherePlaneIntersect();
	//testBSphereBBox();

	return 0;
}

// Local Variables:
// compile-command: "make TH_DIR=../ MG_DIR=../.."
// End:
