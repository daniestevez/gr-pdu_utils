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

#ifndef INCLUDED_PDU_UTILS_PDU_FINE_TIME_MEASURE_H
#define INCLUDED_PDU_UTILS_PDU_FINE_TIME_MEASURE_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief Perform time domain fine burst start time estimate
 * \ingroup pdu_utils
 *
 * Perform time domain fine burst start time estimate.  It considers
 * the pre/post burst times to be noise and the rest of the burst to
 * be signal.  The new start time is when a moving average goes above
 * .5*(Signal + Noise).
 *
 * Pre/post burst time - Extra time pre/appended to a burst, that is
 * presumed to be noise. Average Width - The number of samples used
 * in a moving average to decide the start of burst.
 *
 * Buffer Percent - We don't want to include transitions in our
 * Signal/Noise calculations.  Remove this percent of points from
 * consideration. (0 - 100%)
 *
 */
class PDU_UTILS_API pdu_fine_time_measure : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_fine_time_measure> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_fine_time_measure.
     *
     * @param pre_burst_time -
     * @param post_burst_time -
     * @param average_width -
     * @param buffer_percent -
     */
    static sptr make(float pre_burst_time,
                     float post_burst_time,
                     size_t average_width,
                     float buffer_percent);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_FINE_TIME_MEASURE_H */
