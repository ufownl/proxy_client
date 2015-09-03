#pragma once

#include "common.h"

const std::size_t BUFFER_SIZE = 8 * 1024;

typedef boost::array<byte_t, BUFFER_SIZE> buf_t;
typedef boost::shared_ptr<buf_t> buf_ptr;

struct read_handler {
    socket_ptr psrc;
    socket_ptr pdst;
    buf_ptr pbuf;
    content_processor_ptr pprocess;

    read_handler(
      socket_ptr psrc,
      socket_ptr pdst,
      content_processor_ptr pprocess)
        : psrc(psrc)
        , pdst(pdst)
        , pbuf(new buf_t())
        , pprocess(pprocess) {}
    byte_t* raw_buf_ptr() { return &(*pbuf)[0]; }
    boost::asio::mutable_buffers_1 asio_buffer() { 
        return boost::asio::buffer(*pbuf); 
    }
    void close();
    void async_read_some() { psrc->async_read_some(asio_buffer(), *this); }
    void operator()(
        const boost::system::error_code& err, std::size_t bytes_read);
};

struct write_handler {
    socket_ptr pdst;
    buf_ptr pbuf;
    std::size_t pos;
    std::size_t size;

    write_handler(socket_ptr pdst, std::size_t size)
        : pdst(pdst)
        , pbuf(new buf_t())
        , pos(0)
        , size(size) {}
    byte_t* raw_buf_ptr() { return &(*pbuf)[0]; }
    boost::asio::mutable_buffers_1 asio_buffer() { 
        return boost::asio::buffer(raw_buf_ptr() + pos, size - pos);
    }
    void async_write_some() { pdst->async_write_some(asio_buffer(), *this); }
    void operator()(
        const boost::system::error_code& err, std::size_t bytes_written);
};