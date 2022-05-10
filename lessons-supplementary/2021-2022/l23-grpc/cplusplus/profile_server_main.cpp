#include "profile_service.h"
#include <string>
#include <grpcpp/grpcpp.h>

int main() {
    const std::string listen_address = "localhost:1901";
    ProfileManagerService service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(listen_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    server->Wait();
    return 0;
}
