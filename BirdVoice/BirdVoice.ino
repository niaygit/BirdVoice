//加速度センサが基準とする0gの値
const int XG0 = 756; //基本姿勢の関係でXの0gの値が大きい。
const int YG0 = 511; 
const int ZG0 = 495; 

int mode, Do, Re, Mi, Fa, So, preX, preY, preZ;
float degX, degY, degZ;
bool check, b4, b6, b7, b8, b10;

void setup() {
  Serial.begin(9600);
  //ピンモード
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  //D5ピンはうまく反応しなかったため使用せず
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  mode = 0;//ドレミ演奏と鳥のさえずりの切り替え
  check = false;//切り替えボタンが押されたことの判定用

  //各音のボタンが押されたことの判定
  b4 = false;
  b6 = false;
  b7 = false;
  b8 = false;
  b10 = false;

  //音程
  Do = 1046;
  Re = 1170;
  Mi = 1318;
  Fa = 1369;
  So = 1567;
   
 }

//加速度センサの値を取得する関数
void kakudo(int x, int y, int z){
  //ローパスフィルタ
  int nowX = preX * 0.9 + x * 0.1;
  int nowY = preY * 0.9 + y * 0.1;
  int nowZ = preZ * 0.9 + z * 0.1;
  preX = nowX;
  preY = nowY;
  preZ = nowZ;
  degX = degrees(atan(nowX / (float)nowZ));
  degY = degrees(atan(nowY / (float)nowZ));
}

