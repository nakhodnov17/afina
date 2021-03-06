#ifndef AFINA_NETWORK_BLOCKING_SERVER_H
#define AFINA_NETWORK_BLOCKING_SERVER_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <unordered_set>

#include <afina/Executor.h>
#include <afina/network/Server.h>

namespace Afina {
namespace Network {
namespace Blocking {

/**
 * # Network resource manager implementation
 * Server that is spawning a separate thread for each connection
 */
class ServerImpl : public Server {
public:
    explicit ServerImpl(std::shared_ptr<Afina::Storage> ps);

    ~ServerImpl() override = default;

    // See Server.h
    void Start(uint16_t port, uint16_t workers) override;

    // See Server.h
    void Stop() override;

    // See Server.h
    void Join() override;

protected:
    /**
     * Method is running in the connection acceptor thread
     */
    void RunAcceptor();

    /**
     * Methos is running for each connection
     */
    void RunConnection(int con_socket);

private:
    static void *RunAcceptorProxy(void *p);

    static void *RunConnectionProxy(void *proxy_args);

    // Check if the connection exists.
    bool IsConnectionActive(int con_socket);

    // Close connection and destroy worker.
    void CloseConnection(int con_socket);

    // Read len bytes from con_socket to dest.
    bool ReadStrict(int con_socket, char *dest, size_t len);

    // Write len to con_socket from source.
    bool WriteStrict(int con_socket, const char *source, size_t len);

    // Nested class to pass parameters of new connection through proxy function
    class ProxyArgs{
    public:
        ServerImpl *server;
        int con_socket;
    };

    // Atomic flag to notify threads when it is time to stop. Note that
    // flag must be atomic in order to safely publisj changes cross thread
    // bounds
    std::atomic<bool> running;

    // Maximum number of client allowed to exists concurrently
    // on server, permits access only from inside of accept_thread.
    // Read-only
    uint16_t max_workers;

    // Port to listen for new connections, permits access only from
    // inside of accept_thread
    // Read-only
    uint16_t listen_port;

    // Main (accepter) socket
    int server_socket;

    // ThreadPool executor
    Afina::Executor executor;
};

} // namespace Blocking
} // namespace Network
} // namespace Afina

#endif // AFINA_NETWORK_BLOCKING_SERVER_H
