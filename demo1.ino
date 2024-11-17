
/*  7 segment LCD serial control
   eddy Electronics
   pin definition
   
*/

#define CS 13   //Pin 13 as chip selection output
#define WR 12   //Pin 12 as read clock  output
#define DATA 8  //Pin 7 as Serial data output

#define CS1 digitalWrite(CS, HIGH)
#define CS0 digitalWrite(CS, LOW)
#define WR1 digitalWrite(WR, HIGH)
#define WR0 digitalWrite(WR, LOW)
#define DATA1 digitalWrite(DATA, HIGH)
#define DATA0 digitalWrite(DATA, LOW)


#define sbi(x, y) (x |= (1 << y))
#define cbi(x, y) (x &= ~(1 << y))
#define uchar unsigned char
#define uint unsigned int

//定义HT1621的命令
#define ComMode 0x52  //4COM,1/3bias  1000    010 1001  0
#define RCosc 0x30    //1000 0011 0000
#define LCD_on 0x06   //1000     0000 0 11 0
#define LCD_off 0x04
#define Sys_en 0x02  //1000   0000 0010
#define CTRl_cmd 0x80
#define Data_cmd 0xa0


/*0,1,2,3,4,5,6,7,8,9,A,b,C,c,d,E,F,H,h,L,n,N,o,P,r,t,U,-, ,*/

const char num[] = {
  0b11111010,       //0
  0b00001010,       //1
  0b10111100,       //2
  0b10011110,       //3
  0b01001110,       //4
  0b11010110,       //5
  0b11110110,       //6
  0b10001010,       //7
  0b11111110,       //8
  0b11011110,       //9
  0b00100110,       //11 :a
  0b01110110,       //11 :b
  0b11110000,       //11 :c
  0b00111110,       //11 :d
  0b11110100,       //11 :e
  0b11100100,       //11 :f
  0b00000001,       //10 :dot
};
//0      1      2      3        4      5      6       7       8       9
char dispnum[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int i = 0;

void SendBit_1621(uchar sdata, uchar cnt) {
  //data cnt HT1621
  uchar i;
  for (i = 0; i < cnt; i++) {
    WR0;
    if (sdata & 0x80) DATA1;
    else DATA0;
    WR1;
    sdata <<= 1;
  }
}

void SendCmd_1621(uchar command) {
  CS0;
  SendBit_1621(0x80, 4);
  SendBit_1621(command, 8);
  CS1;
}

void Write_1621(uchar addr, uchar sdata) {
  addr <<= 2;
  CS0;
  SendBit_1621(0xa0, 3);
  SendBit_1621(addr, 6);
  SendBit_1621(sdata, 8);
  CS1;
}

void HT1621_all_off(uchar num) {
  uchar i;
  uchar addr = 0;
  for (i = 0; i < num; i++) {
    Write_1621(addr, 0x00);
    addr += 2;
  }
}

void HT1621_all_on(uchar num) {
  uchar i;
  uchar addr = 0;
  for (i = 0; i < num; i++) {
    Write_1621(addr, 0xff);
    addr += 2;
  }
}

void Init_1621(void) {
  SendCmd_1621(Sys_en);
  SendCmd_1621(RCosc);
  SendCmd_1621(ComMode);
  SendCmd_1621(LCD_on);
}

void displaydata(int p) {
  uchar i = 0;
  switch (p) {
    case 1:
      sbi(dispnum[0], 7);
      break;
    case 2:
      sbi(dispnum[1], 7);
      break;
    case 3:
      sbi(dispnum[2], 7);
      break;
    default: break;
  }
  for (i = 0; i <= 5; i++) {
    Write_1621(i * 2, dispnum[i]);
  }
}



void setup() {
  Serial.begin(9600);     // open the serial port at 9600 bps:
  pinMode(CS, OUTPUT);    //
  pinMode(WR, OUTPUT);    //
  pinMode(DATA, OUTPUT);  //
  CS1;
  DATA1;
  WR1;
  delay(50);
  Init_1621();
   HT1621_all_on(16);  //1621
   delay(500);
   HT1621_all_off(16);  //1621
   delay(500);

}

void loop() {

   while (i < sizeof(num)) {
  //   Serial.println(num[i]);  // prints a label
   Write_1621(0,num[i]);
    Write_1621(2,num[i]);
    Write_1621(4,num[i]);
    Write_1621(6,num[i]);
    Write_1621(8,num[i]);
    Write_1621(10,num[i]);
    Write_1621(12,num[i]);
    Write_1621(14,num[i]);
     delay(500);
     i = i + 1;
   }
   i = 0;
}
