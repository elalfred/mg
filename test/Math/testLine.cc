#include "t.h"
#include "line.h"
#include "tools.h"
#include "constants.h"

FILE *fh;

static void PrintLine_(Line & l) {
	fprintf(fh, "{O:(%.3f, %.3f,%.3f) N:(%.3f, %.3f,%.3f)}\n", l.m_O[0], l.m_O[1], l.m_O[2], l.m_d[0], l.m_d[1], l.m_d[2]);
}

void setFromAtoB() {

	BFUN;
	Line l;
	// CreateLinePP can normalize A->B vector or not. Normalize always in test!
	l.setFromAtoB(Vector3(0.0, 0.0, 0.0),
				  Vector3(0.707, 0.707, 0.0));
	PrintLine_(l);
}

void testDistance () {

	BFUN;

	Line l;
	float dist, u;

	l.setFromAtoB(Vector3(0.0, 0.0, 0.0),
				  Vector3(0.0, 1.0, 0.0));
	Vector3 v(0.0, 1.0, 0.0);
	u = l.paramDistance(v);
	dist = l.distance(v);
	PrintLine_(l);
	fprintf(fh, "u:%.3f dist:%.3f\n", u, dist);

	// CreateLinePP can normalize A->B vector or not. Normalize always in test!
	v[0] = 100;
	v[1] = -2;
	v[2] = 6;
	v = v.normalize();
	l.setFromAtoB(Vector3(0.0, 0.0, 0.0),
				  v);
	v[0] = 0;
	v[1] = 1;
	v[2] = 0;
	u = l.paramDistance(v);
	dist = l.distance(v);
	PrintLine_(l);
	fprintf(fh, "u:%.3f dist:%.3f\n", u, dist);

	/* printf("Introduce O and d\n"); */
	/* printf("Ox, Oy, Oz:"); */
	/* scanf("%f, %f, %f", &Ox, &Oy, &Oz); */

	/* printf("Dx, Dy, Dz:"); */
	/* scanf("%f, %f, %f", &Dx, &Dy, &Dz); */

	/* printf("Px, Py, Pz:"); */
	/* scanf("%f, %f, %f", &vx, &vy, &vz); */

	/* l = CreateLine(Ox, Oy, Oz, Dx, Dy, Dz); */
	/* u = ParamDistance(l, vx, vy, vz); */
	/* dist = DistanceP(l, vx, vy, vz); */

	/* printf("Line: u:%.3f dist:%.3f\n", u, dist); */
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "[E]: need an output file\n");
		exit(1);
	}

	if ((fh = fopen(argv[1], "w")) == NULL) {
		fprintf(stderr, "[E]  can't create %s\n", argv[1]);
		exit(1);
	}
	setFromAtoB();
	testDistance();
	return 0;
}

// Local Variables:
// compile-command: "make TH_DIR=../ MG_DIR=../.."
// End:
