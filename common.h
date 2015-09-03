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

typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<content_processor> content_processor_ptr;
