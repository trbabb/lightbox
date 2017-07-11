#include <iostream>
#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>

#include "Image.h"

using namespace geom;
using namespace std;

int main(int argc, char** argv) {
    Image<double,3> img(1024, 512); // a 3-channel image.
    
    for (index_t y = 0; y < img.height; ++y) {
        for (index_t x = 0; x < img.width; ++x) {
            Vec2i xy = Vec2i(x,y);
            Vec2d st = ((Vec2d)(xy)) / img.width;
            st -= Vec2d(0.5, (0.5 * img.height) / img.width);
            double phz = st.x * M_PI * 5;
            img[xy] = Vec3d(sin(phz)/2 + 0.5, cos(phz)/2 + 0.5, cos(phz/2)/2 + 0.5);
        }
    }
    
    save_png(img, "output/test.png");
    
    return 0;
}
