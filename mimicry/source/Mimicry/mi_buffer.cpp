#include "mi_include_buffer.h"

mCBuffer::mCBuffer( MILPCVoid a_pSource, MIUInt a_uSize ) :
    m_uSize( a_uSize ),
    m_pData( a_uSize > sizeof( m_pData ) ? static_cast< MILPByte >( mCStringAllocator::Alloc( a_uSize ) ) : 0 )
{
    g_memcpy( AccessMemory(), a_pSource, m_uSize );
}

mCBuffer::mCBuffer( MIUInt a_uSize, MIByte a_byteValue ) :
    m_uSize( a_uSize ),
    m_pData( a_uSize > sizeof( m_pData ) ? static_cast< MILPByte >( mCStringAllocator::Alloc( a_uSize ) ) : 0 )
{
    g_memset( AccessMemory(), a_byteValue, m_uSize );
}
    
mCBuffer::mCBuffer( mCBuffer const & a_bufSource ) :
    m_uSize( a_bufSource.m_uSize ),
    m_pData( a_bufSource.m_uSize > sizeof( m_pData ) ? static_cast< MILPByte >( mCStringAllocator::Alloc( a_bufSource.m_uSize ) ) : 0 )
{
    g_memcpy( AccessMemory(), a_bufSource.GetMemory(), m_uSize );
}

mCBuffer::mCBuffer( void ) :
    m_uSize( 0 ),
    m_pData( 0 )
{
}

mCBuffer::~mCBuffer( void )
{
    if ( m_uSize > sizeof( m_pData ) )
        mCStringAllocator::Free( m_pData );
}

mCBuffer & mCBuffer::operator = ( mCBuffer const & a_bufSource )
{
    mCBuffer bufCopy( a_bufSource );
    Swap( bufCopy );
    return *this;
}

MIByte const & mCBuffer::operator [] ( MIUInt a_uOffset ) const
{
    return GetMemory()[ a_uOffset ];
}

MIByte & mCBuffer::operator [] ( MIUInt a_uOffset )
{
    return AccessMemory()[ a_uOffset ];
}

MIBool mCBuffer::operator == ( mCBuffer const & a_bufOther ) const
{
    return ( ( m_uSize == a_bufOther.m_uSize ) && ( g_memcmp( GetMemory(), a_bufOther.GetMemory(), m_uSize ) == 0 ) );
}

MIBool mCBuffer::operator != ( mCBuffer const & a_bufOther ) const
{
    return !( *this == a_bufOther );
}

MILPByte mCBuffer::AccessMemory( void )
{
    if ( m_uSize > sizeof( m_pData ) )
        return m_pData;
    return reinterpret_cast< MILPByte >( &m_pData );
}

MILPCByte mCBuffer::GetMemory( void ) const
{
    if ( m_uSize > sizeof( m_pData ) )
        return m_pData;
    return reinterpret_cast< MILPCByte >( &m_pData );
}

MIUInt mCBuffer::GetSize( void ) const
{
    return m_uSize;
}

MILPByte mCBuffer::Resize( MIUInt a_uNewSize )
{
    if ( m_uSize == a_uNewSize )
        return AccessMemory();
    if ( a_uNewSize > sizeof( m_pData ) )
    {
        if ( m_uSize > sizeof( m_pData ) )
            return m_pData = static_cast< MILPByte >( mCStringAllocator::Realloc( m_pData, m_uSize = a_uNewSize ) );
        mCBuffer bufNew( a_uNewSize );
        bufNew.SetAt( 0, &m_pData, m_uSize );
        Swap( bufNew );
        return m_pData;
    }
    mCBuffer bufNew( GetMemory(), a_uNewSize );
    Swap( bufNew );
    return reinterpret_cast< MILPByte >( &m_pData );
}

void mCBuffer::SetAt( MIUInt a_uOffset, MILPVoid a_pSource, MIUInt a_uSize )
{
    a_uOffset = g_min( a_uOffset, a_uSize );
    a_uSize = g_min( a_uSize, m_uSize - a_uOffset );
    g_memcpy( AccessMemory() + a_uOffset, a_pSource, a_uSize );
}

void mCBuffer::SetAt( MIUInt a_uOffset, MIByte a_byteValue, MIUInt a_uSize )
{
    a_uOffset = g_min( a_uOffset, a_uSize );
    a_uSize = g_min( a_uSize, m_uSize - a_uOffset );
    g_memset( AccessMemory() + a_uOffset, a_byteValue, a_uSize );
}

void mCBuffer::Swap( mCBuffer & a_bufOther )
{
    MIByte * pData = m_pData;
    m_pData = a_bufOther.m_pData;
    a_bufOther.m_pData = pData;
    MIUInt uSize = m_uSize;
    m_uSize = a_bufOther.m_uSize;
    a_bufOther.m_uSize = uSize;
}