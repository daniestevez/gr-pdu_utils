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

#ifndef INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_H
#define INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_H

#include <gnuradio/block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Will randomly drop a message with specified probability.
 * \ingroup pdu_utils
 *
 */
class PDU_UTILS_API msg_drop_random : virtual public gr::block
{
public:
    typedef boost::shared_ptr<msg_drop_random> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::msg_drop_random.
     *
     * @param p_drop - percentage to drop, [0..1]
     * @param seed - RNG seed value
     */
    static sptr make(float p_drop, uint64_t seed = 12345678);

    /**
     * Set percentage of dropped messages
     *
     * @param p - percentage drop, [0..1]
     */
    virtual void set_prob_drop(float p) = 0;

    /**
     * Resets counters
     */
    virtual void reset(void) = 0;

    /**
     * Returns count of dropped messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_drop_count() = 0;

    /**
     * Returns count of incoming messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_msg_count() = 0;

    /**
     * Returns count of passed messages
     *
     * @return uint64_t
     */
    virtual uint64_t get_pass_count() = 0;
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_MSG_DROP_RANDOM_H */
