#include "handler.h"

void connect_handler::operator()(
  const boost::system::error_code& err, tcp::resolver::iterator server_it) {
    try {
        if (!err) {
            read_handler client_reader(
                pclient_socket, pserver_socket, pencoder);
            client_reader.async_read_some();
            read_handler server_reader(
                pserver_socket, pclient_socket, pdecoder);
            server_reader.async_read_some();
        }
    } catch (...) {}
}

void read_handler::close() {
    boost::system::error_code err;
    pdst->shutdown(tcp::socket::shutdown_both, err);
    pdst->cancel(err);
}

void read_handler::operator()(
  const boost::system::error_code& err, std::size_t bytes_read) {
    try {
        if (!err) {
            if (bytes_read) {
                write_handler writer(
                    pdst, (*pprocess)(raw_buf_ptr(), bytes_read));
                writer.async_write_some();
            }
            async_read_some();
        } else {
            close();
        }
    } catch (...) {
        close();
    }
}

void write_handler::operator()(
  const boost::system::error_code& err, std::size_t bytes_written) {
    try {
        if (!err) {
            pos += bytes_written;
            if (pos < pbuf->size()) async_write_some();
        }
    } catch (...) {
    }
}
