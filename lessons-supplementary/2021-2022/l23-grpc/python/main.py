import asyncio

import grpc
import social_network_pb2_grpc as gen
import social_network_pb2 as pb

HOST = 'localhost'
PROFILES_PORT = 1901
CHAT_PORT = 1902

profiles_server = grpc.insecure_channel(f'{HOST}:{PROFILES_PORT}')
profile_manager = gen.ProfileManagerStub(profiles_server)

chat_server = grpc.insecure_channel(f'{HOST}:{CHAT_PORT}')
chat_service = gen.ChatServiceStub(chat_server)


def get_profile(name: str) -> pb.UserProfile:
    user = pb.User(login=name)
    return profile_manager.GetUserProfile(user)


def send_message(sender: str, receiver: str, text: str):
    from_user = pb.User(login=sender)
    to_user = pb.User(login=receiver)
    message = pb.Message(sender=from_user, receiver=to_user, content=text)
    chat_service.SendMessage(message)


def start_receiving(login: str):
    user = pb.User(login=login)
    messages_stream = chat_service.ReceiveMessages(user)
    for message in messages_stream:
        print(f'-- Got message from {message.sender.login}: {message.content}')

