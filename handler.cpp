#include "handler.h"

void read_handler::close() {
    boost::system::error_code err;
    psrc->shutdown(tcp::socket::shutdown_both, err);
    pdst->shutdown(tcp::socket::shutdown_both, err);
    psrc->close(err);
    pdst->close(err);
}

void read_handler::operator()(
  const boost::system::error_code& err, std::size_t bytes_read) {
    try {
        if (!err) {
            if (bytes_read) {
                write_handler writer(pdst, bytes_read);
                (*pprocess)(raw_buf_ptr(), bytes_read, writer.raw_buf_ptr());
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
            if (pos < size) async_write_some();
        }
    } catch (...) {
    }
}
