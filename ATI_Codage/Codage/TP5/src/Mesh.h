#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include "Vec3.h"

#include <cfloat>

#include <string.h>


// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

struct MeshVertex {
    inline MeshVertex () {}
    inline MeshVertex (const Vec3 & _p, const Vec3 & _n) : position (_p), normal (_n) , u(0) , v(0) {}
    inline MeshVertex (const MeshVertex & vertex) : position (vertex.position), normal (vertex.normal) , u(vertex.u) , v(vertex.v) {}
    inline virtual ~MeshVertex () {}
    inline MeshVertex & operator = (const MeshVertex & vertex) {
        position = vertex.position;
        normal = vertex.normal;
        u = vertex.u;
        v = vertex.v;
        return (*this);
    }
    // membres :
    Vec3 position; // une position
    Vec3 normal; // une normale
    float u,v; // coordonnees uv
};

struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};




class Mesh {
protected:
    void build_positions_array() {
        positions_array.resize( 3 * vertices.size() );
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            positions_array[3*v + 0] = vertices[v].position[0];
            positions_array[3*v + 1] = vertices[v].position[1];
            positions_array[3*v + 2] = vertices[v].position[2];
        }
    }
    void build_triangles_array() {
        triangles_array.resize( 3 * triangles.size() );
        for( unsigned int t = 0 ; t < triangles.size() ; ++t ) {
            triangles_array[3*t + 0] = triangles[t].v[0];
            triangles_array[3*t + 1] = triangles[t].v[1];
            triangles_array[3*t + 2] = triangles[t].v[2];
        }
    }
public:
    std::string name;
    int id;

    Vec3 BB_Min;
    Vec3 BB_Max;


    std::vector<MeshVertex> vertices;
    std::vector<MeshTriangle> triangles;

    std::vector< float > positions_array;
    std::vector< unsigned int > triangles_array;



    void loadOFF (const std::string & filename);
    void writeOFF (const std::string & filename);
    void recomputeNormals ();
    void centerAndScaleToUnit ();
    void scaleUnit ();


    virtual
    void build_arrays() {
        recomputeNormals();
        build_positions_array();
        build_triangles_array();
    }

    void calculateBoundingBox(){
        Vec3 box_min;
        Vec3 box_max;
        box_min = box_max = vertices[0].position;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (unsigned char p = 0; p < 3; p++)
            {
                if (vertices[i].position[p] < box_min[p])
                    box_min[p] = vertices[i].position[p];
                if (vertices[i].position[p] > box_max[p])
                    box_max[p] = vertices[i].position[p];
            }
        }

        BB_Min = box_min;
        BB_Max = box_max;
    }

    void translate( Vec3 const & translation ){
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            vertices[v].position += translation;
        }
    }

    void apply_transformation_matrix( Mat3 transform ){
        for( unsigned int v = 0 ; v < vertices.size() ; ++v ) {
            vertices[v].position = transform*vertices[v].position;
            vertices[v].normal = transform*vertices[v].normal;
        }

        //recomputeNormals();
        //build_positions_array();
        //build_normals_array();
    }

    void scale( Vec3 const & scale ){
        Mat3 scale_matrix(scale[0], 0., 0.,
                0., scale[1], 0.,
                0., 0., scale[2]); //Matrice de transformation de mise à l'échelle
        apply_transformation_matrix( scale_matrix );
    }

    void rotate_x ( float angle ){
        float x_angle = angle * M_PI / 180.;
        Mat3 x_rotation(1., 0., 0.,
                        0., cos(x_angle), -sin(x_angle),
                        0., sin(x_angle), cos(x_angle));
        apply_transformation_matrix( x_rotation );
    }

    void rotate_y ( float angle ){
        float y_angle = angle * M_PI / 180.;
        Mat3 y_rotation(cos(y_angle), 0., sin(y_angle),
                        0., 1., 0.,
                        -sin(y_angle), 0., cos(y_angle));
        apply_transformation_matrix( y_rotation );
    }

    void rotate_z ( float angle ){
        float z_angle = angle * M_PI / 180.;
        Mat3 z_rotation(cos(z_angle), -sin(z_angle), 0.,
                        sin(z_angle), cos(z_angle), 0.,
                        0., 0., 1.);
        apply_transformation_matrix( z_rotation );
    }

    Mesh operator = (Mesh &m)
    {

        for(MeshVertex v : m.vertices)
        {
            vertices.push_back(v);
        }
        for(MeshTriangle t : m.triangles)
        {
            triangles.push_back(t);
        }

        BB_Min = m.BB_Min;
        BB_Max = m.BB_Max;

        return *this;
    }
};

#endif
