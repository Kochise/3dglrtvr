
#include "trackball.h"

#include <stdio.h>
#include <math.h>                                               // Needed for sqrtf

//Arcball sphere constants:
//Diameter is       2.0f
//Radius is         1.0f
//Radius squared is 1.0f

void Trackball::_mapToSphere(const Point2fT* NewPt, Vector3fT* NewVec) const
{
    Point2fT TempPt;
    GLfloat length;

    //Copy paramter into temp point
    TempPt = *NewPt;

    //Adjust point coords and scale down to range of [-1 ... 1]
    TempPt.s.X  =        (TempPt.s.X * this->AdjustWidth)  - 1.0f;
    TempPt.s.Y  = 1.0f - (TempPt.s.Y * this->AdjustHeight);

    //Compute the square of the length of the vector to the point from the center
    length      = (TempPt.s.X * TempPt.s.X) + (TempPt.s.Y * TempPt.s.Y);

    //If the point is mapped outside of the sphere... (length > radius squared)
    if (length > 1.0f)
    {
        GLfloat norm;

        //Compute a normalizing factor (radius / sqrt(length))
        norm    = 1.0f / FuncSqrt(length);

        //Return the "normalized" vector, a point on the sphere
        NewVec->s.X = TempPt.s.X * norm;
        NewVec->s.Y = TempPt.s.Y * norm;
        NewVec->s.Z = 0.0f;
    }
    else    //Else it's on the inside
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        NewVec->s.X = TempPt.s.X;
        NewVec->s.Y = TempPt.s.Y;
        NewVec->s.Z = FuncSqrt(1.0f - length);
    }
}

//Create/Destroy
Trackball::Trackball(GLfloat NewWidth, GLfloat NewHeight)
{
    //Clear initial values
    this->StVec.s.X     =
    this->StVec.s.Y     = 
    this->StVec.s.Z     = 

    this->EnVec.s.X     =
    this->EnVec.s.Y     = 
    this->EnVec.s.Z     = 0.0f;

    //Set initial bounds
    this->setBounds(NewWidth, NewHeight);
}

//Mouse down
void    Trackball::click(const Point2fT* NewPt)
{
    //Map the point to the sphere
    this->_mapToSphere(NewPt, &this->StVec);
}

//Mouse drag, calculate rotation
void    Trackball::drag(const Point2fT* NewPt, Quat4fT* NewRot)
{
    //Map the point to the sphere
    this->_mapToSphere(NewPt, &this->EnVec);

//    printf ("%.2f %.2f %.2f\n",StVec.s.X,StVec.s.Y,StVec.s.Z);
 //   printf ("%.2f %.2f %.2f\n",EnVec.s.X,EnVec.s.Y,EnVec.s.Z);

    //Return the quaternion equivalent to the rotation
    if (NewRot)
    {
        Vector3fT  Perp;

        //Compute the vector perpendicular to the begin and end vectors
        Vector3fCross(&Perp, &this->StVec, &this->EnVec);

        //Compute the length of the perpendicular vector
        if (Vector3fLength(&Perp) > Epsilon)    //if its non-zero
        {
            //We're ok, so return the perpendicular vector as the transform after all
            NewRot->s.X = Perp.s.X;
            NewRot->s.Y = Perp.s.Y;
            NewRot->s.Z = Perp.s.Z;
            //In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
            NewRot->s.W= Vector3fDot(&this->StVec, &this->EnVec);
        }
        else                                    //if its zero
        {
            //The begin and end vectors coincide, so return an identity transform
            NewRot->s.X = 
            NewRot->s.Y = 
            NewRot->s.Z = 
            NewRot->s.W = 0.0f;
        }
    }
}
