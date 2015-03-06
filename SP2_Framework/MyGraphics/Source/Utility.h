/****************************************************************************/
/*!
\file Utility.h
\Author SP2 Group 4
\par
\brief
Header file for Mtx44 to change positions
*/
/****************************************************************************/
#ifndef _UTILITY_H
#define _UTILITY_H

#include "Vertex.h"
#include "Mtx44.h"

Position operator*(const Mtx44& lhs, Position rhs);

#endif