/*
 * Image.h
 * Author: tbabb
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <png.h>
#include <cstdio>
#include <math.h>
#include <stdexcept>
 
#include <geomc/linalg/Vec.h>


using namespace geom;


// forward declaration:
template <typename T, index_t Channels>
class Image;
 

// clip `val` to lie between `lo` and `hi`.
template <typename T>
inline T clamp(T val, T lo, T hi) {
    return std::max(std::min(val, hi), lo);
}


// get the raw data bytes to write to file.
// converts each pixel to an integer and orders the channels and bytes the correct way.
template <typename T, index_t Channels>
png_bytep* png_data(
        const Image<T,Channels> &img, 
        typename PointType<T,Channels>::point_t white, 
        bool sixteenbit) {
    typedef Image<T,Channels> img_t;
    
    index_t nchannels = img.channels();
    index_t w = img.dimensions()[0];
    index_t h = img.dimensions()[1];
    png_bytep *rows = new png_bytep[h];
    
    // foreach row:
    for (index_t y = 0; y < h; y++){
        // generate a row of pixels
        rows[y] = new png_byte[w * nchannels * (1 + sixteenbit)]; 
        
        // foreach pixel:
        for (index_t x = 0; x < w; x++){
            Vec2i icoords = Vec2i(x, h - y - 1);
            typename img_t::pixel pixel = img[icoords];
            
            //foreach channel:
            for (index_t c = 0; c < nchannels; c++){
                T wht = PointType<T,Channels>::iterator(white)[c];
                T val = PointType<T,Channels>::iterator(pixel)[c];
                val = clamp(val, (T)0, wht);
                if (sixteenbit){
                    int ival = (val * 0xffff) / wht;
                    png_byte hi = (ival & 0xff00) >> 8;
                    png_byte lo =  ival & 0x00ff;
                    //MSB first:
                    rows[y][(x * nchannels + c) * 2]     = hi;
                    rows[y][(x * nchannels + c) * 2 + 1] = lo;
                } else {
                    png_byte b = (val * 0xff) / wht;
                    rows[y][x*nchannels + c] = b;
                }
            }
        }
    }
    
    return rows;
}


// class holding a 2D image, with channel values of type T.
template <typename T, index_t Channels>
class Image {
    public:
    
    // defines a type which is Vec<T,Channels>, unless Channels == 1 (then it's simply a T).
    typedef typename PointType<T,Channels>::point_t pixel;
    
    pixel*  data;
    index_t width;
    index_t height;
    
    
    // constructor
    Image(index_t w, index_t h):
            data(new pixel[w * h]),
            width(w),
            height(h) {}
    
    
    // destructor
    ~Image() {
        delete [] data;
        data   = NULL;
        width  = 0;
        height = 0;
    }
    
    
    // get the pixel at the given coordinates
    inline pixel& operator[](Vec2i coords) {
        return data[coords.y * width + coords.x];
    }
    
    
    inline pixel operator[](Vec2i coords) const {
        return data[coords.y * width + coords.x];
    }
    
    
    // get the width and height of the image
    inline Vec2i dimensions() const {
        return Vec2i(width,height);
    }
    
    
    // get the number of channels in the image
    inline index_t channels() const {
        return Channels;
    }
    
    
    // save this image to PNG file.
    inline bool save_png(
            const char* filepath,          // Where to save the image
            double exposure=0,             // Exposure parameter, in camera stops. Negative = darker; positive = brighter
            bool sixteenbit=false) const { // Whether to save with 16-bit pixel data.
        int imgtype;
        switch (Channels){
            case 1:
                imgtype = PNG_COLOR_TYPE_GRAY;
                break;
            case 2:
                imgtype = PNG_COLOR_TYPE_GRAY_ALPHA;
                break;
            case 3:
                imgtype = PNG_COLOR_TYPE_RGB;
                break;
            case 4:
                imgtype = PNG_COLOR_TYPE_RGB_ALPHA;
                break;
            default:
                return false;
        }
        
        // white point
        T scale = std::pow(2, exposure);
        typename PointType<T,Channels>::point_t white = typename PointType<T,Channels>::point_t(scale);
        
        // get raw pixel data (bytes) for writing
        png_bytep *imgdat = png_data<T,Channels>(*this, white, sixteenbit);
        
        // actually do the write.
        FILE *fp = fopen(filepath, "wb");
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) throw std::runtime_error("could not make png write structure");
        png_infop  info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) throw std::runtime_error("could not make png info structure");
        if (setjmp(png_jmpbuf(png_ptr))) throw std::runtime_error("png write init failure");
        png_init_io(png_ptr, fp);
        if (setjmp(png_jmpbuf(png_ptr))) throw std::runtime_error("png write header failure");
        png_set_IHDR(png_ptr, info_ptr, width, height, sixteenbit?16:8, imgtype, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        //png_set_gAMA(png_ptr, info_ptr, 1/2.2);
        png_set_sRGB(png_ptr, info_ptr, PNG_sRGB_INTENT_ABSOLUTE);
        png_write_info(png_ptr, info_ptr);
        if (setjmp(png_jmpbuf(png_ptr))) throw std::runtime_error("png write image failure");
        png_write_image(png_ptr, imgdat);
        if (setjmp(png_jmpbuf(png_ptr))) throw std::runtime_error("png write completion failure");
        png_write_end(png_ptr, NULL);
        fclose(fp);
        
        return true;
    }
    
};


#endif /* IMAGE_H_ */
