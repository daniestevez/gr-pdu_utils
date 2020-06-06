/* -*- c++ -*- */
/*
 * Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
 * Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains
 * certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_PDU_UTILS_UPSAMPLE_H
#define INCLUDED_PDU_UTILS_UPSAMPLE_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief upsample PDUs
 * \ingroup pdu_utils
 *
 * This block will upsample via zero insertion or repetition.
 *
 */
class PDU_UTILS_API upsample : virtual public gr::block
{
public:
    typedef boost::shared_ptr<upsample> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::upsample.
     *
     * @param n - upsample by factor n
     * @param repeat - true to repeat values, false to zero fill
     */
    static sptr make(uint32_t n, bool repeat);

    /**
     * Set upsample factor
     *
     * @param n
     */
    virtual void set_n(uint32_t n) = 0;

    /**
     * Set repeat behavior
     *
     * @param repeat = true to report values, false to zero fill
     */
    virtual void set_repeat(bool repeat) = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_UPSAMPLE_H */
