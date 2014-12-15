/* 
 * File:   OwnCheckerboard.h
 * Author: student
 *
 * Created on September 23, 2014, 5:58 PM
 */

#ifndef OWNCHECKERBOARD_H
#define	OWNCHECKERBOARD_H

#include "Globals.h"

#include "OwnObject.h"
#include <math.h>

class OwnCheckerboard : public OwnObject {
public:
    OwnCheckerboard();
    OwnCheckerboard(const OwnCheckerboard& orig);
    virtual ~OwnCheckerboard();
    void draw();
    
private:

};

#endif	/* OWNCHECKERBOARD_H */

