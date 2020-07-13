#ifndef COP_CRC_HPP
#define COP_CRC_HPP
/*
* Library: libcrc
* File:    src/crc16.c
* Author:  Lammert Bies
*
* This file is licensed under the MIT License as stated below
*
* Copyright (c) 1999-2016 Lammert Bies
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
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE
* SOFTWARE.
*
* Description
* -----------

* The source file src/crc16.c contains routines which calculate the
* common

* CRC16 cyclic redundancy check values for an incomming byte string.
* modified by Christoph Thurnheer

*/
#include "Error.hpp"
#include <array>
#include <iterator>

namespace cop {

constexpr std::array<uint16_t,256> init_crc16_tab( void ) {
    constexpr uint16_t CRC_POLY_16 = 0xA001;
    std::array<uint16_t, 256> crc_tab16 {};
    for (uint16_t i=0; i<256; i++) {

        uint16_t crc = 0;
        uint16_t c   = i;
        for (uint16_t j=0; j<8; j++) {

            if ( (crc ^ c) & 0x0001 ) {
                crc = static_cast<uint16_t>(( crc >> 1 ) ^ CRC_POLY_16);
            }
            else {
                crc = static_cast<uint16_t>(crc >> 1);
            }
            c = static_cast<uint16_t>(c >> 1);

        }

        crc_tab16[i] = crc;

    }
    return crc_tab16;
}  /* init_crc16_tab */

constexpr std::array<uint16_t, 256> crc_table16 = init_crc16_tab();
static constexpr uint16_t CRC_START = 0x00;

class Crc {
    uint16_t crc_;

    template<class Iterator>
    void calcCrc(Iterator& it, Iterator& end) noexcept {

        while (std::distance(it, end) > 2) {

            crc_ = static_cast<uint16_t>((crc_ >> 8)
                ^ crc_table16[ (crc_ ^ static_cast<uint16_t>(*it++)) & 0x00FF ]);

        }
    }

public:
    Crc() noexcept : crc_(CRC_START)
    {}

    template<class Iterator>
    ProtocolErrc send(Iterator& it, Iterator& end) noexcept {
        if(std::distance(it, end) <= 2) {
            return ProtocolErrc::not_enough_space_in_buffer;
        }
        calcCrc(it, end);

        *it++ = std::byte(crc_ >> 8);
        *it++ = std::byte(crc_ & 0x00FF);
        return ProtocolErrc::success;
    }

    template<class Iterator>
    ProtocolErrc receive(Iterator& it, Iterator& end) noexcept {
        if(it == end) {
            return ProtocolErrc::not_enough_data;
        }
        calcCrc(it, end);
        uint16_t crc = std::to_integer<uint16_t>(*it++);
        crc = static_cast<uint16_t>(crc << 8);
        crc = static_cast<uint16_t>(crc + std::to_integer<uint16_t>(*it++));
        if(crc_  == crc) {
            return ProtocolErrc::success;
        }
        return ProtocolErrc::crc_error;
    }

};
}

#endif // COP_CRC_HPP

