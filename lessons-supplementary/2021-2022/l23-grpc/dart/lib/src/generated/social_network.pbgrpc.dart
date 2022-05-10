///
//  Generated code. Do not modify.
//  source: social_network.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,unnecessary_const,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type,unnecessary_this,prefer_final_fields

import 'dart:async' as $async;

import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'social_network.pb.dart' as $0;
export 'social_network.pb.dart';

class ProfileManagerClient extends $grpc.Client {
  static final _$getUserProfile = $grpc.ClientMethod<$0.User, $0.UserProfile>(
      '/ProfileManager/GetUserProfile',
      ($0.User value) => value.writeToBuffer(),
      ($core.List<$core.int> value) => $0.UserProfile.fromBuffer(value));

  ProfileManagerClient($grpc.ClientChannel channel,
      {$grpc.CallOptions? options,
      $core.Iterable<$grpc.ClientInterceptor>? interceptors})
      : super(channel, options: options, interceptors: interceptors);

  $grpc.ResponseFuture<$0.UserProfile> getUserProfile($0.User request,
      {$grpc.CallOptions? options}) {
    return $createUnaryCall(_$getUserProfile, request, options: options);
  }
}

abstract class ProfileManagerServiceBase extends $grpc.Service {
  $core.String get $name => 'ProfileManager';

  ProfileManagerServiceBase() {
    $addMethod($grpc.ServiceMethod<$0.User, $0.UserProfile>(
        'GetUserProfile',
        getUserProfile_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $0.User.fromBuffer(value),
        ($0.UserProfile value) => value.writeToBuffer()));
  }

  $async.Future<$0.UserProfile> getUserProfile_Pre(
      $grpc.ServiceCall call, $async.Future<$0.User> request) async {
    return getUserProfile(call, await request);
  }

  $async.Future<$0.UserProfile> getUserProfile(
      $grpc.ServiceCall call, $0.User request);
}

class ChatServiceClient extends $grpc.Client {
  static final _$sendMessage = $grpc.ClientMethod<$0.Message, $0.Empty>(
      '/ChatService/SendMessage',
      ($0.Message value) => value.writeToBuffer(),
      ($core.List<$core.int> value) => $0.Empty.fromBuffer(value));
  static final _$receiveMessages = $grpc.ClientMethod<$0.User, $0.Message>(
      '/ChatService/ReceiveMessages',
      ($0.User value) => value.writeToBuffer(),
      ($core.List<$core.int> value) => $0.Message.fromBuffer(value));

  ChatServiceClient($grpc.ClientChannel channel,
      {$grpc.CallOptions? options,
      $core.Iterable<$grpc.ClientInterceptor>? interceptors})
      : super(channel, options: options, interceptors: interceptors);

  $grpc.ResponseFuture<$0.Empty> sendMessage($0.Message request,
      {$grpc.CallOptions? options}) {
    return $createUnaryCall(_$sendMessage, request, options: options);
  }

  $grpc.ResponseStream<$0.Message> receiveMessages($0.User request,
      {$grpc.CallOptions? options}) {
    return $createStreamingCall(
        _$receiveMessages, $async.Stream.fromIterable([request]),
        options: options);
  }
}

abstract class ChatServiceBase extends $grpc.Service {
  $core.String get $name => 'ChatService';

  ChatServiceBase() {
    $addMethod($grpc.ServiceMethod<$0.Message, $0.Empty>(
        'SendMessage',
        sendMessage_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $0.Message.fromBuffer(value),
        ($0.Empty value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.User, $0.Message>(
        'ReceiveMessages',
        receiveMessages_Pre,
        false,
        true,
        ($core.List<$core.int> value) => $0.User.fromBuffer(value),
        ($0.Message value) => value.writeToBuffer()));
  }

  $async.Future<$0.Empty> sendMessage_Pre(
      $grpc.ServiceCall call, $async.Future<$0.Message> request) async {
    return sendMessage(call, await request);
  }

  $async.Stream<$0.Message> receiveMessages_Pre(
      $grpc.ServiceCall call, $async.Future<$0.User> request) async* {
    yield* receiveMessages(call, await request);
  }

  $async.Future<$0.Empty> sendMessage(
      $grpc.ServiceCall call, $0.Message request);
  $async.Stream<$0.Message> receiveMessages(
      $grpc.ServiceCall call, $0.User request);
}
