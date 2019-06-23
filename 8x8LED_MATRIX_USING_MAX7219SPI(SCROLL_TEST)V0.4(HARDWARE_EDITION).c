//-----------------------------------------------------------
/*
 Program    :- 8x8LED_Matrix_using_MAX7219SPI(Scroll_Test)V.04(Hardware Edition)
 Author     :- LK.Perera
 Controller :- PIC16F887
 Date       :- 2019.04.23
 Program.No :- ---
*/
//-----------------------------------------------------------
/*
Notes/Mentions :-
->This program helps to drive 8x8Mat with limited rom*Seriously bro....This chip only have 8k?
->If you use Proteus to simulate,Arrange pins Digits(0-7)->Mat(1-8) and Segments(A-Dp)->MatSegments(A-G) Respectively.
->Too much arrays crashes the program due to limited ram.
->So try to keep max 2 arrays whe using chips with 8k E2PROM.Use separate array for driving columns.
->Never drive the columns with for increment it will crash the program after column-4.Not suitable for text scrolling.
  But still usefull for small work.
->Trust me this scrolling text thing gives me a pain in the ass for weeks until i figured it out step by step through experiments.

Update/Fixed :-
->This source code tested on commercialy available Max7219 module.
->Always consider that proteus simulation vs real hardware gets opposit diraction of data mapping.
->simply if you want display "d" on hardware.you should program it "b" this way.


last Update :-
->Finally this code/algorithm sucsessfully scroll letters.
->Due to unavoidable glitchs and bugs i have to come up with new algorithm to fix this.
->In this algorithm i use only 8 increments in row.it means there are separate variables that increment in different stages(after 8 by 8).
->After make it in proper order,It will drives the data values in corresponding column slots without giving crashes.
->Always keep mind on column array ratio,and a value in for loop.Otherwise it will crash the code.
->Always use 0x00 in array instead of using empty slots.It will filter out unwanted popups in matrix display.
*/
//-----------------------------------------------------------
//All the Source code contains are own by the authour.All rights reserved.
//-----------------------------------------------------------
#define Load_Dir TRISC.F0 //Assign the SPI Direction.
#define Load_State PORTC.F0//Assign the Chip Select for SPI.
//This function set-up the Max7219 chip to its defaults.
void Max7219_Init(){
    Load_State = 0;//Select Max7219.
    SPI1_Write(0x09); //Select Decode-Mode Register.
    SPI1_Write(0x00);//Set Decode-Mode Register into no decode for 7-0.
    Load_State = 1;//Deselect Max7219.

    Load_State = 0;//Select Max7219.
    SPI1_Write(0x0A);//Select Intensity Register Format.
    SPI1_Write(0x0F);//Set Intensity Register Format into 31/32.
    Load_State = 1;//Deselect Max7219.

    Load_State = 0;//Select Max7219.
    SPI1_Write(0x0B);//Select Scan Limit Formant Register.
    SPI1_Write(0x07);//Set Scan Limit Register Format into 7 that display all digits.
    Load_State = 1;//Deselect Max7219.

    Load_State = 0;//Select Max7219.
    SPI1_Write(0x0C);//Select Shutdown Register.
    SPI1_Write(0x01);//Set Shutdown Register into Normal Operation.
    Load_State = 1;//Deselect Max7219.

    Load_State = 0;//Select Max7219.
    SPI1_Write(0x00);//Select No-Operation Register.
    SPI1_Write(0xFF);//Set No-Operation Register into Normal Operation.
    Load_State = 1;//Deselect Max7219.
}
//Custorm Function for clear the Matrix.
void Matrix_Clear(){
int i;//This variable use in for loop.
for(i=0;i<9;i++){
    Load_State=0;//Select Max7219.
    SPI1_Write(i);//For Selecting Digit Register.(In this case Digit7)
    SPI1_Write(0x00);//For Selecting A-DP.(In this case F)
    Load_State=1;//Deselect Max7219.
 }
}
/*
In Data1 array,
-> 1-2-2-1   = Used in Letter A.
-> 3-3-4     = Used in Letter L.
-> 4-5-6-7-4 = Used in Letter N.
-> 8-9-10-4  = Used in Letter K.
*/
//Keep This Array as common Array.Do all the modifications using this one.
int Data1[] = {0x00,0x1E,0x24,0x02,0x3E,0x04,0x08,0x10,
               0x22,0x14,0x08,};

//Always keep mind about 0x00 ratio in the colum array.In this case 29 0x00's.
//Dont mess with this array. Update it when you changing the for loop values(a).
int Column[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,0x00,0x00};

