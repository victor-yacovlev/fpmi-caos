#ifndef GRPC_CHAT_EXAMPLE_PROFILE_SERVICE_H
#define GRPC_CHAT_EXAMPLE_PROFILE_SERVICE_H

#include "social_network.grpc.pb.h"

class ProfileManagerService: public ProfileManager::Service
{
public:
    grpc::Status
    GetUserProfile(::grpc::ServerContext *context, const ::User *request, ::UserProfile *response) override;

public:

};

#endif //GRPC_CHAT_EXAMPLE_PROFILE_SERVICE_H
