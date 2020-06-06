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

#ifndef INCLUDED_PDU_UTILS_PDU_LOGGER_H
#define INCLUDED_PDU_UTILS_PDU_LOGGER_H

#include <gnuradio/sync_block.h>
#include <pdu_utils/api.h>

namespace gr {
namespace pdu_utils {

/*!
 * \brief PDU data to file
 * \ingroup pdu_utils
 *
 * The PDU Logger block dumps a c32 or f32 data vector from PDUs to file.
 *
 *
 */
class PDU_UTILS_API pdu_logger : virtual public gr::block
{
public:
    typedef boost::shared_ptr<pdu_logger> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of pdu_utils::pdu_logger.
     *
     * @param logfile - Filename to write to
     */
    static sptr make(std::string logfile);
};

} // namespace pdu_utils
} // namespace gr

#endif /* INCLUDED_PDU_UTILS_PDU_LOGGER_H */
