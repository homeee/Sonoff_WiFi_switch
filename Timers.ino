void initTimers() {
  HTTP.on("/timerSave", handle_timer_Save);
  HTTP.on("/timersDel", handle_timer_Del);
// задача проверять таймеры каждую секунду.
  ts.add(0, 1000, [&](void*) {
    runTimers();
  }, nullptr, true);
  loadTimer();
  modulesReg("timers");
}

void handle_timer_Save() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& Timers = jsonBuffer.parseObject(jsonTimer);
  JsonArray& arrays = Timers["timer"].asArray();
  JsonObject& record = arrays.createNestedObject();
  record["id"]  = HTTP.arg("id").toInt();
  record["trigger"]  = HTTP.arg("trigger");
  record["module"]  = HTTP.arg("module");
  record["day"]  = HTTP.arg("day");
  record["time"]  = HTTP.arg("time");
  record["work"]  = HTTP.arg("work");
  jsonTimer = "";
  Timers.printTo(jsonTimer);
  writeFile("timer.save.json", jsonTimer );

  loadTimer();
  HTTP.send(200, "text/plain", "OK");
}

void handle_timer_Del() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& Timers = jsonBuffer.parseObject(jsonTimer);
  JsonArray& nestedArray = Timers["timer"].asArray();
  //nestedArray.printTo(Serial);
  int y;
  for (int i = 0; i <= nestedArray.size() - 1; i++) {
    if (Timers["timer"][i]["id"] == HTTP.arg("id").toInt()) y = i;

  }
  nestedArray.removeAt(y);
  jsonTimer = "";
  Timers.printTo(jsonTimer);
  writeFile("timer.save.json", jsonTimer );
  loadTimer();
  HTTP.send(200, "text/plain", "OK");
}

bool loadTimer() {
  Timerset = "";
  jsonTimer = readFile("timer.save.json", 4096);
  String Weekday = GetWeekday();
  DynamicJsonBuffer jsonBuffer;
  JsonObject& Timers = jsonBuffer.parseObject(jsonTimer);
  JsonArray& nestedArray = Timers["timer"].asArray();
  int j = nestedArray.size();
  if (j != 0) {
    for (int i = 0; i <= j - 1; i++) {
      if ((Weekday == Timers["timer"][i]["day"].as<String>()) || (Timers["timer"][i]["day"].as<String>() == "All")) {
        Timerset += Timers["timer"][i]["time"].as<String>() + ",";
        Timerset += Timers["timer"][i]["module"].as<String>() + ",";
        Timerset += Timers["timer"][i]["work"].as<String>() + ",";
        Timerset += Timers["timer"][i]["trigger"].as<String>();
        Timerset += "\r\n";
      }
    }
    Serial.println(Timerset);
  }
  //runTimers();
  return true;
}

void runTimers() {
  // Список текущих таймеров во временную переменную
  String timers = Timerset;
  String now = GetTime();
  configJson = jsonWrite(configJson, "time", now);
  int i;
  // Будем повторять проверку для каждого установленного таймера
  do {
    // проверяем есть ли таймеры
    i = timers.indexOf("\r\n");
    if (i != -1) {
      // получаем строку текщего таймера
      String timer = timers.substring(0, i);
      // Если время совпадает с текущим
      //Serial.println(GetTime());
      if (timer.substring(0, 8) == now) {
        // Отрезаем время из строки 12:44:15,relay,0,not
        timer = timer.substring(9);
        int b = timer.indexOf(",");
        String temp = timer.substring(0, b);
        Serial.println(temp);
        int e = timer.lastIndexOf(",");
        // Загружаем время работы реле
        int interval = timer.substring(b, e).toInt();
        // выполняем необходимое действие
        temp += timer.substring(e + 1);
        Serial.println(temp);
        if (temp != "") {
          command = temp;
        }

      }
      timers = timers.substring(timers.indexOf("\r\n") + 2);
    }
  } while (i != -1);
}

