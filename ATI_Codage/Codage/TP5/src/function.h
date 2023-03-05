#ifndef FUNCTION_H
#define FUNCTION_H

#include <unordered_map>
#include "map"
#include "Mesh.h"
#include "Writer.h"


Vec3 dracoQuantisizeCoord(Vec3 coord, Vec3 bbmin, int qp, float range)
{
    Vec3 v = (coord - bbmin) * (pow(2, qp)/range);

    v[0] = round(v[0]);
    v[1] = round(v[1]);
    v[2] = round(v[2]);

    return v;
}
Vec3 dracoDequantisizeCoord(Vec3 coord, Vec3 bbmin, int qp, float range)
{
    return ((coord * range) / pow(2,qp)) + bbmin;
}

void dracoQuantisize(Mesh &meshIn, Mesh &meshOut, int qp)
{
    float range = meshIn.BB_Max[0] - meshIn.BB_Min[0];
    if(meshIn.BB_Max[1] - meshIn.BB_Min[1] > range)
        range = meshIn.BB_Max[1] - meshIn.BB_Min[1];
    if(meshIn.BB_Max[2] - meshIn.BB_Min[2] > range)
        range = meshIn.BB_Max[2] - meshIn.BB_Min[2];

    for(int i = 0; i < meshIn.vertices.size(); i++)
    {
        meshOut.vertices[i].position = dracoQuantisizeCoord(meshIn.vertices[i].position, meshIn.BB_Min, qp, range);
    }
}

void dracoDequantisize(Mesh &mesh, int qp)
{
    float range = mesh.BB_Max[0] - mesh.BB_Min[0];
    if(mesh.BB_Max[1] - mesh.BB_Min[1] > range)
        range = mesh.BB_Max[1] - mesh.BB_Min[1];
    if(mesh.BB_Max[2] - mesh.BB_Min[2] > range)
        range = mesh.BB_Max[2] - mesh.BB_Min[2];

    for(int i = 0; i < mesh.vertices.size(); i++)
    {
        mesh.vertices[i].position = dracoDequantisizeCoord(mesh.vertices[i].position, mesh.BB_Min, qp, range);
    }
}

float rmse(Mesh m1, Mesh m2)
{
    float yes = 0;

    for(int i = 0; i < m1.vertices.size(); i++)
    {
        yes += pow(m1.vertices[i].position[0] - m2.vertices[i].position[0], 2);
        yes += pow(m1.vertices[i].position[1] - m2.vertices[i].position[1], 2);
        yes += pow(m1.vertices[i].position[2] - m2.vertices[i].position[2], 2);
    }

    yes = sqrt( yes/(3*m1.vertices.size()) );

    return yes;
}
template<typename T>
int findInVector(std::vector<T> vec, T el)
{
    for(int i = 0; i < vec.size(); i++)
    {
        if(vec[i] == el)
            return i;
    }
    return -1;
}

void rANS_buildArrays(std::vector<int> &sequence, std::vector<int> &alpha, std::vector<int> &alpha_freq, std::vector<int> &freqCumul, int &M)
{
    M = 0;

    for(int c : sequence)
    {
        int index = findInVector(alpha, c);
        if( index == -1)
        {
            alpha.push_back(c);
            alpha_freq.push_back(1);
        } else {
            alpha_freq[index]++;
        }

    }

    freqCumul.push_back(0);
    for(int i = 0; i < alpha_freq.size()-1; i++)
    {
        freqCumul.push_back(alpha_freq[i] + freqCumul[i]);
    }

    for(int el : alpha_freq){
        M += el;
    }
}

unsigned long int rANS_encode(std::vector<int> &sequence, std::vector<int> &alpha, std::vector<int> &alpha_freq, std::vector<int> &freqCumul, int &M)
{
    unsigned long int x = 0;

    for(int c : sequence)
    {
        int alpha_index = findInVector(alpha, c);
        x = floor(x/alpha_freq[alpha_index]) * M + freqCumul[alpha_index] + x % alpha_freq[alpha_index];
    }

    return x;
}

std::vector<int> rANS_decode(unsigned long long int encodedInt, std::vector<int> &alpha, std::vector<int> &alpha_freq, std::vector<int> &freqCumul, int &M)
{
    int n, st;
    std::vector<int> sequence;

    while (encodedInt != 0)
    {
        n = encodedInt % M;


        int alpha_index = findInVector(freqCumul, n);
        if(alpha_index != -1){
            st = alpha[alpha_index];
        } else {
            st = 0;
        }

        encodedInt = floor(encodedInt/M) * alpha_freq[st] + n - freqCumul[st];

        sequence.insert(sequence.begin(), st);
    }

    n = encodedInt % M;
    int alpha_index = findInVector(freqCumul, n);
    if(alpha_index != -1){
        st = alpha[alpha_index];
    } else {
        st = 0;
    }
    encodedInt = floor(encodedInt/M) * alpha_freq[st] + n - freqCumul[st];
    sequence.insert(sequence.begin(), st);

    return sequence;
}

#endif
