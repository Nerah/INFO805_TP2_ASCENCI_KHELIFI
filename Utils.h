//
// Created by Utilisateur on 17/02/2020.
//

#ifndef TP2_ASCENCI_KHELIFI_UTILS_H
#define TP2_ASCENCI_KHELIFI_UTILS_H

namespace rt {
    struct RayUtils {
        static Vector3 reflect(const Vector3 &W, Vector3 N) {
            return W - 2 * W.dot(N) * N;
        }
        static Vector3 vRefract(const Ray& ray, Vector3 N, const Material& m) {
            Real tmp,
                 r = m.in_refractive_index / m.out_refractive_index,
                 c = (-1.0f) * N.dot(ray.direction);

            if(ray.direction.dot(N) <= 0) r = 1.0f /r;

            Real par = 1 - ((r * r) * (1 - (c * c)));

            if(c > 0) tmp = r * c - sqrt(par);
            else tmp = r * c + sqrt(par);

            if(par < 0) return reflect(ray.direction,N);
            return Vector3(r*ray.direction + tmp * N);
        }
    };
    struct MathUtils {
        static float randFloat(float a, float b){
            return ((b - a ) * ((float) rand() / RAND_MAX)) + a;
        }
    };
}
#endif //TP2_ASCENCI_KHELIFI_UTILS_H
