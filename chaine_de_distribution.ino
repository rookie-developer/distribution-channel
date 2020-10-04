#include <avr/io.h>
#include <util/delay.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define pirOff        (!(PIND & (1<<3)))
#define pirOn         (PIND & (1<<3))
#define MOTOR1_ON     PORTD|=(1<<PD6)
#define MOTOR2_ON     PORTD|=(1<<PD7)
#define MOTOR1_OFF    PORTD&=~(1<<PD6)
#define MOTOR2_OFF    PORTD&=~(1<<PD7)

int seconds=0,minutes=0,hours=0,var=0;

Servo myservo;
LiquidCrystal lcd(13,12,11,10,9,8);

/// just a silly intro ////
void intro()
{
  lcd.setCursor(0,0);
  lcd.print("PROJECT BY :");
  _delay_ms(500);
  lcd.setCursor(0,1);
  lcd.print("Rookie_developer");
  _delay_ms(500);
  lcd.clear();
  _delay_ms(500);
}

/// the library sweep uses timer1 for pwm signals so we can't use timer1 ///
void sweep()
{
  int pos = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("removing junk");
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    _delay_ms(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    _delay_ms(15);                       // waits 15ms for the servo to reach the position
  }
  lcd.clear();
}

//// motor on mode ////
void on()
{
  MOTOR1_ON;
  MOTOR2_ON;
}

//// motor off mode ////
void off()
{
  MOTOR1_OFF;
  MOTOR2_OFF;
}

//// affichage sur lcd /////
void affichage()
{
    lcd.setCursor(0,0);
    lcd.print("channel is on");
    lcd.setCursor(0,1);
    lcd.print("time:");
    lcd.setCursor(13,1);
    lcd.println(seconds);
    lcd.setCursor(10,1);
    lcd.println(minutes);
    lcd.setCursor(7,1);
    lcd.println(hours);
    lcd.setCursor(9,1);
    lcd.print(":");
    lcd.setCursor(12,1);
    lcd.print(":");
}

int main(void)
{
  cli();
  myservo.attach(0);
  lcd.begin(16,4);
  DDRD=0xC0;
  intro();
  //// here i unable necessary bits and registeres to put timer2 on CTC mode /////
  TCCR2A=0b00000010;
  TCCR2B=0b00000110;   // prescaler 256
  TIMSK2=0b00000010;
  TCNT2=0;
  OCR1A=255;
  sei();
  affichage();
  while(1)
  {
    on();
    if(pirOn)
    {
      off();
      sweep();
    }
  }
}

/// routine d'interruptions en utilisent le timer2 mode CTC ////
ISR(TIMER2_COMPA_vect)
{
  var++;
  if(var>10000){
    affichage();
    var=0;
  if(seconds++==60)
  {
    affichage();
    seconds=0;
    if(minutes++==60)
    {
      affichage();
      minutes=0;
      if(hours++==24)
      {
        affichage();
        hours=0;
      }
    }
  }
  }
}  ///// code is in github repos ////
