#pragma once

#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

typedef unsigned char byte_t;
typedef std::vector<byte_t> byte_vector;
typedef boost::shared_ptr<byte_vector> byte_vec_ptr;

struct content_processor {
    virtual byte_vec_ptr operator()(
      const byte_t* psrc, std::size_t size) {
        return byte_vec_ptr(new byte_vector(psrc, psrc + size));
    }
};

struct socket_t: tcp::socket {
    socket_t(boost::asio::io_service& iosvc)
        : tcp::socket(iosvc) {}
    template <typename Buf, typename Handler>
    void async_read_some(const Buf& buf, const Handler& handler) {
        boost::lock_guard<boost::mutex> lock(mut_);
        tcp::socket::async_read_some(buf, handler);
    }
    template <typename Buf, typename Handler>
    void async_write_some(const Buf& buf, const Handler& handler) {
        boost::lock_guard<boost::mutex> lock(mut_);
        tcp::socket::async_write_some(buf, handler);
    }
    void shutdown(shutdown_type what, boost::system::error_code& err) {
        boost::lock_guard<boost::mutex> lock(mut_);
        tcp::socket::shutdown(what, err);
    }
    void cancel(boost::system::error_code& err) {
        boost::lock_guard<boost::mutex> lock(mut_);
        tcp::socket::cancel(err);
    }

private:
    boost::mutex mut_;
};

typedef boost::shared_ptr<socket_t> socket_ptr;
typedef boost::shared_ptr<content_processor> content_processor_ptr;
