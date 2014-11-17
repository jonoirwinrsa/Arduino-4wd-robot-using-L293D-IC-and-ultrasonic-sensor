void driveForwards(){
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
    digitalWrite(controlPin3, HIGH);
    digitalWrite(controlPin4, LOW);
}

void driveBackwards(String direct){
  if(direct == "left"){
    analogWrite(enablePin1, 0);
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }
  else if (direct == "right"){
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
    digitalWrite(controlPin3, HIGH);
    digitalWrite(controlPin4, LOW);
  }
  else{
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, HIGH);
  }
}

void stopDriving(int time){
    analogWrite(enablePin, 0);
    analogWrite(enablePin1, 0);
    delay(time);
    analogWrite(enablePin, 1);
    analogWrite(enablePin1, 1);
}
