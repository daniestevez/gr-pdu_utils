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

#include "pdu_logger_impl.h"
#include <gnuradio/io_signature.h>
#include <cerrno>

namespace gr {
namespace pdu_utils {

pdu_logger::sptr pdu_logger::make(std::string logfile)
{
    return gnuradio::get_initial_sptr(new pdu_logger_impl(logfile));
}

/*
 * The private constructor
 */
pdu_logger_impl::pdu_logger_impl(std::string logfile)
    : gr::block(
          "pdu_logger", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_logfile(logfile),
      d_burstnum(0)
{
    message_port_register_in(PMTCONSTSTR__PDU_IN);
    set_msg_handler(PMTCONSTSTR__PDU_IN,
                    boost::bind(&pdu_logger_impl::handle_pdu, this, _1));
}

/*
 * Our virtual destructor.
 */
pdu_logger_impl::~pdu_logger_impl() {}

void pdu_logger_impl::handle_pdu(pmt::pmt_t pdu)
{
    // make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_NOTICE(d_logger, "received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t samples = pmt::cdr(pdu);
    size_t N = pmt::length(samples);

    if (pmt::is_c32vector(samples)) {
        const gr_complex* burst = (const gr_complex*)pmt::c32vector_elements(samples, N);
        write_data_c(burst, N, (char*)"c", d_burstnum);
        d_burstnum++;
    } else if (pmt::is_f32vector(samples)) {
        const float* burst = (const float*)pmt::f32vector_elements(samples, N);
        write_data_f(burst, N, (char*)"f", d_burstnum);
        d_burstnum++;
    }
}

void pdu_logger_impl::write_data_c(const gr_complex* data,
                                   size_t len,
                                   char* name,
                                   int num)
{
    char filename[512];
    sprintf(filename, "%s%s_%04d.fc32", d_logfile.c_str(), name, num);
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        char msg[128] = "Error opening ";
        perror(strcat(msg, filename));
    } else {
        fwrite(data, sizeof(gr_complex), len, fp);
        fclose(fp);
    }
}

void pdu_logger_impl::write_data_f(const float* data, size_t len, char* name, int num)
{
    char filename[512];
    sprintf(filename, "%s%s_%04d.f32", d_logfile.c_str(), name, num);
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        char msg[128] = "Error opening ";
        perror(strcat(msg, filename));
    } else {
        fwrite(data, sizeof(float), len, fp);
        fclose(fp);
    }
}

} /* namespace pdu_utils */
} /* namespace gr */
