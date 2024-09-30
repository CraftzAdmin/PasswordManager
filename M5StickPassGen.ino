/*
 *  M5StickC Plus Password Generator
 *  --------------------------------
 * 
 *  Description: 
 *  This project is a secure, BLE-enabled password generator using the M5StickC Plus device. 
 *  It allows users to generate consistent, unique passwords based on their selected Bank, 
 *  a 4-digit PIN, and a chosen password length. The device generates passwords using 
 *  SHA256 hashing of the device ID and user input, sending the passwords via BLE 
 *  to connected devices such as laptops and smartphones.
 * 
 *  License:
 *  This project is licensed under the Creative Commons Attribution-ShareAlike 4.0 International 
 *  (CC BY-SA 4.0) license. You are free to share, adapt, and build upon this work, even for 
 *  commercial purposes, as long as you give appropriate credit and distribute your contributions 
 *  under the same license.
 *  For full license details, refer to: https://creativecommons.org/licenses/by-sa/4.0/
 * 
 *  Disclaimer:
 *  This project is provided as-is, with no warranties or guarantees of any kind. Use at your 
 *  own risk. The creator assumes no responsibility for any damages, security breaches, or data loss 
 *  caused by using this project. Ensure you understand the security implications and use this 
 *  device in a manner that fits your needs.
 * 
 *  Author:
 *  Craftz - Daniel Brunod
 * 
 *  Version: 
 *  1.0
 * 
 * 
 *  Notes:
 *  - Make sure to modify the 'deviceID' and 'initialPin' values before compiling.
 *  - BLE Keyboard functionality only starts after entering the correct 4-digit PIN.
 *  - Known bug: Windows 11 BLE reconnection issue after disconnection requires re-pairing.
 * 
 */


#include <M5StickCPlus.h>
#include <BleKeyboard.h>
#include <SHA256.h>
#include <Preferences.h>
#include <string>
#include <esp_bt_defs.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>
#include <esp_bt_device.h>
#include <esp_err.h>

BleKeyboard bleKeyboard("M5Keyboard", "Craftz", 100);
Preferences preferences;

const char* PREF_NAMESPACE = "password_mgr";
const char* PREF_PIN = "unlock_pin";
const char* PREF_LENGTH = "pwd_length";
const char* PREF_BRIGHTNESS = "brightness";

// Variáveis pré-definidas
const String deviceID = "5c6560bca85c943f0006dd3d702f8bcc9f25f0e8c710fb908c32bab2984fc69a"; // Exemplo de SHA256 device ID
const String initialPin = "1234"; // PIN inicial padrão

int unlockAttempts = 0;
int passwordLength = 12;  // Valor padrão
int tempPasswordLength = 12;  // Tamanho temporário
String bank = "A";
String inputPin = "";
int defaultBrightness = 80;
bool bleConnected = false;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1); // Modo paisagem
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  // Inicializar BLE
  bleKeyboard.begin();

  // Limpar informações de pareamento (bonding) a cada inicialização
  clearBondingInformation();

  // Inicializar Preferences
  preferences.begin(PREF_NAMESPACE, false);
  if (!preferences.isKey(PREF_PIN)) {
    preferences.putString(PREF_PIN, initialPin); // Armazena o PIN inicial se não estiver definido
  }

  // Exibe a tela inicial
  startupScreen();
}

void loop() {
  if (bleConnected) {
    displayMainMenu();
  } else {
    unlockDevice();
  }
}

// Função para limpar informações de pareamento (bonding)
void clearBondingInformation() {
  esp_ble_bond_dev_t bondDevices[10];
  int dev_num = esp_ble_get_bond_device_num();
  if (dev_num) {
    esp_ble_get_bond_device_list(&dev_num, bondDevices);
    for (int i = 0; i < dev_num; i++) {
      esp_ble_remove_bond_device(bondDevices[i].bd_addr);
    }
  }
}

