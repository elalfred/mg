#include "t.h"
#include "tools.h"
#include "trfm3D.h"
#include "scene.h"
#include "node.h"
#include "gObject.h"
#include "gObjectManager.h"
#include "list.h"


void create_scene() {

	GObject *gobj;
	GObject *gobj2;
	GObject *gobj3;
	GObject *gobj4;
	GObject *gobj5;

	// sceneRegisterGObject takes ownership of geometric objects
	gobj = SceneRegisterGObject( "../../obj/spheres/", "solid.obj");
	//gobj2 = CreateFromWavefront( "./obj/spheres/", "smooth.obj");
	gobj2 = SceneRegisterGObject( "../../obj/chapel/", "chapel.obj");
	gobj3 = SceneRegisterGObject( "../../obj/cubes/", "cubo.obj");
	gobj4 = SceneRegisterGObject( "../../obj/cubes/", "cubotex.obj");
	gobj5 = SceneRegisterGObject( "../../obj/floor/", "floor.obj");

	/*   auxNode = CreateNodeGobj(gobj, &T); */
	/*   sceneAttachNode(auxNode); // takes ownership */

	SetTransTrfm3D(&T, 0, -10, -100);
	myNode = CreateNodeTrfm(&T);
	sceneAttachNode(myNode);

	SetTransTrfm3D(&T, -20, 0, 0);
	auxNode = CreateNodeGobj(gobj, &T);
	attachNode(myNode, auxNode); // takes ownership

	SetTransTrfm3D(&T, 20, 0, 0);
	auxNode = CreateNodeGobj(gobj2, &T);
	attachNode(myNode, auxNode); // takes ownership

	SetTransTrfm3D(&T, 0, 0, -20);
	auxNode = CreateNodeGobj(gobj3, &T);
	attachNode(myNode, auxNode); // takes ownership

	SetTransTrfm3D(&T, 0, 0, 20);
	auxNode = CreateNodeGobj(gobj4, &T);
	attachNode(myNode, auxNode); // takes ownership

	SetTransTrfm3D(&T, 0, -10, 00);
	auxNode = CreateNodeGobj(gobj5, &T);
	attachNode(myNode, auxNode); // takes ownership

}

int main(int argc, char *argv[]) {
	create_scene();
	return 0;
}
