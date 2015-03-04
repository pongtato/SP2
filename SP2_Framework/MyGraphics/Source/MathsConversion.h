/****************************************************************************/
/*!
\file MathsConversion.h
\Author SP2 Group 4
\par
\brief
Codes to convert Radians to Degrees and back.
*/
/****************************************************************************/
#pragma once

#define	PI			3.14159265
#define	DEGTORAD	( PI / 180.0 )
#define	RADTODEG	( 180.0 / PI )

// Convert Radians To Degrees
template <class Type>
inline Type RadToDeg(const Type rad)
{
	return static_cast<Type>( rad * RADTODEG );
}

// Convert Degrees To Radians
template <class Type>
inline Type DegToRad(const Type deg)
{
	return static_cast<Type>( deg * DEGTORAD );
}