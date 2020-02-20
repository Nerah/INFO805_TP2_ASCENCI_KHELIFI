/**
@file Renderer.h
@author JOL
*/
#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Color.h"
#include "Image2D.h"
#include "Ray.h"
#include "Background.h"
#include "Utils.h"
#include <random>
#include <iostream>

// constants
#define EPSILON 0.001f

/// Namespace RayTracer
namespace rt {
    inline void progressBar( std::ostream& output,
                             const double currentValue, const double maximumValue)
    {
        static const int PROGRESSBARWIDTH = 60;
        static int myProgressBarRotation = 0;
        static int myProgressBarCurrent = 0;
        // how wide you want the progress meter to be
        double fraction = currentValue /maximumValue;

        // part of the progressmeter that's already "full"
        int dotz = static_cast<int>(floor(fraction * PROGRESSBARWIDTH));
        if (dotz > PROGRESSBARWIDTH) dotz = PROGRESSBARWIDTH;

        // if the fullness hasn't changed skip display
        if (dotz == myProgressBarCurrent) return;
        myProgressBarCurrent = dotz;
        myProgressBarRotation++;

        // create the "meter"
        int ii=0;
        output << "[";
        // part  that's full already
        for ( ; ii < dotz;ii++) output<< "#";
        // remaining part (spaces)
        for ( ; ii < PROGRESSBARWIDTH;ii++) output<< " ";
        static const char* rotation_string = "|\\-/";
        myProgressBarRotation %= 4;
        output << "] " << rotation_string[myProgressBarRotation]
               << " " << (int)(fraction*100)<<"/100\r";
        output.flush();
    }

    /// This structure takes care of rendering a scene.
    struct Renderer {

        /// The scene to render
        Scene* ptrScene;
        /// The origin of the camera in space.
        Point3 myOrigin;
        /// (myOrigin, myOrigin+myDirUL) forms a ray going through the upper-left
        /// corner pixel of the viewport, i.e. pixel (0,0)
        Vector3 myDirUL;
        /// (myOrigin, myOrigin+myDirUR) forms a ray going through the upper-right
        /// corner pixel of the viewport, i.e. pixel (width,0)
        Vector3 myDirUR;
        /// (myOrigin, myOrigin+myDirLL) forms a ray going through the lower-left
        /// corner pixel of the viewport, i.e. pixel (0,height)
        Vector3 myDirLL;
        /// (myOrigin, myOrigin+myDirLR) forms a ray going through the lower-right
        /// corner pixel of the viewport, i.e. pixel (width,height)
        Vector3 myDirLR;
        /// On rajoute un pointeur vers un objet Background
        Background* ptrBackground;


        int myWidth;
        int myHeight;

        Renderer() : ptrScene( 0 ) {}
        Renderer( Scene& scene ) : ptrScene( &scene ) {
            ptrBackground = new Background();

        }
        void setScene( rt::Scene& aScene ) { ptrScene = &aScene; }

        void setViewBox( Point3 origin,
                         Vector3 dirUL, Vector3 dirUR, Vector3 dirLL, Vector3 dirLR )
        {
            myOrigin = origin;
            myDirUL = dirUL;
            myDirUR = dirUR;
            myDirLL = dirLL;
            myDirLR = dirLR;
        }

        void setResolution( int width, int height )
        {
            myWidth  = width;
            myHeight = height;
        }


        /// The main rendering routine
        void render( Image2D<Color>& image, int max_depth )
        {
            std::cout << "Rendering into image ... might take a while." << std::endl;
            image = Image2D<Color>( myWidth, myHeight );
            for ( int y = 0; y < myHeight; ++y )
            {
                Real    ty   = (Real) y / (Real)(myHeight-1);
                progressBar( std::cout, ty, 1.0 );
                Vector3 dirL = (1.0f - ty) * myDirUL + ty * myDirLL;
                Vector3 dirR = (1.0f - ty) * myDirUR + ty * myDirLR;
                dirL        /= dirL.norm();
                dirR        /= dirR.norm();
                for ( int x = 0; x < myWidth; ++x )
                {
                    Real    tx   = (Real) x / (Real)(myWidth-1);
                    Vector3 dir  = (1.0f - tx) * dirL + tx * dirR;
                    Ray eye_ray  = Ray( myOrigin, dir, max_depth );
                    Color result = trace( eye_ray );
                    image.at( x, y ) = result.clamp();
                }
            }
            std::cout << "Done." << std::endl;
        }

        /// anti-aliasing renderer
        void randomRender( Image2D<Color>& image, int max_depth )
        {
            int antialiasing = 20;

            std::cout << "Rendering into image ... might take a while." << std::endl;
            image = Image2D<Color>( myWidth, myHeight );
            for ( int y = 0; y < myHeight; ++y ) {
                for ( int x = 0; x < myWidth; ++x ) {
                    Color result = Color(0.0f,0.0f,0.0f);
                    for(int i = 0 ; i<antialiasing ; i++) {
                        Real    progress   = (Real) y / (Real)(myHeight-1);

                        Real    ty   = ((Real) y + MathUtils::randFloat(0.0, 1.0))/ (Real)(myHeight-1);
                        progressBar( std::cout, progress, 1.0 );
                        Vector3 dirL = (1.0f - ty) * myDirUL + ty * myDirLL;
                        Vector3 dirR = (1.0f - ty) * myDirUR + ty * myDirLR;
                        dirL        /= dirL.norm();
                        dirR        /= dirR.norm();
                        Real    tx   = ((Real) x + MathUtils::randFloat(0.0, 1.0)) / (Real)(myWidth-1);
                        Vector3 dir  = ((1.0f - tx) * dirL) + (tx * dirR);
                        Ray eye_ray  = Ray( myOrigin, dir, max_depth );
                        result = result + trace( eye_ray );
                    }
                    result =  result/antialiasing;
                    image.at( x, y ) = result.clamp();
                }
            }
            std::cout << "Done." << std::endl;
        }

