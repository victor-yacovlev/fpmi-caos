///
//  Generated code. Do not modify.
//  source: social_network.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,unnecessary_const,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type,unnecessary_this,prefer_final_fields

// ignore_for_file: UNDEFINED_SHOWN_NAME
import 'dart:core' as $core;
import 'package:protobuf/protobuf.dart' as $pb;

class Gender extends $pb.ProtobufEnum {
  static const Gender Male = Gender._(0, const $core.bool.fromEnvironment('protobuf.omit_enum_names') ? '' : 'Male');
  static const Gender Female = Gender._(1, const $core.bool.fromEnvironment('protobuf.omit_enum_names') ? '' : 'Female');

  static const $core.List<Gender> values = <Gender> [
    Male,
    Female,
  ];

  static final $core.Map<$core.int, Gender> _byValue = $pb.ProtobufEnum.initByValue(values);
  static Gender? valueOf($core.int value) => _byValue[value];

  const Gender._($core.int v, $core.String n) : super(v, n);
}

class PhoneType extends $pb.ProtobufEnum {
  static const PhoneType Home = PhoneType._(0, const $core.bool.fromEnvironment('protobuf.omit_enum_names') ? '' : 'Home');
  static const PhoneType Work = PhoneType._(1, const $core.bool.fromEnvironment('protobuf.omit_enum_names') ? '' : 'Work');
  static const PhoneType Mobile = PhoneType._(2, const $core.bool.fromEnvironment('protobuf.omit_enum_names') ? '' : 'Mobile');

  static const $core.List<PhoneType> values = <PhoneType> [
    Home,
    Work,
    Mobile,
  ];

  static final $core.Map<$core.int, PhoneType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static PhoneType? valueOf($core.int value) => _byValue[value];

  const PhoneType._($core.int v, $core.String n) : super(v, n);
}

