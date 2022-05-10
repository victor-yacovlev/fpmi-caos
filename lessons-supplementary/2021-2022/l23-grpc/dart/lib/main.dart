import 'package:flutter/material.dart';
import 'package:grpc_demo/src/main_screen.dart';

void main() {
  final app = MainScreen(Uri.parse('http://localhost:8081'));
  runApp(app);
}