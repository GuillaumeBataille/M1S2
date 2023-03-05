#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Vec3.h"
#include "Ray.h"
#include "Plane.h"

struct RayTriangleIntersection{
    bool intersectionExists;
    float t;
    float w0,w1,w2;
    unsigned int tIndex;
    Vec3 intersection;
    Vec3 normal;
};

class Triangle {
private:
    Vec3 m_c[3] , m_normal;
    float area;
public:
    Triangle() {}
    Triangle( Vec3 const & c0 , Vec3 const & c1 , Vec3 const & c2 ) {
        m_c[0] = c0;
        m_c[1] = c1;
        m_c[2] = c2;
        updateAreaAndNormal();
    }
    void updateAreaAndNormal() {
        Vec3 nNotNormalized = Vec3::cross( m_c[1] - m_c[0] , m_c[2] - m_c[0] );
        float norm = nNotNormalized.length();
        m_normal = nNotNormalized / norm;
        area = norm / 2.f;
    }
    void setC0( Vec3 const & c0 ) { m_c[0] = c0; } // remember to update the area and normal afterwards!
    void setC1( Vec3 const & c1 ) { m_c[1] = c1; } // remember to update the area and normal afterwards!
    void setC2( Vec3 const & c2 ) { m_c[2] = c2; } // remember to update the area and normal afterwards!
    Vec3 const & normal() const { return m_normal; }
    Vec3 projectOnSupportPlane( Vec3 const & p ) const {
        Vec3 result;
        //TODO completer
        return result;
    }
    float squareDistanceToSupportPlane( Vec3 const & p ) const {
        float result;
        //TODO completer
        return result;
    }
    float distanceToSupportPlane( Vec3 const & p ) const { return sqrt( squareDistanceToSupportPlane(p) ); }
    bool isParallelTo( Line const & L ) const {
        bool result;
        //TODO completer
        return result;
    }
    Vec3 getIntersectionPointWithSupportPlane( Line const & L ) const {
        // you should check first that the line is not parallel to the plane!
        Vec3 result;
        //TODO completer
        return result;
    }
    void computeBarycentricCoordinates( Vec3 const & p , float & u0 , float & u1 , float & u2 ) const {
        //TODO Complete
    }

    RayTriangleIntersection getIntersection( Ray const & ray, bool refractMode = false ) const {
        RayTriangleIntersection result;
        result.intersectionExists = false;

        // 1) check that the ray is not parallel to the triangle:
        // 2) check that the triangle is "in front of" the ray:
        if(Vec3::dot(this->m_normal, ray.direction()) == 0){
            return result;
        }

        // Calcul de l'intersection avec le plan
        
        float D = Vec3::dot(m_c[0], this->m_normal);
        float t = (D - Vec3::dot(ray.origin(), this->m_normal)) / Vec3::dot(ray.direction(), this->m_normal);
        Vec3 point = ray.origin() + ray.direction() * t;


        // Projection sur les vecteurs du triangle pour effectuer un changement de repère sur le triangle
        // Calcul des coordonnées barycentriques
        // CONVENTION: compute u,v such that p = w0*c0 + w1*c1 + w2*c2, check that 0 <= w0,w1,w2 <= 1

        Vec3 ab(this->m_c[1] - this->m_c[0]);
        Vec3 ac(this->m_c[2] - this->m_c[0]);
        Vec3 ap(point - this->m_c[0]);

        Vec3 abp_normale = Vec3::cross(ab, ap);
        Vec3 apc_normale = Vec3::cross(ap, ac);

        float abc_aire = Vec3::cross(ab, ac).norm()/2;

        // si les produit scalaire entre 
        float checkU = Vec3::dot(m_normal, apc_normale);
        float checkV = Vec3::dot(m_normal, abp_normale);
        
        double u = (apc_normale.norm()/2)/abc_aire;
        double v = (abp_normale.norm()/2)/abc_aire;
        double w = 1 - (u+v);


        // Vérification si le point est dans le traingle
        if(u > 0 && u <= 1 &&
           v > 0 && v <= 1 &&
           w > 0 && w <= 1 &&
           checkU > 0 && checkV > 0){

            result.intersection = point;
            result.intersectionExists = true;
            if(refractMode){
                result.normal = Vec3(0,0,0) - this->m_normal;
            } else {
                result.normal = this->m_normal;
            }
            result.t = t;
            result.w0 = u;
            result.w1 = v;
            result.w2 = w;
        }

        return result;
    }
};
#endif
