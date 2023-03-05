#include "function.h"
int main()
{
   /// EX 1 2
    Mesh mesh = Mesh();
    mesh.loadOFF("../bunny.off");
    mesh.calculateBoundingBox();

    Mesh meshC = Mesh();
    meshC = mesh;

    Writer writer("../dat/rmse.dat");

    for(int qp = 5; qp <= 30; qp = qp + 1)
    {
        quantize(mesh, meshC, qp);
        dequantize(meshC ,qp);

        std::string s;
        s.append("../compressedOFF/bunnyC").append(std::to_string(qp)).append(".off");
        meshC.writeOFF(s);
        //ecriture
        writer.write(std::to_string(qp) + " " + std::to_string(computeRMSE(mesh, meshC)));
    }
}