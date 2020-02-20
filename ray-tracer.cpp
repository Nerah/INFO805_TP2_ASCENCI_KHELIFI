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
#include "WaterPlane.hpp"

using namespace std;
using namespace rt;


void addBubble( Scene& scene, Point3 c, Real r, Material transp_m )
{
    Material revert_m = transp_m;
    std::swap( revert_m.in_refractive_index, revert_m.out_refractive_index );
    Sphere* sphere_out = new Sphere( c, r, transp_m );
    Sphere* sphere_in  = new Sphere( c, r-0.02f, revert_m );
    scene.addObject( sphere_out );
    scene.addObject( sphere_in );
}

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc,argv);

    // Creates a 3D scene
    Scene scene;


    // Light at infinity

    Light* light1 = new PointLight( GL_LIGHT1, Point4( 20,5,10,1 ),
                                    Color( 1.0, 0.8, 0.8 ) );

    scene.addLight( light1 );
    // Objects
    Sphere* sphere1 = new Sphere( Point3( 0, 3, 2), 2.0, Material::emerald() );
    Sphere* sphere10 = new Sphere( Point3( 0, 9, 2), 2.0, Material::emerald() );
    Sphere* sphere2 = new Sphere( Point3( 10, 3, 2), 2.0, Material::emerald() );
    Sphere* sphere11 = new Sphere( Point3( 10, 9, 2), 2.0, Material::emerald() );
    Sphere* sphere3 = new Sphere( Point3( 6, 6, 3), 3.0, Material::emerald() );
    Sphere* sphere4 = new Sphere( Point3( 5, 6, 3), 3.0, Material::emerald() );
    Sphere* sphere5 = new Sphere( Point3( 4, 6, 3), 3.0, Material::emerald() );
    Sphere* sphere6 = new Sphere( Point3( 7, 6, 3), 3.0, Material::emerald() );
    Sphere* sphere7 = new Sphere( Point3( 8, 6, 3), 3.0, Material::emerald() );
    Sphere* sphere8 = new Sphere( Point3( 3, 6, 3), 3.0, Material::emerald() );
    Sphere* sphere9 = new Sphere( Point3( 9, 6, 3), 3.0, Material::emerald() );
    scene.addObject( sphere1 );
    scene.addObject( sphere10 );
    scene.addObject( sphere2 );
    scene.addObject( sphere11 );
    scene.addObject( sphere3 );
    scene.addObject( sphere4 );
    scene.addObject( sphere5 );
    scene.addObject( sphere6 );
    scene.addObject( sphere7 );
    scene.addObject( sphere8 );
    scene.addObject( sphere9 );

    // Un sol noir et blanc

    PeriodicPlane* pplane = new PeriodicPlane( Point3( 0, 0, -1 ), Vector3( 5, 0, 0 ), Vector3( 0, 5, 0 ),
    Material::whitePlastic(), Material::darkMatter(), 0.1f );


    WaterPlane* wplane = new WaterPlane(Point3( 0, 0, 0 ), Vector3( 5, 0, 0 ), Vector3( 0, 5, 0 ),
                                        Material::blueWater());

    scene.addObject( pplane );
    scene.addObject( wplane );




    // Instantiate the viewer.
    Viewer viewer;
    // Give a name
    viewer.setWindowTitle("Ray-tracer preview");

    // Sets the scene
    viewer.setScene( scene );

    // Make the viewer window visible on screen.
    viewer.show();
    // Run main loop.
    application.exec();
    return 0;
}