        /// The rendering routine for one ray.
        /// @return the color for the given ray.
        Color trace( const Ray& ray )
        {
            assert( ptrScene != 0 );
            Color result = Color( 0.0, 0.0, 0.0 );
            GraphicalObject* obj_i = 0; // pointer to intersected object
            Point3           p_i;       // point of intersection

            // Look for intersection in this direction.
            Real ri = ptrScene->rayIntersection( ray, obj_i, p_i );
            // Nothing was intersected
            if (ri >= 0.0f) return this->background(ray);

            Material m = obj_i->getMaterial(p_i);

            if(ray.depth > 0 && m.coef_reflexion != 0) {
                Vector3 rRefl = RayUtils::reflect(ray.direction, obj_i->getNormal(p_i));
                Ray newRay = Ray(p_i + rRefl * EPSILON, rRefl, ray.depth - 1);
                Color cRefl = trace(newRay);
                result += cRefl * m.specular * m.coef_reflexion;
            }
            if(ray.depth > 0 && m.coef_refraction != 0) {
                Ray newRay = refractionRay(ray,p_i,obj_i->getNormal(p_i), m);
                Color cRefrac = trace(newRay);
                result += cRefrac * m.diffuse * m.coef_refraction;
            }

            Color tmp = illumination(ray, obj_i, p_i);
            if(ray.depth != 0) tmp = tmp * obj_i->getMaterial(p_i).coef_diffusion;
            result += tmp;
            return result;
        }

        Color illumination(const Ray& ray, GraphicalObject* obj, Point3 p){
            Material m = obj->getMaterial(p);
            Color final = Color(0,0,0);

            for(auto it : ptrScene->myLights) {
                Vector3 lDir = it->direction(p);
                Color lColor = it->color(p);
                Ray pointToLight = Ray(p, lDir);
                Color colorShadow = shadow(pointToLight, lColor);

                Vector3 normalP = obj->getNormal(p);
                Vector3 w = RayUtils::reflect(ray.direction, normalP);

                Real cosb = w.dot(lDir) / (lDir.norm() * w.norm());
                if(cosb < 0) cosb = 0;
                Real coeffSpec = std::pow(cosb, m.shinyness);

                Real coeffDiff = normalP.dot(lDir) / (lDir.norm() * normalP.norm());
                if(coeffDiff < 0) coeffDiff = 0;

                final += it->color(p) * m.specular * coeffSpec;
                final += it->color(p) * m.diffuse * coeffDiff;
                final = final * colorShadow;
            }
            final += m.ambient;
            return final;
        }

        /// Affiche les sources de lumières avant d'appeler la fonction qui
        /// donne la couleur de fond.
        Color background( const Ray& ray )
        {
            Color result = Color( 0.0, 0.0, 0.0 );
            for ( Light* light : ptrScene->myLights )
            {
                Real cos_a = light->direction( ray.origin ).dot( ray.direction );
                if ( cos_a > 0.99f )
                {
                    Real a = acos( cos_a ) * 360.0 / M_PI / 8.0;
                    a = std::max( 1.0f - a, 0.0f );
                    result += light->color( ray.origin ) * a * a;
                }
            }
            if ( ptrBackground != 0 ) result += ptrBackground->backgroundColor( ray );
            return result;
        }

        /// Calcule la couleur de la lumière (donnée par light_color) dans la
        /// direction donnée par le rayon. Si aucun objet n'est traversé,
        /// retourne light_color, sinon si un des objets traversés est opaque,
        /// retourne du noir, et enfin si les objets traversés sont
        /// transparents, attenue la couleur.
        Color shadow( const Ray& ray, Color light_color ){
            GraphicalObject* obj_i = 0; // pointer to intersected object
            Point3           p_i;       // point of intersection
            Point3 newP =  ray.origin;
            while(light_color.max() > 0.003f){
                newP =  newP + ray.direction * EPSILON; // on decale le point p;
                // Look for intersection in this direction.
                Ray newRay = Ray(newP,ray.direction,ray.depth);
                Real ri = ptrScene->rayIntersection( newRay, obj_i, p_i );
                if ( ri >= 0.0f ){
                    break;
                }
                Material m = obj_i->getMaterial(p_i);
                light_color = light_color * m.diffuse * m.coef_refraction;
                newP = p_i;

            }
            return light_color;
        }

        Ray refractionRay( const Ray& aRay, const Point3& p, Vector3 N, const Material& m ){
            Real tmp;
            Real r = m.in_refractive_index / m.out_refractive_index;
            Real c  = (-1.0f) * N.dot(aRay.direction);

            //When the ray is inside the object and go out
            if(aRay.direction.dot(N) <= 0 ) {
                r = 1.0f /r;
            }
            if(c>0)
                tmp = r*c - sqrt(1 - ( (r*r) * (1 - (c*c) )));
            else {
                tmp = r * c + sqrt(1 - ((r * r) * (1 - (c * c))));
            }

            Vector3 vRefrac = Vector3(r*aRay.direction + tmp * N);

            //Total reflexion
            if( 1 - ( (r*r) * (1 - (c*c) )) < 0) {
                vRefrac = RayUtils::reflect(aRay.direction, N);
            }

            Ray newRay = Ray(p + vRefrac * EPSILON,vRefrac,aRay.depth -1);

            return newRay;
        }

    };



} // namespace rt

#endif // #define _RENDERER_H_
