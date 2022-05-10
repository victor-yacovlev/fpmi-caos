import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:grpc/grpc.dart';
import 'package:grpc/grpc_connection_interface.dart';
import 'package:grpc/grpc_or_grpcweb.dart';
import 'package:grpc/grpc_web.dart';
import 'package:grpc_demo/src/generated/social_network.pb.dart';
import 'package:grpc_demo/src/generated/social_network.pbgrpc.dart';

class MainScreen extends StatefulWidget {
  final Uri grpcLocation;
  const MainScreen(this.grpcLocation, {Key? key}) : super(key: key);

  @override
  State createState() => MainScreenState();
}

class MainScreenState extends State<MainScreen> {

  final TextEditingController _loginName = TextEditingController();
  UserProfile? _loadedProfile;
  String? _errorText;
  late final ProfileManagerClient _profileManagerApi;


  @override
  void initState() {
    super.initState();
    final grpcChannel = GrpcWebClientChannel.xhr(widget.grpcLocation);
    _profileManagerApi = ProfileManagerClient(grpcChannel);
  }

  void loadUserProfile() {
    String login = _loginName.text;
    final futureProfile = _profileManagerApi.getUserProfile(User(login: login));
    futureProfile.then((UserProfile value) {
      setState((){
        _errorText = null;
        _loadedProfile = value;
      });
    })
    .onError((error, stackTrace) {
      setState(() {
        _loadedProfile = null;
        _errorText = error.toString() + '\n' + stackTrace.toString();
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    List<Widget> items = [];
    final searchBox = Card(
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          SizedBox(
            width: 450,
            height: 64,
            child: TextField(
              controller: _loginName,
              onEditingComplete: loadUserProfile,
            )
          ),
          ElevatedButton(
            onPressed: loadUserProfile,
            child: const Icon(Icons.search),
          ),
        ]
      ),
    );
    items.add(searchBox);
    if (_loadedProfile != null) {
      items.add(buildProfile(context));
    }
    if (_errorText != null) {
      items.add(Text(_errorText!,
        style: TextStyle(
          color: Theme.of(context).errorColor,
        ),
      ));
    }

    return MaterialApp(
      title: 'gRPC Demo',
      home: Scaffold(
        appBar: AppBar(
          title: const Text('gRPC Demo'),
          centerTitle: true,
        ),
        body: Column(children: items),
      ),
    );
  }

  Widget buildProfile(BuildContext context) {
    final theme = Theme.of(context);
    List<Row> items = [];
    void addRowItem(String label, String value) {
      items.add(Row(
        children: [
          Text(label + ': ',
            style: TextStyle(color: theme.primaryColor),
            textAlign: TextAlign.right,
          ),
          Text(value),
        ],
      ));
    }
    addRowItem('First name', _loadedProfile!.firstName);
    addRowItem('Last name', _loadedProfile!.lastName);
    addRowItem('Age', '${_loadedProfile!.age} years');
    return Card(
      child: Column(children: items),
    );
  }

}