// Exibe a tela inicial
void startupScreen() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 30);
  M5.Lcd.print("Password Manager");
  M5.Lcd.setCursor(60, 80);
  M5.Lcd.print("by Craftz");
  M5.Lcd.setCursor(30, 120);
  M5.Lcd.print("    Press A");

  while (true) {
    M5.update();
    if (M5.BtnA.wasPressed()) {
      break;
    }
  }
  unlockDevice();
}

// Desbloqueia o dispositivo com o PIN inicial e conecta ao BLE
void unlockDevice() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(40, 50);
  M5.Lcd.print("Enter PIN:");

  inputPin = "";
  int pinLength = 0;
  int pinDigit = 0;
  bool confirmDigit = false;

  while (pinLength < 4) {
    M5.update();

    if (M5.BtnB.wasPressed() && !confirmDigit) {
      pinDigit++;
      if (pinDigit > 9) pinDigit = 0;
      M5.Lcd.fillRect(60 + pinLength * 10, 80, 10, 20, BLACK); // Limpa o número anterior
      M5.Lcd.setCursor(60 + pinLength * 10, 80);
      M5.Lcd.print(pinDigit); // Mostra o dígito atual
    }

    if (M5.BtnA.wasPressed() && !confirmDigit) {
      // Confirma o dígito e exibe o asterisco
      inputPin += String(pinDigit);
      M5.Lcd.fillRect(60 + pinLength * 10, 80, 10, 20, BLACK); // Limpa o número
      M5.Lcd.setCursor(60 + pinLength * 10, 80);
      M5.Lcd.print("*");
      pinLength++;
      confirmDigit = true;
    }

    if (confirmDigit) {
      pinDigit = 0;  // Reseta para o próximo dígito
      confirmDigit = false;
    }

    if (pinLength == 4 && M5.BtnA.wasPressed()) {
      // Verifica o PIN
      if (checkPin(inputPin)) {
        unlockAttempts = 0;
        waitForBLEConnection();
        return;
      } else {
        unlockAttempts++;
        if (unlockAttempts >= 3) {
          lockout();
        }
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(40, 50);
        M5.Lcd.print("Wrong PIN!");
        delay(2000);
        unlockDevice();
      }
    }
  }
}

// Espera a conexão BLE após desbloquear o dispositivo
void waitForBLEConnection() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 50);
  M5.Lcd.print("Waiting for BLE...");
  
  // Loop até a conexão BLE
  while (!bleKeyboard.isConnected()) {
    M5.update();
    if (M5.BtnA.wasPressed()) {
      bleConnected = true;
      break;
    }
  }
  
  if (bleKeyboard.isConnected()) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.print("Connected!");
    delay(2000);
    bleConnected = true;
  }
}

// Verifica se o PIN inserido corresponde ao armazenado nas Preferences
bool checkPin(String pin) {
  String storedPin = preferences.getString(PREF_PIN, initialPin);
  return pin.equals(storedPin);
}

// Bloqueia o usuário após 3 tentativas erradas
void lockout() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(40, 50);
  M5.Lcd.print("Locked out!");
  delay(60000);  // Bloqueia por 1 minuto
  unlockAttempts = 0;
  unlockDevice();
}

// Exibe o menu principal
void displayMainMenu() {
  M5.Lcd.fillScreen(BLACK);
  int menuChoice = 0;
  displayMenuOption(menuChoice);

  while (true) {
    M5.update();
    
    if (M5.BtnB.wasPressed()) {
      menuChoice = (menuChoice + 1) % 5; // Volta ao primeiro menu após o último
      displayMenuOption(menuChoice);
    }

    if (M5.BtnA.wasPressed()) {
      switch (menuChoice) {
        case 0: generatePassword(); break;
        case 1: setPasswordLength(); break;
        case 2: setBrightness(); break;
        case 3: checkPowerStatus(); break;
        case 4: shutdownDevice(); break;
      }
    }
  }
}

// Mostra as opções do menu na tela
void displayMenuOption(int menuChoice) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(20, 50);
  switch (menuChoice) {
    case 0: M5.Lcd.print("1. Gen Psswrd"); break;
    case 1: M5.Lcd.print("2. Psswrd Size"); break;
    case 2: M5.Lcd.print("3. Brightness"); break;
    case 3: M5.Lcd.print("4. Power Status"); break;
    case 4: M5.Lcd.print("5. Exit"); break;
  }
}

