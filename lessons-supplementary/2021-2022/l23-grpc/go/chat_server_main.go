package main

import (
	pb "chat_server/generated/chat_server"
	"google.golang.org/grpc"
	"log"
	"net"
)

func main() {
	service := ChatService{}
	service.Initialize()
	var opts []grpc.ServerOption
	grpcServer := grpc.NewServer(opts...)
	pb.RegisterChatServiceServer(grpcServer, &service)
	listener, err := net.Listen("tcp", "localhost:1902")
	if err != nil {
		log.Fatalf("Cant listen: %v", err)
	}
	grpcServer.Serve(listener)
}
