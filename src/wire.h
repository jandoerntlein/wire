/******************************************************************************/
/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Jan Doerntlein
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/******************************************************************************/
/**
 *  wire v0.0.1 
 *  A minimalistic signal-slot pattern implementation in C++14.
 *
 *  Use
 *      #define WIRE_USE_EXCEPTIONS
 *  before including wire.h to enable exceptions.
 *
 *  Note: 
 *  - this implementation has not been tested extensively yet
 *  - slots can only return void
 *  - slots must be member functions (may be enhanced in the future)
 *
 */
/******************************************************************************/
#ifndef _WIRE_H
#define _WIRE_H


/******************************************************************************/
#include <functional>
#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_map>


/******************************************************************************/
template<int> struct placeholderpack{};

template<int I>
struct std::is_placeholder<placeholderpack<I>> 
    : std::integral_constant<int, I+1>{};

namespace wire
{
/******************************************************************************/
/**
 *  Standard Wire object for many-to-many relationships. A wire can be 
 *  connected to an unlimited number for member objects which are called in the
 *  same order as they were connected. 
 *
 */
template<typename... Args>
class Wire
{
public:
    template<typename K>
    inline void connect( void ( K::* pMemberFunc )( Args... args ), 
        K* pInstance ) 
    {
        connect_helper<K>( std::make_integer_sequence<int, sizeof...( Args )>{}, 
            pMemberFunc, 
            pInstance );
    }

    inline void disconnect()
    {
        m_fn.clear();
    }

    inline void operator()( Args... params ) const
    {
        for( const auto& p : m_fn ) p( params... );
    }
protected:
    std::vector<std::function<void( Args... )>> m_fn;

private:
    template<typename K, int... Idcs>
    inline void connect_helper( std::integer_sequence<int, Idcs...>, 
        void ( K::* pMemberFunc )( Args... args ), 
        K* pInstance ) 
    {
        std::function<void( Args... )> f = std::bind( pMemberFunc, 
            pInstance, 
            placeholderpack<Idcs>{}... );
        m_fn.emplace_back( f );
    }
};


/******************************************************************************/
/**
 *  The DirectWire class uses unordered_map instead of vector to connect each
 *  wire to an index. For each connection you have to pass an index to 
 *  connect( idx, ... ) to glue the target member function to that index. To
 *  call this function use the index as argument. 
 *  This is for performance reasons if you do not want to traverse the whole
 *  vector because you already know which target function should be called.
 *
 */
template<typename... Args>
class DirectWire
{
public:
    template<typename K>
    inline void connect( unsigned int idx, 
        void ( K::* pMemberFunc )( Args... args ), 
        K* pInstance ) 
    {
        if( m_fn.count( idx ) == 0 )
            connect_helper<K>( std::make_integer_sequence<int, sizeof...( Args )>{}, 
                idx, 
                pMemberFunc, 
                pInstance );
#ifdef WIRE_USE_EXCEPTIONS
        else
            throw std::runtime_error( "Requested index already in use." );
#endif
    }

    inline void disconnect( unsigned int idx )
    {
        if( m_fn.count( idx ) > 0 )
            m_fn.erase( idx );      
#ifdef WIRE_USE_EXCEPTIONS
        else
            throw std::runtime_error( "Cannot delete unused key." );
#endif
    }

    inline void operator()( unsigned int idx, Args... params ) const
    {
        if( m_fn.count( idx ) > 0 ) 
            m_fn.at( idx )( params... );
#ifdef WIRE_USE_EXCEPTIONS
        else
            throw std::runtime_error( "Function index is not in use." );
#endif
    }
protected:
    std::unordered_map<unsigned int, std::function<void( Args... )>> m_fn;

private:
    template<typename K, int... Idcs>
    inline void connect_helper( std::integer_sequence<int, Idcs...>,  
        unsigned int idx, 
        void ( K::* pMemberFunc )( Args... args ), 
        K* pInstance ) 
    {
        std::function<void( Args... )> f = std::bind( pMemberFunc, 
            pInstance, 
            placeholderpack<Idcs>{}... );
        m_fn.emplace( idx, f );
    }
};


/******************************************************************************/
}
#endif // _WIRE_H
