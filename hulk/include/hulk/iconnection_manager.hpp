#ifndef HULK_ICONNECTION_MANAGER_H_
#define HULK_ICONNECTION_MANAGER_H_

#include <memory>

namespace hulk
{
    class connection;

    class iconnection_manager
    {
    public:
        virtual ~iconnection_manager() = default;

        virtual void start(std::shared_ptr<connection> conn) = 0;

        virtual void stop(std::shared_ptr<connection> conn) = 0;
    }; 

} // namespace hulk

#endif // HULK_ICONNECTION_MANAGER_H_