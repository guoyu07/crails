#ifndef  REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include "http_server.hpp"

namespace Crails
{
  class RequestHandler
  {
  public:
    RequestHandler(const std::string& name) : name(name) {}
    virtual ~RequestHandler() {}

    const std::string& get_name(void) const { return name; }

    virtual void operator()(const HttpServer::request& request, BuildingResponse& response, Params& params, std::function<void(bool)> callback) = 0;
    
  private:
    const std::string name;
  };
}

#endif
