#include "mi_include_3d.h"
#include "mi_include_standard.h"

mCQuaternion::mCQuaternion( MIFloat a_fX, MIFloat a_fY, MIFloat a_fZ, MIFloat a_fW ) :
    m_fX( a_fX ),
    m_fY( a_fY ),
    m_fZ( a_fZ ),
    m_fW( a_fW )
{
}

mCQuaternion::mCQuaternion( mCMatrix4 const & a_matSource )
{
    MIFloat const * pMatrixElements = a_matSource.GetElements();
    MIFloat fTrace = pMatrixElements[ 0 ] + pMatrixElements[ 5 ] + pMatrixElements[ 10 ] + 1.0f;
    if ( fTrace > 0.0f )
    {
        MIFloat fTemp = 0.5f / sqrt( fTrace );
        m_fX = ( pMatrixElements[ 9 ] - pMatrixElements[ 6 ] ) * fTemp;
        m_fY = ( pMatrixElements[ 2 ] - pMatrixElements[ 8 ] ) * fTemp;
        m_fZ = ( pMatrixElements[ 4 ] - pMatrixElements[ 1 ] ) * fTemp;
        m_fW = 0.25f / fTemp;
        return;
    }
    MIBool const bYGreaterThanZ = pMatrixElements[ 5 ] > pMatrixElements[ 10 ];
    MIBool const bXGreatest = bYGreaterThanZ ? ( pMatrixElements[ 0 ] > pMatrixElements[ 5 ] ) : ( pMatrixElements[ 0 ] > pMatrixElements[ 10 ] );
    if ( bXGreatest )
    {
        MIFloat fTemp  = sqrt( 1.0f + pMatrixElements[ 0 ] - pMatrixElements[ 5 ] - pMatrixElements[ 10 ] ) * 2;
        m_fX = 0.5f * fTemp;
        m_fY = ( pMatrixElements[ 1 ] + pMatrixElements[ 4 ] ) / fTemp;
        m_fZ = ( pMatrixElements[ 2 ] + pMatrixElements[ 8 ] ) / fTemp;
        m_fW = ( pMatrixElements[ 9 ] - pMatrixElements[ 6 ] ) / fTemp;
    }
    else if ( bYGreaterThanZ )
    {
        MIFloat fTemp  = sqrt( 1.0f + pMatrixElements[ 5 ] - pMatrixElements[ 0 ] - pMatrixElements[ 10 ] ) * 2;
        m_fX = ( pMatrixElements[ 1 ] + pMatrixElements[ 4 ] ) / fTemp;
        m_fY = 0.5f * fTemp;
        m_fZ = ( pMatrixElements[ 6 ] + pMatrixElements[ 9 ] ) / fTemp;
        m_fW = ( pMatrixElements[ 2 ] - pMatrixElements[ 8 ] ) / fTemp;
    }
    else
    {
        MIFloat fTemp  = sqrt( 1.0f + pMatrixElements[ 10 ] - pMatrixElements[ 0 ] - pMatrixElements[ 5 ] ) * 2;
        m_fX = ( pMatrixElements[ 2 ] + pMatrixElements[ 8 ] ) / fTemp;
        m_fY = ( pMatrixElements[ 6 ] + pMatrixElements[ 9 ] ) / fTemp;
        m_fZ = 0.5f * fTemp;
        m_fW = ( pMatrixElements[ 4 ] - pMatrixElements[ 1 ] ) / fTemp;
    }
}

mCQuaternion::mCQuaternion( mCQuaternion const & a_quatSource ) :
    m_fX( a_quatSource.m_fX ),
    m_fY( a_quatSource.m_fY ),
    m_fZ( a_quatSource.m_fZ ),
    m_fW( a_quatSource.m_fW )
{
}

mCQuaternion::mCQuaternion( void ) :
    m_fX( 0.0f ),
    m_fY( 0.0f ),
    m_fZ( 0.0f ),
    m_fW( 1.0f )
{
}

mCQuaternion::~mCQuaternion( void )
{
}

mCQuaternion & mCQuaternion::operator = ( mCQuaternion const & a_quatSource )
{
    if ( this == &a_quatSource )
        return *this;
    m_fX = a_quatSource.m_fX;
    m_fY = a_quatSource.m_fY;
    m_fZ = a_quatSource.m_fZ;
    m_fW = a_quatSource.m_fW;
    return *this;
}

MIFloat & mCQuaternion::operator [] ( MIUInt a_uCoordIndex )
{
    return m_arrCoordinates[ a_uCoordIndex ];
}

MIFloat const & mCQuaternion::operator [] ( MIUInt a_uCoordIndex ) const
{
    return m_arrCoordinates[ a_uCoordIndex ];
}

MIFloat & mCQuaternion::AccessX( void )
{
    return m_fX;
}

MIFloat & mCQuaternion::AccessY( void )
{
    return m_fY;
}

MIFloat & mCQuaternion::AccessZ( void )
{
    return m_fZ;
}

MIFloat & mCQuaternion::AccessW( void )
{
    return m_fW;
}

void mCQuaternion::Clear( void )
{
    m_fX = m_fY = m_fZ = 0.0f;
    m_fW = 1.0f;
}

MIFloat mCQuaternion::GetX( void ) const
{
    return m_fX;
}

MIFloat mCQuaternion::GetY( void ) const
{
    return m_fY;
}

MIFloat mCQuaternion::GetZ( void ) const
{
    return m_fZ;
}

MIFloat mCQuaternion::GetW( void ) const
{
    return m_fW;
}

mCQuaternion & mCQuaternion::Inverse( void )
{
    m_fX *= -1.0f;
    m_fY *= -1.0f;
    m_fZ *= -1.0f;
    return *this;
}

mCQuaternion & mCQuaternion::Normalize( void )
{
    MIFloat fLength = sqrt( m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ + m_fW * m_fW );
    m_fX /= fLength;
    m_fY /= fLength;
    m_fZ /= fLength;
    m_fW /= fLength;
    return *this;
}

void mCQuaternion::Swap( mCQuaternion & a_quatOther )
{
    if ( this == &a_quatOther )
        return;
    g_memswap( *this, a_quatOther );
}

mCIStreamBinary & operator >> ( mCIStreamBinary & a_streamSource, mCQuaternion & a_quatDest )
{
    a_streamSource >> a_quatDest.AccessX();
    a_streamSource >> a_quatDest.AccessY();
    a_streamSource >> a_quatDest.AccessZ();
    a_streamSource >> a_quatDest.AccessW();
    return a_streamSource;
}

mCIStreamBinary & operator << ( mCQuaternion & a_quatDest, mCIStreamBinary & a_streamSource )
{
    return ( a_streamSource >> a_quatDest );
}

mCOStreamBinary & operator << ( mCOStreamBinary & a_streamDest, mCQuaternion const & a_quatSource )
{
    a_streamDest << a_quatSource.GetX();
    a_streamDest << a_quatSource.GetY();
    a_streamDest << a_quatSource.GetZ();
    a_streamDest << a_quatSource.GetW();
    return a_streamDest;
}

mCOStreamBinary & operator >> ( mCQuaternion const & a_quatSource, mCOStreamBinary & a_streamDest )
{
    return ( a_streamDest << a_quatSource );
}
