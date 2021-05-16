#ifndef  CUBE_INC
#define  CUBE_INC

#include "Geometry.h"

#include <string>

using std::string;

class Cube : public Geometry
{
    public:
        Cube();

        void setUvArray(string name);
};

#endif
