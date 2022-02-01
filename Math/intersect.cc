#include <cmath>
#include "intersect.h"
#include "constants.h"
#include "tools.h"

/* | algo           | difficulty | */
/* |----------------+------------| */
/* | BSPherePlane   |          1 | */
/* | BBoxBBox       |          2 | */
/* | BBoxPlane      |          4 | */

// @@ TODO: test if a BSpheres intersects a plane.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int BSpherePlaneIntersect(const BSphere *bs, Plane *pl) {
	/* =================== PUT YOUR CODE HERE ====================== */
	float dist = pl->distance(bs->m_centre);
	dist-=bs->getRadius();
	printf ("distancia ---> %f",dist);
	int respuesta;
	if (abs(dist) < Constants::distance_epsilon) respuesta = IINTERSECT;
	else if (dist > Constants::distance_epsilon) respuesta = IREJECT;
	else respuesta = -IREJECT;

	/* =================== END YOUR CODE HERE ====================== */
	
	return respuesta;
}


// @@ TODO: test if two BBoxes intersect.
//! Returns :
//    IINTERSECT intersect
//    IREJECT don't intersect

int  BBoxBBoxIntersect(const BBox *bba, const BBox *bbb ) {
	/* =================== PUT YOUR CODE HERE ====================== */
	float coord_a_min , coord_a_max , coord_b_min, coord_b_max;
	bool interseccion = true;
	int i=0;
	while ( interseccion && i!=3){
		coord_a_min = bba->m_min[i];
		coord_a_max = bba->m_max[i];
		coord_b_min = bbb->m_min[i];
		coord_b_max = bbb->m_max[i];
		
		interseccion = (coord_b_min <= coord_a_min && coord_b_max >= coord_a_min) ||
					   (coord_b_min <= coord_a_max && coord_b_max >= coord_a_max);
		i++;
	}

	return (interseccion)? IINTERSECT : IREJECT;
	/* =================== END YOUR CODE HERE ====================== */
}

// @@ TODO: test if a BBox and a plane intersect.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int  BBoxPlaneIntersect (const BBox *theBBox, Plane *thePlane) {
	/* =================== PUT YOUR CODE HERE ====================== */
	float max_simil = 0, aux;
	Vector3 lista_diag[4], temp;
	lista_diag[0] = (theBBox->m_max - theBBox->m_min);
	int max= 0;

	//calculo de diagonales y de la más similar a la normal del plano
	for (int i =0; i<4; i++){
		temp = lista_diag[0];
		//calculo y almacenamiento de las diagonales
		if (i!= 0){
			temp[i] =  theBBox->m_min[i] - theBBox->m_max[i] ;
			lista_diag[i] = temp; 
		}
		
		//calculo y almacenamiento de la máxima similitud junto con su indice
		aux= thePlane->m_n.dot( temp ) ;
		aux /= (thePlane->m_n.lengthSquare() * temp.lengthSquare());
		if (  abs(aux)  >  max_simil){
			max_simil= abs(aux);
			max= i;
		}
	}

	//obtenemos el vector con el angulo más similar a la normal
	temp = lista_diag[ max ];
	Vector3 n_max = theBBox->m_max , n_min = theBBox->m_min;

	//se calculan los nuevos puntos correspondientes a la diagonal
	if (max != 0){
		n_max[ max ]= n_min[ max ];
		n_min[ max ]= theBBox->m_max[ max ];
	}
	float dist_Pl_min , dist_PL_max;

	dist_Pl_min = thePlane->signedDistance(n_min);
	dist_PL_max = thePlane->signedDistance(n_max);

	//revisar creo que falta calcular los vertices con respecto al plano
	/* =================== END YOUR CODE HERE ====================== */
}

// Test if two BSpheres intersect.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBSphereIntersect(const BSphere *bsa, const BSphere *bsb ) {

	Vector3 v;
	v = bsa->m_centre - bsb->m_centre;
	float ls = v.dot(v);
	float rs = bsa->m_radius + bsb->m_radius;
	if (ls > (rs * rs)) return IREJECT; // Disjoint
	return IINTERSECT; // Intersect
}


// Test if a BSpheres intersect a BBox.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBBoxIntersect(const BSphere *sphere, const BBox *box) {

	float d;
	float aux;
	float r;

	r = sphere->m_radius;
	d = 0;

	aux = sphere->m_centre[0] - box->m_min[0];
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[0] - box->m_max[0];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}

	aux = (sphere->m_centre[1] - box->m_min[1]);
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[1] - box->m_max[1];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}

	aux = sphere->m_centre[2] - box->m_min[2];
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[2] - box->m_max[2];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}
	if (d > r * r) return IREJECT;
	return IINTERSECT;
}

// Test if a Triangle intersects a ray.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int IntersectTriangleRay(const Vector3 & P0,
						 const Vector3 & P1,
						 const Vector3 & P2,
						 const Line *l,
						 Vector3 & uvw) {
	Vector3 e1(P1 - P0);
	Vector3 e2(P2 - P0);
	Vector3 p(crossVectors(l->m_d, e2));
	float a = e1.dot(p);
	if (fabs(a) < Constants::distance_epsilon) return IREJECT;
	float f = 1.0f / a;
	// s = l->o - P0
	Vector3 s(l->m_O - P0);
	float lu = f * s.dot(p);
	if (lu < 0.0 || lu > 1.0) return IREJECT;
	Vector3 q(crossVectors(s, e1));
	float lv = f * q.dot(l->m_d);
	if (lv < 0.0 || lv > 1.0) return IREJECT;
	uvw[0] = lu;
	uvw[1] = lv;
	uvw[2] = f * e2.dot(q);
	return IINTERSECT;
}

/* IREJECT 1 */
/* IINTERSECT 0 */

const char *intersect_string(int intersect) {

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
