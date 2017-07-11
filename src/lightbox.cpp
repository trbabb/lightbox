#include <iostream>
#include <geomc/linalg/Vec.h>
#include <geomc/linalg/AffineTransform.h>

#include "Image.h"

using namespace geom;
using namespace std;


// Produce a pixel color based on the image coordinates x,y.
// The image center will be (0,0), and x will range from -0.5 to 0.5.
inline Vec3d render_pixel(Vec2d xy) {
    double phz = xy.x * M_PI * 12;
    
    // here's some expression which makes a pixel color value:
    Vec3d px = Vec3d(
        cos(phz)/2 + 0.5,      // red
        cos(phz*1.1)/2 + 0.5,  // green
        cos(phz*1.2)/2 + 0.5); // blue
    
    return px;
}


void render_image(Image<double,3>& img) {
    for (index_t y = 0; y < img.height; ++y) {
        for (index_t x = 0; x < img.width; ++x) {
            Vec2i xy = Vec2i(x,y);
            Vec2d st = ((Vec2d)(xy)) / img.width;
            st -= Vec2d(0.5, (0.5 * img.height) / img.width);
            img[xy] = render_pixel(st);
        }
    }
}


int main(int argc, char** argv) {
    Image<double,3> img(1024, 512); // a 3-channel image.
    
    render_image(img);
    
    img.save_png("output/test.png");
    
    return 0;
}
