//
// Created by Utilisateur on 17/02/2020.
//

#ifndef TP2_ASCENCI_KHELIFI_BACKGROUND_H
#define TP2_ASCENCI_KHELIFI_BACKGROUND_H

namespace rt {
    struct Background {
        Color backgroundColor( const Ray& ray ) {
            Color cInter;

            float z  = ray.direction.at(2);
            if(z >= 0.0f && z <=0.5f) return Color(1 - 2 * z, 1 - 2 * z,1);
            if( z > 0.5f && z <= 1.0f) return Color(0, 0, 1.0f - (z - 0.5f) * 2);

            Real x = -0.5f * ray.direction[0] / ray.direction[2];
            Real y = -0.5f * ray.direction[1] / ray.direction[2];
            Real d = sqrt(x*x + y*y);
            Real t = std::min(d, 30.0f) / 30.0f;
            x -= floor(x);
            y -= floor(y);

            if ((x >= 0.5f && y >= 0.5f) || (x < 0.5f && y < 0.5f)) cInter = Color( 0.7f, 0.7f, 0.7f );
            else cInter = Color( 0.9f, 0.9f, 0.9f );

            return (1.0f - t) * cInter + t * Color( 1.0f, 1.0f, 1.0f );
        }
    };
}

#endif //TP2_ASCENCI_KHELIFI_BACKGROUND_H
