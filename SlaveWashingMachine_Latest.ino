const int fillPump = 2;
const int drainPump = 3;

String command = "";

void setup() {
  Serial.begin(9600);

  pinMode(fillPump, OUTPUT);
  pinMode(drainPump, OUTPUT);

  digitalWrite(fillPump, HIGH);   // OFF (relay is active LOW)
  digitalWrite(drainPump, HIGH);  // OFF

  Serial.println("Relay Slave Ready");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      Serial.print("Received: ");
      Serial.println(command);
      handleCommand(command);
      command = "";
    } else {
      command += c;
    }
  }
}

void handleCommand(String cmd) {
  cmd.trim();

  if (cmd == "FILL_ON") {
    digitalWrite(fillPump, LOW);  // Relay ON
    Serial.println("FILL pump ON");
  } 
  else if (cmd == "FILL_OFF") {
    digitalWrite(fillPump, HIGH); // Relay OFF
    Serial.println("FILL pump OFF");
  } 
  else if (cmd == "DRAIN_ON") {
    digitalWrite(drainPump, LOW); // Relay ON
    Serial.println("DRAIN pump ON");
  } 
  else if (cmd == "DRAIN_OFF") {
    digitalWrite(drainPump, HIGH); // Relay OFF
    Serial.println("DRAIN pump OFF");
  } 
  else {
    Serial.println("Unknown command");
  }
}