void loop() {
  int change = digitalRead(3);
  int di4 = digitalRead(4);
  int di6 = digitalRead(6);
  int di7 = digitalRead(7);
  int di8= digitalRead(8);
  int di10 = digitalRead(10);

  //ボタンを離した瞬間にドレミ演奏と鳥の鳴き声の切り替え
  if(check == false && change == LOW){
    check = true;
  }else if(check == true && change == HIGH){
    switch(mode){
      case 0:
        mode = 1;
        break;
      case 1:
        mode = 0;

        break;
  }
    check = false;
    Serial.println(mode);
  }

  //ドレミ
  //それぞれ音が鳴る前にそれよりも少し低い音から目標までゆっくりと高くしていき、ボタンを離すと反対にゆっくりと低くしてから消音することで、音の立ち上がりと余韻を表現した。
  
  if(mode == 0){
    //赤色LEDを点灯
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    //ド
    if(di4 == LOW && b4 == false){
      Serial.println(mode);
      Serial.println(b7);
      b4 = true;
        for(int i = Do - 70;i<Do;i++){
          tone(9, i);
          delay(2);
        }
    }
    if(di4 == LOW && b4 == true){
      tone(9, Do);
      delay(2);
    }
    if(di4 == HIGH && b4 == true){
      for(int i = Do;i>Do-70;i--){
        tone(9, i);
        delay(2);
      }
      noTone(9);
      b4 = false;
    }

    //レ
    if(di6 == LOW && b6 == false){
      b6 = true;
        for(int i = Re - 70;i<Re;i++){
          tone(9, i);
          delay(2);
        }
      tone(9, Re);
      delay(1000);
    }
    if(di6 == HIGH && b6 == true){
      for(int i = Re;i>Re-70;i--){
        tone(9, i);
        delay(2);
      }
      noTone(9);
      b6 = false;
    }
    
    //ミ
    
    if(di7 == LOW && b7 == false){
      b7 = true;
        for(int i = Mi - 70;i<Mi;i++){
          tone(9, i);
          delay(2);
        }
      tone(9, Mi);
      delay(1000);
    }
    if(di7 == HIGH && b7 == true){
      for(int i = Mi;i>Mi-70;i--){
        tone(9, i);
        delay(2);
      }
      noTone(9);
      b7 = false;
    }
    //ファ
    if(di8 == LOW && b8 == false){
      b8 = true;
        for(int i = Fa - 70;i<Fa;i++){
          tone(9, i);
          delay(2);
        }
      tone(9, Fa);
      delay(1000);
    }
    if(di8 == HIGH && b8 == true){
      for(int i = Fa;i>Fa-70;i--){
        tone(9, i);
        delay(2);
      }
      noTone(9);
      b8 = false;
    }
    //ソ
    if(di10 == LOW && b10 == false){
      b10 = true;
        for(int i = So - 70;i<So;i++){
          tone(9, i);
          delay(2);
        }
      tone(9, So);
      delay(1000);
    }
    if(di10 == HIGH && b10 == true){
      for(int i = So;i>So-70;i--){
        tone(9, i);
        delay(2);
      }
      noTone(9);
      b10 = false;
    }
}
  // 鳥の鳴き声
  else if(mode == 1){
    //黄色LEDを点灯
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);

    //for文によって音を連続的に変化させることで、鳥の鳴き声の響きを表現した
    //音の立ち上がりをボタンを押した瞬間のみ再現するため、ボタンが押されたときをbool値のb4、b6、b7、b8、b10で判定する
    //kakudo関数で傾きを検出し、音の高低を変化させることで、鳥が巣箱の中で動き回りながら鳴く様子を表現した

    //シジュウカラ
    if(di4 == LOW && b4 == false){
      b4 = true;
      while(di4 == LOW){
        kakudo(analogRead(3) - XG0, analogRead(4) - YG0, analogRead(5) - ZG0);
        di4 = digitalRead(4);
        for(int i = 2500;i<3000;i+=10){
          tone(9, i - degX * 10 - degY * 10);
          delay(1);
        }
        for(int i = 2000;i<2500;i+=4){
          tone(9, i - degX * 10 - degY * 10);
          delay(2);
        }
        noTone(9);
        delay(10);
      }
    }
    if(di4 == HIGH && b4 == true){
      noTone(9);
      b4 = false;
    }
    //ヒヨドリ
    if(di6 == LOW && b6 == false){
      b6 = true;
      while(di6 == LOW){
        kakudo(analogRead(3) - XG0, analogRead(4) - YG0, analogRead(5) - ZG0);
        di6 = digitalRead(4);
        for(int i = 1000;i<1200;i+=2){
          tone(9, i - degX * 10 - degY * 10);
          delay(1);
        }
        noTone(9);
        delay(200);
        for(int i = 2000;i<2200;i+=2){
          tone(9, i - degX * 10 - degY * 10);
          delay(2);
        }
        noTone(9);
        delay(200);
        for(int i = 2000;i<2200;i+=2){
          tone(9, i - degX * 10 - degY * 10);
          delay(2);
        }
        noTone(9);
        delay(200);
      }
    }

    if(di6 == HIGH && b6 == true){
      noTone(9);
      b6 = false;
    }

    //メジロ
    if(di7 == LOW && b7 == false){
      b7 = true;
      while(di7 == LOW){
        kakudo(analogRead(3) - XG0, analogRead(4) - YG0, analogRead(5) - ZG0);
        di7 = digitalRead(4);
        for(int j = 0;j<3;j++){
          for(int i = 1800;i<2000;i+=5){
            tone(9, i - degX * 10 - degY * 10);
            delay(10);
          }
        noTone(9);
        delay(random(21, 501));        
      }
      }
    }
    
    if(di7 == HIGH && b7 == true){
      noTone(9);
      b7 = false;
    }
    
    //ホオジロ
    if(di8 == LOW && b8 == false){
      b8 = true;
      while(di8 == LOW){
        kakudo(analogRead(3) - XG0, analogRead(4) - YG0, analogRead(5) - ZG0);
        di8 = digitalRead(4);
        for(int i = 2000;i>1800;i-=10){
          tone(9, i  - degX * 10 - degY * 10);
          delay(5);
        }
        noTone(9);
        delay(100);
        for(int i = 2200;i>2000;i-=15){
          tone(9, i  - degX * 10 - degY * 10);
          delay(5);
        }
        noTone(9);
        delay(5);
        for(int i = 2200;i>2000;i-=15){
          tone(9, i  - degX * 10 - degY * 10);
          delay(5);
        }
        noTone(9);
        delay(100);
        for(int j = 0;j<7;j++){
          for(int i = 3000;i>1000;i-=100){
            tone(9, i  - degX * 10 - degY * 10);
            delay(1);
        }
        }
        noTone(9);
        delay(100);
      }
    }

    if(di8 == HIGH && b8 == true){
      noTone(9);
      b8 = false;
    }


    //ヤマゲラ
    if(di10 == LOW && b10 == false){
      b10 = true;
      while(di10 == LOW){
        kakudo(analogRead(3) - XG0, analogRead(4) - YG0, analogRead(5) - ZG0);
        di10 = digitalRead(8);
        for(int j = 0;j<3;j++){
          for(int i = 2000;i<2200;i+=10){
          tone(9, i  - degX * 10 - degY * 10);
          delay(7);
        }
        }
        noTone(9);
        delay(50);
        for(int i = 1950;i<2000;i+=1){
            tone(9, i  - degX * 10 - degY * 10);
            delay(1);
        }
        for(int i = 2000;i>1900;i-=5){
            tone(9, i  - degX * 10 - degY * 10);
            delay(5);
          }
        noTone(9);
        delay(100);
      }
    }

    if(di10 == HIGH && b10 == true){
      noTone(9);
      b10 = false;
    }
}
}
