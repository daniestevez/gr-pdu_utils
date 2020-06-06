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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "take_skip_to_pdu_impl.h"
#include <gnuradio/io_signature.h>
//#include <pmt/pmt.h>

namespace gr {
namespace pdu_utils {


template <class T>
typename take_skip_to_pdu<T>::sptr take_skip_to_pdu<T>::make(uint32_t take, uint32_t skip)
{
    return gnuradio::get_initial_sptr(new take_skip_to_pdu_impl<T>(take, skip));
}

/*
 * The private constructor
 */
template <class T>
take_skip_to_pdu_impl<T>::take_skip_to_pdu_impl(uint32_t take, uint32_t skip)
    : gr::sync_block("take_skip_to_pdu",
                     gr::io_signature::make(1, 1, sizeof(T)),
                     gr::io_signature::make(0, 0, 0)),
      d_take(take),
      d_skip(skip),
      d_next(take),
      d_triggered(true),
      d_burst_counter(0),
      d_prev_byte(0)
{
    if (d_take == 0) {
        GR_LOG_FATAL(this->d_logger, "TAKE value too small, must be > 0");
        throw std::invalid_argument("TAKE value out of bounds");
    }
    if (d_take > TAKESKIP_MAXIMUM_PDU_SIZE) {
        GR_LOG_FATAL(this->d_logger,
                     boost::format("TAKE value too large, must be less than %d") %
                         TAKESKIP_MAXIMUM_PDU_SIZE);
        throw std::invalid_argument("TAKE value out of bounds");
    }

    d_vector.clear();
    d_meta_dict = pmt::make_dict();
    this->message_port_register_out(PMTCONSTSTR__PDU_OUT);

    GR_LOG_INFO(this->d_logger, "Starting Take Skip PDU Generator!");
}


/*
 * Our virtual destructor.
 */
template <class T>
take_skip_to_pdu_impl<T>::~take_skip_to_pdu_impl()
{
}


template <class T>
void take_skip_to_pdu_impl<T>::publish_message()
{
    d_meta_dict = pmt::dict_add(
        d_meta_dict, pmt::intern("pdu_num"), pmt::from_uint64(d_burst_counter));
    // std::cout << "CPP: sending burst number " << d_burst_counter << " of length " <<
    // d_vector.size() << std::endl;

    // publish mesage
    this->message_port_pub(PMTCONSTSTR__PDU_OUT,
                           pmt::cons(d_meta_dict, this->init_data(d_vector)));

    // prepare for next burst
    d_burst_counter++;
    d_triggered = false;
    d_vector.clear();
}


template <class T>
int take_skip_to_pdu_impl<T>::work(int noutput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    gr::thread::scoped_lock l(this->d_setlock);

    const T* in = (const T*)input_items[0];
    uint64_t consumed = noutput_items;
    bool start_stop = false;

    // figure out where the next start/stop point is
    if (d_next <= (this->nitems_read(0) + noutput_items)) {
        // we are going to start/stop during this buffer
        consumed = d_next - this->nitems_read(0);
        start_stop = true;
        if (d_triggered) {
            d_next += d_skip;
        } else {
            d_next += d_take;
        }
    }

    if (d_triggered) {
        // we are saving data
        // std::cout << "    adding  " << consumed << " items to the queue" << std::endl;
        d_vector.insert(d_vector.end(), &in[0], &in[consumed]);
        if (start_stop) {
            // std::cout << "    publishing message with " << d_vector.size() << " items"
            // << std::endl;
            publish_message();
            if (d_skip == 0) {
                d_triggered = true;
                d_next += d_take;
            }
        }
    } else {
        // if we are not saving data but need to start next time:
        if (start_stop)
            d_triggered = true;
    }

    // Tell runtime system how many output items we produced.
    return consumed;
}

template <class T>
void take_skip_to_pdu_impl<T>::set_take(uint32_t take)
{
    gr::thread::scoped_lock l(this->d_setlock);

    d_take = take;
}


template <class T>
void take_skip_to_pdu_impl<T>::set_skip(uint32_t skip)
{
    gr::thread::scoped_lock l(this->d_setlock);

    d_skip = skip;
}

template class take_skip_to_pdu<unsigned char>;
template class take_skip_to_pdu<short>;
template class take_skip_to_pdu<float>;
template class take_skip_to_pdu<gr_complex>;
} /* namespace pdu_utils */
} /* namespace gr */