//Custorm Function for scroll Matrix right side.
void Matrix_Scroll_Right(){
int a=0;//Use in for loop.
int cnt=0;//This varianle separatly use for counting purposes instead of for loop increment.
int cnt1=0;//This variable use for second stage in display.
int cnt2=0;//This variable use for third stage in displey.
int cnt3=0;//This variable use for forth stage in displey.
int Phase_one=0;//This variable use to identify the end of the first stage data.if this will turn into 1,the second stage is starting.
int Phase_two=0;//This variable use to identify the end of the Second stage data.if this will turn into 1,the second stage is starting.
int Phase_three=0;//This variable use to identify the end of the third stage data.if this will turn into 1,the second stage is starting.
//--------------------
//In this method i tried up to column-8 values and it works like a dream.But remember array should be twice the sice of the increment value.
//Each comment line wil separate the letters.
for(a;a<38;a++){
//--------------------------------------------------------------------------------------------------------------------------K
  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt]);//This will determine the Column value.
  SPI1_Write(Data1[4]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-1]);//This will determine the Column value.
  SPI1_Write(Data1[10]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-2]);//This will determine the Column value.
  SPI1_Write(Data1[9]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-3]);//This will determine the Column value.
  SPI1_Write(Data1[8]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.
//--------------------------------------------------------------------------------------------------------------------------A
  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-5]);//This will determine the Column value.
  SPI1_Write(Data1[1]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-6]);//This will determine the Column value.
  SPI1_Write(Data1[2]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-7]);//This will determine the Column value.
  SPI1_Write(Data1[2]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt-8]);//This will determine the Column value.
  SPI1_Write(Data1[1]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.
//--------------------------------------------------------------------------------------------------------------------------L
  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1]);//This will determine the Column value.
  SPI1_Write(Data1[4]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1-1]);//This will determine the Column value.
  SPI1_Write(Data1[3]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1-2]);//This will determine the Column value.
  SPI1_Write(Data1[3]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.
//--------------------------------------------------------------------------------------------------------------------------A
  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1-4]);//This will determine the Column value.
  SPI1_Write(Data1[1]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1-5]);//This will determine the Column value.
  SPI1_Write(Data1[2]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1-6]);//This will determine the Column value.
  SPI1_Write(Data1[2]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt1-7]);//This will determine the Column value.
  SPI1_Write(Data1[1]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.
//----------------------------------------------------------------------------------------------------------------------------N
  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2]);//This will determine the Column value.
  SPI1_Write(Data1[4]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-1]);//This will determine the Column value.
  SPI1_Write(Data1[7]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-2]);//This will determine the Column value.
  SPI1_Write(Data1[6]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-3]);//This will determine the Column value.
  SPI1_Write(Data1[5]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-4]);//This will determine the Column value.
  SPI1_Write(Data1[4]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.
//--------------------------------------------------------------------------------------------------------------------------A
  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-6]);//This will determine the Column value.
  SPI1_Write(Data1[1]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-7]);//This will determine the Column value.
  SPI1_Write(Data1[2]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt2-8]);//This will determine the Column value.
  SPI1_Write(Data1[2]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.

  Load_State=0;//Select Max7219.
  SPI1_Write(Column[cnt3]);//This will determine the Column value.
  SPI1_Write(Data1[1]);//This will determine the data value Respectively to Column value.
  Load_State=1;//Deselect Max7219.
//-------------------------------------------------------------------------------------------------------------------------------

//Below part contains the Display logic.
  cnt++;//Increment of the cnt variable.
  delay_ms(500);//delay function.This will determine the speed of the multiplex/animation/scroll.
  Matrix_Clear();//Matrix clear Function.
if(cnt==11){//The equal value is determine by the end of the first stage data+1 column space.In this case it is 12.
  Phase_one=1;//This make Phase_one variable equals to one.It identify first stage and starts to increment.
}
if(Phase_one==1){//When Phase_one equals 1.
  cnt1++;//Increment of the cnt1 variable/First stage.
}
if(cnt1==10){//The equal value is determine by the end of the first stage data+1 column space.In this case it is 12.
  Phase_two=1;//This make Phase_two variable equals to one.It identify second stage and starts to increment.
}
if(Phase_two==1){//When Phase_two equals to 1.
  cnt2++;//Increment of cnt2 variable/Second stage.
}
if(cnt2==10){
   Phase_three=1;
}
if(Phase_three==1){//When Phase_two equals to 1.
  cnt3++;//Increment of cnt2 variable/Third stage.
}
//----------------------------------------Function-End----
 }
}
void main(){
ANSEL = 0;//Make all GPIO=Digital
ANSELH = 0;//Make all GPIO=Digital
C1ON_bit = 0;//Disable the Comparator 1
C2ON_bit = 0;//Disable the Comparator 2
Load_Dir = 0x00;//Set the Load_ pin direction as O/p (TRISC.F0)
SPI1_Init();//Initialise Buit-in SPI routine
Max7219_Init();//Initialise custom built Max7219 driver
while(1){
    Matrix_Scroll_Right();//Scroll matrix left to right.
    Matrix_Clear();//Clear the matrix
    delay_ms(500);//Add this delay to avoid crashes.
    //Matrix_Scroll_Left();//Scroll the matrix right to left.
    //delay_ms(500);
 }
}