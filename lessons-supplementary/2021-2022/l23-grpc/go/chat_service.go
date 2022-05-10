package main

import (
	pb "chat_server/generated/chat_server"
	"context"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
)

type ChatService struct {
	pb.UnimplementedChatServiceServer
	Inboxes map[string]chan *pb.Message
}

func (service *ChatService) Initialize() {
	service.Inboxes = make(map[string]chan *pb.Message)
}

func (service *ChatService) SendMessage(ctx context.Context, message *pb.Message) (*pb.Empty, error) {
	receiver := message.Receiver.Login
	inbox, exists := service.Inboxes[receiver]
	if !exists {
		return nil, status.Error(codes.NotFound, "User is not present online")
	}
	inbox <- message
	return &pb.Empty{}, nil
}

func (service *ChatService) ReceiveMessages(user *pb.User, sink pb.ChatService_ReceiveMessagesServer) error {
	inbox, exists := service.Inboxes[user.Login]
	if !exists {
		service.Inboxes[user.Login] = make(chan *pb.Message)
		inbox = service.Inboxes[user.Login]
	} else {
		inbox = service.Inboxes[user.Login]
	}
	for message := range inbox {
		if err := sink.Send(message); err != nil {
			break
		}
	}
	return nil
}
