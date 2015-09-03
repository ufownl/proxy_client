#pragma once

#include "common.h"

const std::size_t BUFFER_SIZE = 8 * 1024;

typedef boost::array<byte_t, BUFFER_SIZE> buf_t;
typedef boost::shared_ptr<buf_t> buf_ptr;

struct connect_handler {
    socket_ptr pclient_socket;
    socket_ptr pserver_socket;
    content_processor_ptr pencoder;
    content_processor_ptr pdecoder;

    connect_handler(
      socket_ptr pclient_socket,
      content_processor_ptr pencoder,
      content_processor_ptr pdecoder)
        : pclient_socket(pclient_socket)
        , pserver_socket(new tcp::socket(pclient_socket->get_io_service()))
        , pencoder(pencoder)
        , pdecoder(pdecoder) {}
    void operator()(
        const boost::system::error_code& err, 
        tcp::resolver::iterator server_it);
};

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
    socket_ptr   pdst;
    byte_vec_ptr pbuf;
    std::size_t  pos;

    write_handler(socket_ptr pdst, byte_vec_ptr pbuf)
        : pdst(pdst)
        , pbuf(pbuf)
        , pos(0) {}
    byte_t* raw_buf_ptr() { return &(*pbuf)[0]; }
    boost::asio::mutable_buffers_1 asio_buffer() { 
        return boost::asio::buffer(raw_buf_ptr() + pos, pbuf->size() - pos);
    }
    void async_write_some() { pdst->async_write_some(asio_buffer(), *this); }
    void operator()(
        const boost::system::error_code& err, std::size_t bytes_written);
};
