void saveSocketSet() {
  String socketID = server.arg("socketID");
  String isv3 = server.arg("isV3");
  if (isv3 != "on") {
    isv3 = "off";
  }
  String socketName = server.arg("name");
  String houseCode = server.arg("houseCode");
  String groupCode = server.arg("groupCode");
  String socketCode = server.arg("socketCode");

  String path = SOCKET_CONFIG_PATH + socketID;
  File f = SPIFFS.open(path, "w");
  
  f.println(isv3);
  f.println(socketName);
  f.println(houseCode);
  f.println(groupCode);
  f.println(socketCode);
  
  f.close();
  loadSocketSets();
  returnTo("/settings");
}

void loadSocketSets() {
  Dir socketFolder = SPIFFS.openDir(SOCKET_CONFIG_PATH);
  while(socketFolder.next()){
    File entry = socketFolder.openFile("r");
    if (entry) {
      String socketId = entry.name();
      String isv3;
      String socketName;
      String houseCode;
      String groupCode;
      String socketCode;
      int n = 0;
      while (entry.available()) {
        switch (n){
          case 0:
            isv3 = entry.readStringUntil('\n');
            break;
          case 1:
            socketName = entry.readStringUntil('\n');
            break;
          case 2:
            houseCode = entry.readStringUntil('\n');
            break;
          case 3:
            groupCode = entry.readStringUntil('\n');
            break;
          case 4:
            socketCode = entry.readStringUntil('\n');
            break;
          default:
            print("Error too many lines in reading sockets... deleting file");
            SPIFFS.remove(entry.name());
            break;
        }
        n ++;
      }
      socketId.replace(SOCKET_CONFIG_PATH, "");
      int i = socketId.toInt();
      isv3.trim();
      socketName.trim();
      houseCode.trim();
      groupCode.trim();
      socketCode.trim();
      configSocketSets[i][0] = isv3;
      configSocketSets[i][1] = socketName;
      configSocketSets[i][2] = houseCode;
      configSocketSets[i][3] = groupCode;
      configSocketSets[i][4] = socketCode;
      print("Cache socket { ID: "+socketId+\
            ", isv3: "+isv3+\
            ", Name: "+socketName+\
            ", HouseC: "+houseCode+\
            ", GroupC: "+groupCode+\
            ", SocketC: "+socketCode+\
            " }");
      entry.close();
    } else {
      print("Error in reading sockets");
    }
  }
}

void SocketOn(int i) {
  String isv3String = configSocketSets[i][0];
  bool isv3;
  if (isv3String.equals(String("on"))) {
    isv3 = true;
  } else {
    isv3 = false;
  }
  String houseC_String = configSocketSets[i][2];
  int groupC = configSocketSets[i][3].toInt();
  int socketC = configSocketSets[i][4].toInt();

  print("On: " + String(i));

  if (isv3) {
    char houseC = houseC_String.charAt(0);
    mySwitch.switchOn(houseC, groupC, socketC);
  } else {
    //@TODO: friedls scheiß funktioniert einfach nicht
    int len = houseC_String.length() + 1;
    char houseC[len];
    houseC_String.toCharArray(houseC, len);

    Serial.println(houseC);
    char* yolo = "00100";
    
    mySwitch.switchOn(&houseC[0], socketC);
  }
  statusSocketSets[i] = true;
  returnTo("/");
}

void SocketOff(int i) {
  String isv3String = configSocketSets[i][0];
  bool isv3;
  if (isv3String.equals(String("on"))) {
    isv3 = true;
  } else {
    isv3 = false;
  }
  String houseC_String = configSocketSets[i][2];
  int groupC = configSocketSets[i][3].toInt();
  int socketC = configSocketSets[i][4].toInt();

  print("Off: " + String(i));

  if (isv3) {
    char houseC = houseC_String.charAt(0);
    mySwitch.switchOff(houseC, groupC, socketC);
  } else {
    //@TODO: friedls scheiß funktioniert einfach nicht
    int len = houseC_String.length() + 1;
    char houseC[len];
    houseC_String.toCharArray(houseC, len);
    
    Serial.println(houseC);
    char* yolo = "00100";
    
    mySwitch.switchOff(&houseC[0], socketC);
  }
  statusSocketSets[i] = false;
  returnTo("/");
}

void SocketToggle(int i) {
  Serial.print("Toggle-");
  if (!statusSocketSets[i]) {
    SocketOn(i);
  } else {
    SocketOff(i);
  }
}

