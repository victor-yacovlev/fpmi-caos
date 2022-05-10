#include "profile_service.h"

using grpc::Status;
using grpc::ServerContext;

Status ProfileManagerService::GetUserProfile(ServerContext *context, const User *request, UserProfile *response) {
    Status status = Status::OK;
    if (request->login() == "vovan") {
        response->set_login(request->login());
        response->set_first_name("Вовка");
        response->set_last_name("Пу");
        response->set_age(69);
        response->set_gender(Gender::Male);
        response->set_height(170.0);

    } else if (request->login() == "dimon") {
        response->set_login(request->login());
        response->set_first_name("Дима");
        response->set_last_name("Ме");
        response->set_gender(Gender::Male);
        response->set_age(56);
        Phone* phone = response->add_phones();
        phone->set_number(70001234567);
        phone->set_phone_type(PhoneType::Mobile);
    } else {
        status = Status(grpc::StatusCode::NOT_FOUND, "Profile not found", request->login());
    }
    return status;
}
