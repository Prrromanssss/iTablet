#include "FireTimer.h" // Подключаем библиотеку FireTimerFireTimer
#include <SoftwareSerial.h> // Подключаем библиотеку SoftwareSerial, для подключение модуля к плате
#include <DFPlayerMini_Fast.h> // Подключаем библиотеку DFPlayerMini_Fastб для работы со звуковым модулем
#include <ESP8266WiFi.h> // Подключаем библиотеку, для подключения платы к WiFi
#include <UniversalTelegramBot.h> // Подключаем библиотеку, для работы с телеграмм ботом

#define WIFI_SSID "iPhone(Roman)" // Указываем сеть WiFi
#define WIFI_PASSWORD "12345678" // Указываем пароль от WiFi

WiFiClientSecure secured_client;

  /////////////////////////
  //Конфигурация телеграм//
  ////////////////////////
  #define BOT_TOKEN "5027308973:AAF4sh1pBpQFBr1BvIZNr5l_JiYyqMPA-fg"
  #define CHAT_ID "1921020697" //https://api.telegram.org/bot1875231296:AAE7Z-XxLwqJ9tOUwt1IueRa8Ilszkcbptg/getUpdates
  const unsigned long BOT_MTBS = 3000;
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
  UniversalTelegramBot bot(BOT_TOKEN, secured_client); // Создаем объект bot
    
unsigned long bot_lasttime; 

SoftwareSerial mySerial(D0, D1); // Указываем к какими портам подключен DFPlayer, RX, TX
DFPlayerMini_Fast myMP3; // Создаем объект myMP3

unsigned long last_press;
int map_for_flags[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int map_for_but[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
String name_of_buttons[9] = {"Хочу в туалет", "Хочу спать", "Хочу кушать", "Радость", "Мне грустно", "Хочу гулять", "Папа", "Мама", "Хочу пить"};

void setup() {
  //SoftwareSerial mySerial(D0, D1); // Указываем к какими портам подключен DFPlayer, RX, TX
 
  Serial.begin(9600);  
  mySerial.begin(9600); // Открываем последовательную связь
  myMP3.begin(mySerial); // инициализация
  myMP3.volume(5);// Указываем громкость (0-30)  
  
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D9, INPUT_PULLUP); 
  pinMode(A0, INPUT_PULLUP);
  pinMode(D10, INPUT_PULLUP);
  
  /////////////////////////
  // Подключение к WiFi //
  ///////////////////////
   configTime(0, 0, "pool.ntp.org");
   secured_client.setTrustAnchors(&cert);
   Serial.print("Connecting to WiFi SSID ");
   Serial.print(WIFI_SSID);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
    
   }
   Serial.print("\nWiFi connected. IP address: ");
   Serial.println(WiFi.localIP());

   bot.sendMessage(CHAT_ID, "Привет! Я готов общаться!", "");
  
}

void loop() {
        ////////////////////////////////////
       // Обработка приходящих сообщений //
      ////////////////////////////////////
    
//      if(millis() - bot_lasttime > BOT_MTBS) {
//          // Считываем новые сообщения
//          int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
//          while(numNewMessages) {          
//              handleNewMessages(numNewMessages);
//              numNewMessages = bot.getUpdates(bot.last_message_received + 1);
//          }
//      bot_lasttime = millis();
//      }


    ///////////////////////////////////////
    // Функции для работы каждой кнопки //
   /////////////////////////////////////
            ReadButton("BUT1");   
            ReadButton("BUT2");
            ReadButton("BUT3");
            ReadButton("BUT4");
            ReadButton("BUT5");
            ReadButton("BUT6");
            ReadButton("BUT7");
            ReadButton("BUT8");
            ReadButton("BUT9");
  
    
}

void ReadButton(String but) {
    switch (but.substring(3).toInt()) {
      case 1:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D2); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 2:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D3); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 3:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D4); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 4:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D5); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 5:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D6); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 6:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D7); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 7:
        map_for_but[but.substring(3).toInt() - 1] = map(analogRead(A0), 0, 1024, 0, 1); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 8:
        map_for_but[but.substring(3).toInt() - 1] = digitalRead(D9); // считываем состояние кнопки и инвертируем сигнал
        break;
      case 9:
        map_for_but[but.substring(3).toInt() - 1] = map(analogRead(D10), 0, 1024, 0, 1); // считываем состояние кнопки и инвертируем сигнал
        break;
    }
    Serial.println(WiFi.status() == WL_CONNECTED);
    // Выполняем, если кнопка нажата
    if (map_for_but[but.substring(3).toInt() - 1] == 1 && map_for_flags[but.substring(3).toInt() - 1] == 0 && millis() - last_press > 50) {
        Serial.println("butt " + but.substring(3) + " is on");
        map_for_flags[but.substring(3).toInt() - 1] = 1; // Поднимаем флаг последнего состояния
        myMP3.play(but.substring(3).toInt()); // Воспроизводим соответствующее аудио
        bot.sendMessage(CHAT_ID, name_of_buttons[but.substring(3).toInt() - 1], ""); // Отправляем сообщение боту о нажатой кнопке
        last_press = millis();
    }
    // Выполняем, если кнопка не нажата
    if (map_for_but[but.substring(3).toInt() - 1] == 0 && map_for_flags[but.substring(3).toInt() - 1] == 1 && millis() - last_press > 50) {
      Serial.println("BUTT " + but.substring(3) + " IS OFF");
      map_for_flags[but.substring(3).toInt() - 1] = 0;
      last_press = millis();
     }
}


     //////////////////////////////////////
    // Функция для обработки сообщений //
   /////////////////////////////////////
//void handleNewMessages(int numNewMessages) {
//    for(int i = 0; i < numNewMessages; i++) {
//        if(bot.messages[i].chat_id == CHAT_ID) {
//            String text = bot.messages[i].text;
//            if(text == "/notsend") {
//                flag_for_sending = 0;
//                bot.sendMessage(CHAT_ID, "Отправка сообщений отключена", "");
//            }
//            if(text == "/send") {
//                flag_for_sending = 1;
//                bot.sendMessage(CHAT_ID, "Отправка сообщений включена", "");
//            }
//            if(text == "/start" || text == "/help") {
//                String welcome = "Hi i'm Tablet.\n";
//                welcome += "/send - Включить отправку уведомлений\n";
//                welcome += "/notsend - Отключить отправку уведомлений\n";
//                welcome += "/reg - Изменить названия кнопок\n";
//                bot.sendMessage(CHAT_ID, welcome, "");
//            }
//            if (text == "/reg") {
//                String registr = "Введите название каждой кнопки в таком формате: \n";
//                registr += "1 Мама\n2 Папа\n3 Хочу Виталия Евгеньевича\n4 Есть\n5 Пить\n6 Туалет\n7 Гулять\n8 Радость\n9 Злость";
//                bot.sendMessage(CHAT_ID, registr, "");
//            }
//            if (text.length() > 8) {
//                String name_1 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[0] = name_1;
//                
//                String name_2 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[1] = name_2;
//                
//                String name_3 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[2] = name_3;
//                
//                String name_4 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[3] = name_4;
//                
//                String name_5 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[4] = name_5;
//                
//                String name_6 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[5] = name_6;
//                
//                String name_7 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[6] = name_7;
//                
//                String name_8 = text.substring(0, text.indexOf("\n"));
//                text = text.substring(text.indexOf("\n") + 1);
//                name_of_buttons[7] = name_8;
//                
//                String name_9 = text;
//                name_of_buttons[8] = name_9;
//      
//            }
//        }
//    }
//}
