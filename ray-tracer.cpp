#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Material.h"
#include "PointLight.h"
#include "PeriodicPlane.hpp"

using namespace std;
using namespace rt;

void addBubble( Scene& scene, Point3 c, Real r, Material transp_m ) {
    Material revert_m = transp_m;
    std::swap( revert_m.in_refractive_index, revert_m.out_refractive_index );
    Sphere* sphere_out = new Sphere( c, r, transp_m );
    Sphere* sphere_in  = new Sphere( c, r - 0.02f, revert_m );
    scene.addObject( sphere_out );
    scene.addObject( sphere_in );
}

void addLight( Scene& scene, GLenum light_number, Point4 pos, Color emission_color ) {
    Light* light = new PointLight( light_number, pos, emission_color );
    scene.addLight( light );
}

void addPeriodicPlane(Scene& scene, Point3 c, Vector3 u, Vector3 v, Material main_m, Material band_m, Real w) {
    PeriodicPlane* plane = new PeriodicPlane(c, u, v, main_m, band_m, w);
    scene.addObject( plane );
}

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Creates a 3D scene
    Scene scene;

    // Light at infinity
    addLight(scene, GL_LIGHT1, Point4( 30, 15, 30, 1 ), Color( 1.0, 0.8, 0.8 ));

    // Objects
    addBubble(scene, Point3( 3, 9, 2 ), 2.0, Material::bronze());
    addBubble(scene, Point3( 6, 6, 3 ), 1.0, Material::emerald());
    addBubble(scene, Point3( 10, 3, 3 ), 3.0, Material::whitePlastic());

    addBubble(scene, Point3( 16, 3, 3 ), 3.0, Material::whitePlastic());
    addBubble(scene, Point3( 16, 3, 6 ), 3.0, Material::whitePlastic());
    addBubble(scene, Point3( 16, 3, 9 ), 3.0, Material::whitePlastic());
    addBubble(scene, Point3( 16, 3, 12 ), 3.0, Material::whitePlastic());
    addBubble(scene, Point3( 16, 3, 15 ), 3.0, Material::whitePlastic());
    addBubble(scene, Point3( 16, 1, 12 ), 2.0, Material::whitePlastic());
    addBubble(scene, Point3( 16, 5, 12 ), 2.0, Material::whitePlastic());

    addPeriodicPlane(
            scene,
            Point3( 0, 0, 0 ),
            Vector3( 5, 0, 0 ),
            Vector3( 0, 5, 0 ),
            Material::whitePlastic(),
            Material::blackMatter(),
            0.05f
    );

    // Instantiate the viewer.
    Viewer viewer;
    // Give a name
    viewer.setWindowTitle("Jolie petite fenêtre <3");

    // Sets the scene
    viewer.setScene( scene );

    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();
    return 0;
}
