#ifndef HULK_CONNECTION_MANAGER_H_
#define HULK_CONNECTION_MANAGER_H_

#include "iconnection_manager.hpp"
#include "connection.hpp"

#include <set>

namespace hulk
{
    class connection_manager : public iconnection_manager
    {
    public:
        connection_manager() = default;

        void start(std::shared_ptr<connection> conn) override
        {
            m_connections.insert(conn);
            log::info("Connection started: {} connections active.", m_connections.size());
            conn->start();
        }

        void stop(std::shared_ptr<connection> conn) override
        {
            // conn->stop();
            m_connections.erase(conn);
            log::info("Connection stopped: {} active connections remaining.", m_connections.size());
        }

    private:
        std::set<std::shared_ptr<connection>> m_connections;
    }; 

} // namespace hulk
#endif // HULK_CONNECTION_MANAGER_H_