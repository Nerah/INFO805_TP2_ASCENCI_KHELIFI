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
    };
    struct MathUtils {
        static float randFloat(float a, float b){
            return ((b - a ) * ((float) rand() / RAND_MAX)) + a;
        }
    };
}
#endif //TP2_ASCENCI_KHELIFI_UTILS_H