// Gera uma senha com base no Bank e no PIN inicial + DeviceID + Password PIN
void generatePassword() {
  // Seleção do Bank
  char bank = 'A';
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("Bank (A-Z):");
  M5.Lcd.setCursor(50, 80);
  M5.Lcd.print(bank);

  // Lógica para selecionar o Bank
  while (true) {
    M5.update();
    if (M5.BtnB.wasPressed()) {
      bank++;
      if (bank > 'Z') bank = 'A';
      M5.Lcd.fillRect(50, 80, 240, 20, BLACK); // Limpa o valor anterior
      M5.Lcd.setCursor(50, 80);
      M5.Lcd.print(bank);
    }
    if (M5.BtnA.wasPressed()) {
      break;
    }
  }

  // Inserir o PIN de 4 dígitos para a geração da senha
  String passwordPin = enterPasswordPin();

  // Definir o tamanho da senha
  setTemporaryPasswordLength();

  // Gera e exibe a senha usando o PIN inicial, deviceID e passwordPin
  String password = sha256GeneratePassword(bank, preferences.getString(PREF_PIN, initialPin), passwordPin); 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 40);
  M5.Lcd.print("Password:");
  M5.Lcd.setCursor(30, 70);
  M5.Lcd.print(password);

  // Exibe a força da senha
  M5.Lcd.setCursor(30, 110);
  if (isStrongPassword(password)) {
    M5.Lcd.print("Strong");
  } else {
    M5.Lcd.print("Weak");
  }

  while (true) {
    M5.update();
    if (M5.BtnA.wasPressed()) {
      // Envia a senha pelo BLEKeyboard, um caractere por vez com intervalo de 50ms
      for (int i = 0; i < password.length(); i++) {
        bleKeyboard.print(password[i]);
        delay(50);
      }
    }
    if (M5.BtnB.wasPressed()) {
      displayMainMenu();
      return;
    }
  }
}

// Insere o PIN de 4 dígitos para a geração da senha
String enterPasswordPin() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("4-dig. PIN:");
  
  String passwordPin = "";
  int pinLength = 0;
  int pinDigit = 0;
  bool confirmDigit = false;

  while (pinLength < 4) {
    M5.update();

    if (M5.BtnB.wasPressed() && !confirmDigit) {
      pinDigit++;
      if (pinDigit > 9) pinDigit = 0;
      M5.Lcd.fillRect(60 + pinLength * 10, 80, 10, 20, BLACK); // Limpa o número anterior
      M5.Lcd.setCursor(60 + pinLength * 10, 80);
      M5.Lcd.print(pinDigit); // Mostra o dígito atual
    }

    if (M5.BtnA.wasPressed() && !confirmDigit) {
      // Confirma o dígito e exibe o asterisco
      passwordPin += String(pinDigit);
      M5.Lcd.fillRect(60 + pinLength * 10, 80, 10, 20, BLACK); // Limpa o número
      M5.Lcd.setCursor(60 + pinLength * 10, 80);
      M5.Lcd.print("*");
      pinLength++;
      confirmDigit = true;
    }

    if (confirmDigit) {
      pinDigit = 0;  // Reseta para o próximo dígito
      confirmDigit = false;
    }
  }

  return passwordPin;
}

// Permite definir o tamanho temporário da senha
void setTemporaryPasswordLength() {
  tempPasswordLength = passwordLength;  // Começa com o valor padrão
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("Psswrd Length:");
  M5.Lcd.setCursor(30, 80);
  M5.Lcd.print(tempPasswordLength);

  while (true) {
    M5.update();
    if (M5.BtnB.wasPressed()) {
      tempPasswordLength++;
      if (tempPasswordLength > 40) tempPasswordLength = 8; // Limita entre 8 e 40
      M5.Lcd.fillRect(50, 80, 240, 20, BLACK); // Limpa o valor anterior
      M5.Lcd.setCursor(50, 80);
      M5.Lcd.print(tempPasswordLength);
    }
    if (M5.BtnA.wasPressed()) {
      break;
    }
  }
}

