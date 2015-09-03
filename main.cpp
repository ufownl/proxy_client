#include "common.h"
#include "handler.h"
#include "content_processor.h"
#include "config.h"

const unsigned NUM_THREAD = 8;

void start(
    socket_ptr pclient_socket,
    tcp::resolver::iterator server_it,
    content_processor_ptr pencoder,
    content_processor_ptr pdecoder);

int main() {
    try {
        std::cout << "Initializing..." << std::endl;
        config cfg = get_cfg();
        content_processor_ptr pencoder, pdecoder;
        if (cfg.key_src.size()) {
            pencoder.reset(new encoder(cfg.key_src));
            pdecoder.reset(new decoder(cfg.key_src));
        } else {
            pencoder.reset(new content_processor);
            pdecoder.reset(new content_processor);
        }

        boost::asio::io_service iosvc;
        boost::asio::io_service::work work(iosvc);
        for (unsigned i = 0; i < NUM_THREAD; ++i) {
            new boost::thread(
                boost::bind(&boost::asio::io_service::run, &iosvc));
        }

        tcp::resolver resolver(iosvc);
        tcp::resolver::iterator server_it = 
            resolver.resolve(
                tcp::resolver::query(cfg.remote_host, cfg.remote_port));
        tcp::acceptor acceptor(iosvc, 
            tcp::endpoint(tcp::v4(), 
                static_cast<unsigned short>(
                    std::atoi(cfg.local_port.c_str()))));

        std::cout << "Up and running..." << std::endl;
        while (true) {
            socket_ptr pclient_socket(
                new tcp::socket(iosvc));
            boost::system::error_code err;
            acceptor.accept(*pclient_socket, err);
            if (!err) {
                iosvc.post(
                    boost::bind(start,
                        pclient_socket, server_it, pencoder, pdecoder));
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught in main:\n" 
                  << "  " << e.what() << '\n'
                  << "Terminating..." << std::endl;
    }
    catch (...) {
        std::cout << "Unknown exception caught in main.\nTerminating..." 
                  << std::endl;
    }
    return 0;
}

void start(
  socket_ptr pclient_socket,
  tcp::resolver::iterator server_it,
  content_processor_ptr pencoder,
  content_processor_ptr pdecoder) {
    try {
        boost::asio::io_service& iosvc = pclient_socket->get_io_service();
        socket_ptr pserver_socket(new tcp::socket(iosvc));
        boost::asio::connect(*pserver_socket, server_it);
        read_handler client_reader(pclient_socket, pserver_socket, pencoder);
        client_reader.async_read_some();
        read_handler server_reader(pserver_socket, pclient_socket, pdecoder);
        server_reader.async_read_some();
    } catch (...) {}
}
