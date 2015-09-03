#pragma once

#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

typedef unsigned char byte_t;

struct content_processor {
    virtual void operator()(
      const byte_t* psrc, std::size_t size, byte_t* pdst) {
        std::copy(psrc, psrc + size, pdst);
    }
};

typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<content_processor> content_processor_ptr;