// Geração de senha usando SHA256 com base no Bank, PIN inicial, deviceID e PIN de senha
String sha256GeneratePassword(char bank, String pin, String passwordPin) {
  SHA256 sha256;
  sha256.reset();
  sha256.update(String(bank).c_str(), 1);
  sha256.update(pin.c_str(), pin.length());
  sha256.update(deviceID.c_str(), deviceID.length());
  sha256.update(passwordPin.c_str(), passwordPin.length());

  byte hash[32];
  sha256.finalize(hash, sizeof(hash));

  String result = "";
  String charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
  for (int i = 0; i < tempPasswordLength; i++) {
    int index = hash[i % 32] % charset.length(); // Gera com base no hash
    result += charset[index];
  }
  return result;
}

// Verifica se a senha gerada é forte
bool isStrongPassword(String password) {
  bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
  for (int i = 0; i < password.length(); i++) {
    char c = password[i];
    if (isupper(c)) hasUpper = true;
    else if (islower(c)) hasLower = true;
    else if (isdigit(c)) hasDigit = true;
    else hasSpecial = true;
  }
  return (hasUpper && hasLower && hasDigit && hasSpecial);
}

// Define o comprimento da senha
void setPasswordLength() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("Psswrd Length:");

  bool confirmChange = false;
  while (true) {
    M5.update();
    if (M5.BtnB.wasPressed()) {
      passwordLength++;
      if (passwordLength > 40) passwordLength = 8;
      M5.Lcd.fillRect(50, 80, 240, 20, BLACK); // Limpa o valor anterior
      M5.Lcd.setCursor(50, 80);
      M5.Lcd.print(passwordLength);
      confirmChange = true;
    }
    if (confirmChange && M5.BtnA.wasPressed()) {
      preferences.putInt(PREF_LENGTH, passwordLength);
      M5.Lcd.setCursor(50, 120);
      M5.Lcd.print("saved!");
      delay(2000);
      break;
    }
    if (M5.BtnA.wasPressed() && !confirmChange) {
      break;
    }
  }
  displayMainMenu();
}

// Define o brilho da tela
void setBrightness() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("Brightness:");

  bool confirmChange = false;
  while (true) {
    M5.update();
    if (M5.BtnB.wasPressed()) {
      defaultBrightness += 10;
      if (defaultBrightness > 100) defaultBrightness = 10;
      M5.Axp.ScreenBreath(defaultBrightness * 12); // Ajusta o brilho
      M5.Lcd.fillRect(50, 80, 240, 20, BLACK); // Limpa o valor anterior
      M5.Lcd.setCursor(50, 80);
      M5.Lcd.print(defaultBrightness);
      confirmChange = true;
    }
    if (confirmChange && M5.BtnA.wasPressed()) {
      preferences.putInt(PREF_BRIGHTNESS, defaultBrightness);
      M5.Lcd.setCursor(50, 120);
      M5.Lcd.print("saved!");
      delay(2000);
      break;
    }
    if (M5.BtnA.wasPressed() && !confirmChange) {
      break;
    }
  }
  displayMainMenu();
}

// Verifica o status da bateria e energia
void checkPowerStatus() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("Battery: ");
  M5.Lcd.print(M5.Axp.GetBatVoltage(), 2);
  M5.Lcd.print("V");

  M5.Lcd.setCursor(30, 80);
  M5.Lcd.print("    Press A");
  
  while (true) {
    M5.update();
    if (M5.BtnA.wasPressed()) {
      displayMainMenu();
      return;
    }
  }
}

// Desliga o dispositivo
void shutdownDevice() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 50);
  M5.Lcd.print("Shutting down...");
  
  // Desconectar BLE antes de desligar
  if (bleKeyboard.isConnected()) {
    bleKeyboard.end();  // Finaliza a conexão BLE
  }
  
  delay(2000);
  M5.Axp.PowerOff();
}
