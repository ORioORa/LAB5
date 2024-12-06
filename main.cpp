#include <Arduino.h>
#include <TM1638plus.h>

#define STB 10
#define CLK 9
#define DIO 8
TM1638plus tm(STB,CLK,DIO);

int firstOperand = 0;        // ตัวเลขตัวแรก
int secondOperand = 0;       // ตัวเลขตัวที่สอง
char operation = '\0';       // ตัวดำเนินการ (+, -)
bool isSecondOperand = false; // ตรวจสอบว่ากำลังกรอกตัวเลขตัวที่สอง
bool clearFlag = false;      // ใช้สำหรับตรวจสอบปุ่ม "Clear"


void setup() {
  Serial.begin(9600);
  tm.displayBegin();
  tm.reset();

  // put your setup code here, to run once:
}


 void loop() {
  byte buttons = tm.readButtons(); // อ่านสถานะปุ่ม

    // Clear button (Button 8)
    if (buttons & 0b10000000) {
        firstOperand = 0;
        secondOperand = 0;
        operation = '\0';
        isSecondOperand = false;
        tm.displayText("CLEAR"); // แสดงข้อความ "CLEAR"
        delay(500);
        tm.reset();
        return;
    
}
    for (int i = 0; i < 4; i++) {
        if (buttons & (1 << i)) { // ตรวจสอบว่าปุ่มหมายเลขใดถูกกด
            int num = i + 1; // ปุ่ม 1-4 หมายถึงตัวเลข 1-4
            if (!isSecondOperand) {
                firstOperand = firstOperand * 10 + num; // ป้อนตัวเลขแรก
                tm.displayIntNum(firstOperand, false); // แสดงผลตัวเลขแรก
            } else {
                secondOperand = secondOperand * 10 + num; // ป้อนตัวเลขที่สอง
                tm.displayIntNum(secondOperand, false); // แสดงผลตัวเลขที่สอง
            }
            delay(300); // หน่วงเวลาเพื่อหลีกเลี่ยงการกดซ้ำ
        }
    }

if (buttons & 0b00100000) { // Button 5 for Addition
        operation = '+';
        isSecondOperand = true;
        tm.displayText("PLUS"); // แสดงเครื่องหมายบวก
        delay(500);
    }
    if (buttons & 0b01000000) { // Button 6 for Subtraction
        operation = '-';
        isSecondOperand = true;
        tm.displayText("MINUS"); // แสดงเครื่องหมายลบ
        delay(500);
    }

    // Equals button (Button 7)
    if (buttons & 0b00010000) {
        int result = 0;

        // Perform the selected operation
        if (operation == '+') {
            result = firstOperand + secondOperand; // คำนวณบวก
        } else if (operation == '-') {
            result = firstOperand - secondOperand; // คำนวณลบ
        }

        // Display the result
        tm.displayIntNum(result, false);

        // Reset variables for a new calculation
        firstOperand = result;
        secondOperand = 0;
        operation = '\0';
        isSecondOperand = false;

        delay(1000); // หน่วงเวลาเพื่อให้ผู้ใช้เห็นผลลัพธ์
    }
}

