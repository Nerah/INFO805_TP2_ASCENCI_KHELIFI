//
// Created by Utilisateur on 20/02/2020.
//

#ifndef RAY_TRACER_PERIODIC_HPP
#define RAY_TRACER_PERIODIC_HPP

#include "GraphicalObject.h"
#include <math.h>

namespace rt {
    struct PeriodicPlane : public GraphicalObject {
        /// Creates a periodic infinite plane passing through \a c and
        /// tangent to \a u and \a v. Then \a w defines the width of the
        /// band around (0,0) and its period to put material \a band_m,
        /// otherwise \a main_m is used.
        PeriodicPlane(Point3 c, Vector3 u, Vector3 v, Material main_m, Material band_m, Real w) {
            this->c = c;
            this->u = u;
            this->v = v;
            this->main_m = main_m;
            this->band_m = band_m;
            this->w = w;
        };

        void init( Viewer& ) {}

        void draw( Viewer& ) {
            glBegin(GL_TRIANGLES);
            glColor4fv( main_m.ambient );
            glMaterialfv(GL_FRONT, GL_DIFFUSE, main_m.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, main_m.specular);
            glMaterialf(GL_FRONT, GL_SHININESS, main_m.shinyness );
            glVertex3f(0, 0, 0.0);
            glVertex3f(0, 1000.0, 0.0);
            glVertex3f(1000.0, 0, 0.0);
            glEnd();
        }

        Material getMaterial( Point3 p ) {
            Real x = (c - p).dot(u) / u.norm(),
                 y = (c - p).dot(v) / v.norm();

            int roundX = (int) x;
            int roundY = (int) y;

            Real xDiff = x - roundX;
            Real yDiff = y - roundY;

            if(xDiff < 0) xDiff *= -1.0f;
            if(yDiff < 0) yDiff *= -1.0f;

            return (xDiff < w || yDiff < w) ? band_m : main_m;
        }

        Real rayIntersection( const Ray& ray, Point3& p ) {
            Vector3 N = getNormal(c) / getNormal(c).norm(),
                    W = ray.direction / ray.direction.norm(),
                    n0w0 = (c - ray.origin);
            Real t, wn = W.dot(N);

            if(wn == 0.0f) {
                if(N.dot(n0w0) == 0){
                    t = 0;
                    p = ray.origin + W* t;
                    return -1.0f;
                } else return 1.0f;
            }
            t = N.dot(n0w0) / wn;
            p = ray.origin + W* t;
            return t >= 0 ? -1.0f :1.0f;
        }

        Vector3 getNormal( Point3 p ) {
            return p + u.cross( v );
        }

    public:
        Point3 c;
        Vector3 u, v;
        Material band_m, main_m;
        Real w;
    };
}

#endif //RAY_TRACER_PERIODIC_HPP
