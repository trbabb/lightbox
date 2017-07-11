/*
 * ShapeTypes.h
 *
 *  Created on: Nov 11, 2010
 *      Author: tbabb
 */

#ifndef SHAPETYPES_H_
#define SHAPETYPES_H_

#include <boost/utility/enable_if.hpp>
#include <geomc/linalg/LinalgTypes.h>

/** @defgroup shape Shape
 *  @brief Shape-related functions and classes.
 */

namespace geom {
    
    /** @addtogroup shape 
     *  @{
     */


    /** 
     * @brief Array traversal order, specified in terms of which axes to increment first.
     */
    enum ArrayOrder {
        /**
         * @brief Array traversal by incrementing the first coordinate in the 
         * innermost loop. 
         * 
         * For matrices, whose coordinates are ordered `(row, col)`, this 
         * represens column-major (i.e. "fortran") order. If coordinates
         * are `(x, y, ...)`, then this is row-major order.
         */
        ARRAYORDER_FIRST_DIM_CONSECUTIVE,
        /**
         * @brief Array traversal by incrementing the last coordinate in the
         * innermost loop.
         * 
         * For matrices, whose coordinates are ordered `(row, col)`, this
         * represents row-major (i.e. "C") order. If coordinates are `(x, y, ...)`,
         * then this is column-major order.
         */
        ARRAYORDER_LAST_DIM_CONSECUTIVE
    };
    
    template <typename T, index_t N> class Rect;
    template <typename T, index_t N, ArrayOrder Order=ARRAYORDER_FIRST_DIM_CONSECUTIVE> class GridIterator;

    typedef Rect<double,1> Ranged;
    typedef Rect<double,1> Rect1d;
    typedef Rect<double,2> Rect2d;
    typedef Rect<double,3> Rect3d;
    typedef Rect<double,4> Rect4d;

    typedef Rect<index_t,1> Rangei;
    typedef Rect<index_t,1> Rect1i;
    typedef Rect<index_t,2> Rect2i;
    typedef Rect<index_t,3> Rect3i;
    typedef Rect<index_t,4> Rect4i;

    typedef Rect<float,1> Rangef;
    typedef Rect<float,1> Rect1f;
    typedef Rect<float,2> Rect2f;
    typedef Rect<float,3> Rect3f;
    typedef Rect<float,4> Rect4f;

/// @} // addtogroup shape
    
} // namespace geom

#endif /* SHAPETYPES_H_ */
