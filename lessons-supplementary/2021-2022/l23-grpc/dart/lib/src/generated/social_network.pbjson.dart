///
//  Generated code. Do not modify.
//  source: social_network.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,unnecessary_const,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type,unnecessary_this,prefer_final_fields,deprecated_member_use_from_same_package

import 'dart:core' as $core;
import 'dart:convert' as $convert;
import 'dart:typed_data' as $typed_data;
@$core.Deprecated('Use genderDescriptor instead')
const Gender$json = const {
  '1': 'Gender',
  '2': const [
    const {'1': 'Male', '2': 0},
    const {'1': 'Female', '2': 1},
  ],
};

/// Descriptor for `Gender`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List genderDescriptor = $convert.base64Decode('CgZHZW5kZXISCAoETWFsZRAAEgoKBkZlbWFsZRAB');
@$core.Deprecated('Use phoneTypeDescriptor instead')
const PhoneType$json = const {
  '1': 'PhoneType',
  '2': const [
    const {'1': 'Home', '2': 0},
    const {'1': 'Work', '2': 1},
    const {'1': 'Mobile', '2': 2},
  ],
};

/// Descriptor for `PhoneType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List phoneTypeDescriptor = $convert.base64Decode('CglQaG9uZVR5cGUSCAoESG9tZRAAEggKBFdvcmsQARIKCgZNb2JpbGUQAg==');
@$core.Deprecated('Use userDescriptor instead')
const User$json = const {
  '1': 'User',
  '2': const [
    const {'1': 'login', '3': 1, '4': 1, '5': 9, '10': 'login'},
    const {'1': 'password', '3': 2, '4': 1, '5': 9, '9': 0, '10': 'password', '17': true},
  ],
  '8': const [
    const {'1': '_password'},
  ],
};

/// Descriptor for `User`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List userDescriptor = $convert.base64Decode('CgRVc2VyEhQKBWxvZ2luGAEgASgJUgVsb2dpbhIfCghwYXNzd29yZBgCIAEoCUgAUghwYXNzd29yZIgBAUILCglfcGFzc3dvcmQ=');
@$core.Deprecated('Use phoneDescriptor instead')
const Phone$json = const {
  '1': 'Phone',
  '2': const [
    const {'1': 'phone_type', '3': 1, '4': 1, '5': 14, '6': '.PhoneType', '10': 'phoneType'},
    const {'1': 'number', '3': 2, '4': 1, '5': 4, '10': 'number'},
  ],
};

/// Descriptor for `Phone`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List phoneDescriptor = $convert.base64Decode('CgVQaG9uZRIpCgpwaG9uZV90eXBlGAEgASgOMgouUGhvbmVUeXBlUglwaG9uZVR5cGUSFgoGbnVtYmVyGAIgASgEUgZudW1iZXI=');
@$core.Deprecated('Use userProfileDescriptor instead')
const UserProfile$json = const {
  '1': 'UserProfile',
  '2': const [
    const {'1': 'login', '3': 1, '4': 1, '5': 9, '10': 'login'},
    const {'1': 'first_name', '3': 2, '4': 1, '5': 9, '10': 'firstName'},
    const {'1': 'last_name', '3': 3, '4': 1, '5': 9, '10': 'lastName'},
    const {'1': 'age', '3': 4, '4': 1, '5': 13, '10': 'age'},
    const {'1': 'height', '3': 5, '4': 1, '5': 1, '10': 'height'},
    const {'1': 'gender', '3': 6, '4': 1, '5': 14, '6': '.Gender', '10': 'gender'},
    const {'1': 'phones', '3': 10, '4': 3, '5': 11, '6': '.Phone', '10': 'phones'},
    const {'1': 'avatar_url', '3': 100, '4': 1, '5': 9, '9': 0, '10': 'avatarUrl', '17': true},
  ],
  '8': const [
    const {'1': '_avatar_url'},
  ],
};

/// Descriptor for `UserProfile`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List userProfileDescriptor = $convert.base64Decode('CgtVc2VyUHJvZmlsZRIUCgVsb2dpbhgBIAEoCVIFbG9naW4SHQoKZmlyc3RfbmFtZRgCIAEoCVIJZmlyc3ROYW1lEhsKCWxhc3RfbmFtZRgDIAEoCVIIbGFzdE5hbWUSEAoDYWdlGAQgASgNUgNhZ2USFgoGaGVpZ2h0GAUgASgBUgZoZWlnaHQSHwoGZ2VuZGVyGAYgASgOMgcuR2VuZGVyUgZnZW5kZXISHgoGcGhvbmVzGAogAygLMgYuUGhvbmVSBnBob25lcxIiCgphdmF0YXJfdXJsGGQgASgJSABSCWF2YXRhclVybIgBAUINCgtfYXZhdGFyX3VybA==');
@$core.Deprecated('Use messageDescriptor instead')
const Message$json = const {
  '1': 'Message',
  '2': const [
    const {'1': 'sender', '3': 1, '4': 1, '5': 11, '6': '.User', '10': 'sender'},
    const {'1': 'receiver', '3': 2, '4': 1, '5': 11, '6': '.User', '10': 'receiver'},
    const {'1': 'content', '3': 3, '4': 1, '5': 9, '10': 'content'},
  ],
};

/// Descriptor for `Message`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List messageDescriptor = $convert.base64Decode('CgdNZXNzYWdlEh0KBnNlbmRlchgBIAEoCzIFLlVzZXJSBnNlbmRlchIhCghyZWNlaXZlchgCIAEoCzIFLlVzZXJSCHJlY2VpdmVyEhgKB2NvbnRlbnQYAyABKAlSB2NvbnRlbnQ=');
@$core.Deprecated('Use emptyDescriptor instead')
const Empty$json = const {
  '1': 'Empty',
};

/// Descriptor for `Empty`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List emptyDescriptor = $convert.base64Decode('CgVFbXB0eQ==');
