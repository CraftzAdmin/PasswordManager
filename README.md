![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white) [![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC_BY--NC--SA_4.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/) 

# Password Generator with M5StickC Plus


This project aims to create a **password generator** using a **physical device**, specifically the **M5StickC Plus.** The key goal is to ensure that whenever the same patterns and inputs are provided, the device regenerates the **same passwords**. 

**Unlike traditional password generators, where passwords might be hardcoded or stored directly on the device, this solution provides a dynamic and secure way to consistently generate passwords based on user-defined inputs.**

This approach offers a **level of security** and convenience for users who require unique, secure passwords for different applications but want to avoid storing sensitive data on physical or cloud-based platforms, but also with it's downsides....

*Imagine that you frequently access multiple online accounts that require strong, unique passwords. Instead of using a password manager or storing these passwords somewhere, you have your **M5StickC Plus** device, a physical password generator, ready to help.*

*Each time you need to log into a service, for example, your Google account, you know that the password for Google is associated with **Bank A** and a **4-digit PIN** of `1200`, and you prefer a password length of **12 characters**. You simply turn on your M5StickC Plus, select **Bank A**, enter the PIN `1200`, and specify the length of the password as 12. The device instantly generates a secure, unique password, which it sends wirelessly to your laptop via **Bluetooth** as if it were being typed on a keyboard.*

*Because the password is generated deterministically using the **Device ID**, **SHA256 hashing**, and your specific inputs, the same password will be generated each time you enter this pattern. This ensures that you never have to store the password—it will always be the same as long as you input the correct combination of Bank, PIN, and password length. If you lose the device, an attacker would still need both your M5StickC Plus and knowledge of your specific input patterns (Bank, PIN, and length) to regenerate your passwords.*

  ![M5StickCPlus](https://github.com/CraftzAdmin/PasswordManager/blob/main/PasswordManager_Craftz.png) 

## Disclaimer

**This project is provided as-is, with no guarantees or warranties regarding its security, functionality, or fitness for any particular purpose**. The **M5StickC Plus Password Generator** is a tool intended for personal use and should be used at your own risk. While this device offers a secure way to generate deterministic passwords, it is important to recognize that the **M5StickC Plus** does not have built-in encryption or advanced memory protection, and the **Arduino code** is open source, meaning certain sensitive information (e.g., device ID, initial PIN) may be visible in the source code.

You are responsible for ensuring the physical security of your device and the privacy of any inputs (such as PINs and patterns) you use. The developer of this project is not liable for any data loss, security breaches, or damages arising from the use or misuse of this device or its code.

## Main Functionalities of the Password Generator Device

-   **Welcome and Startup Screen**: Displays a welcome message and prompts the user to press **Button A** to begin.
    
-   **PIN-based Unlocking**: Users are required to input a 4-digit PIN to unlock the device. If the wrong PIN is entered three times, the device locks for 1 minute. **Bluetooth functionality is only enabled after the correct PIN is accepted, ensuring that no sensitive data is transmitted until the device is unlocked.**
    
-   **Bank and PIN Selection for Password Generation**:
    
    -   The user selects a **Bank** (A-Z) and inputs a 4-digit **PIN**. This combination, along with the device’s **ID** and **Initial PIN**, is used to deterministically generate passwords.
-   **Customizable Password Length**: The user can specify the length of the password to be generated, typically between **8 and 40 characters**. This allows for flexibility in meeting different password requirements.
    
-   **SHA256-based Password Generation**: The device uses **SHA256 hashing** to combine the **Bank**, **PIN**, and **Device ID** to generate a secure password. This ensures that the same input will always produce the same password, and different inputs will result in unique passwords.
    
-   **Password Strength Evaluation**: After generating a password, the device evaluates its strength by checking for the inclusion of:
    
    -   **Uppercase letters** (`A-Z`)
    -   **Lowercase letters** (`a-z`)
    -   **Numbers** (`0-9`)
    -   **Special characters** (e.g., `!@#$%^&*`)
    
    A password is considered **strong** if it contains all four types of characters. If any of these categories are missing, the password is labeled as **weak**.
    
-   **BLE Keyboard Emulation**: The M5StickC Plus emulates a **Bluetooth keyboard** and can send the generated password to a connected device (e.g., phone, laptop) via Bluetooth, as if it were typed manually. The Bluetooth connection is only initiated after the correct PIN is entered, adding an extra layer of security.
    
-   **Clear Bluetooth Bonding Information**: After each session, the device clears any saved Bluetooth bonding information, requiring the host device to re-pair upon the next connection. This enhances security by preventing automatic reconnections.
    
-   **Settings Menu**:
    
    -   **Set Password Length**: Allows users to configure the default password length for future password generations.
    -   **Set Screen Brightness**: Adjusts the brightness of the device’s display to conserve battery or improve visibility.
-   **Battery Status Check**: The user can check the current battery voltage and charge level of the device.
    
-   **Shutdown Option**: The device provides an option to shut down and conserve power by entering deep sleep mode, ensuring it is not consuming energy when not in use.

## Device Overview

The **M5StickC Plus** is a compact, low-power consumption development board equipped with a **1.14-inch color display** (135x240 TFT LCD), an **ESP32 microcontroller**, and multiple GPIOs. It is widely used for small IoT projects, given its small size and flexibility. In this project, the M5StickC Plus serves as a **physical password generator**, capable of generating strong passwords without the need for cloud storage or third-party password managers.

### Key Features of M5StickC Plus:

-   **ESP32 microcontroller**: A powerful chip that supports both BLE and Wi-Fi, offering wireless capabilities for the project.
-   **Color LCD**: Used to display generated passwords and interactions.
-   **Buttons**: Physical buttons are used to input the patterns and settings for generating the passwords.
-   **BLE Keyboard Emulation**: Sends generated passwords wirelessly as if typed by a physical keyboard.

 ![M5StickCPlus](https://github.com/CraftzAdmin/PasswordManager/blob/main/61QOaBs712L._AC_UF1000,1000_QL80_.jpg)


## Program Overview

The program running on the M5StickC Plus performs the following:

1.  **User Input for Password Generation**:
    -   Users can input a PIN and select a **"Bank"** (an alphabetical category) that forms the basis for the password generation.
    -   The same combination of PIN and Bank will always regenerate the same password.
2.  **Password Customization**:
    -   Users can specify the **length** of the password they want, which provides flexibility in adapting to different password length requirements.
    -   The generated password will always be a secure mix of **uppercase**, **lowercase**, **numbers**, and **special characters**.
3.  **Secure and Consistent Password Generation**:
    -   The passwords are generated dynamically using a **SHA256-based hash**, **ensuring that no passwords** **are hardcoded on the device**. This guarantees that even if the device is compromised, no sensitive password data is stored.
4.  **BLE Keyboard Integration**:
    -   The device uses **Bluetooth Low Energy (BLE)** to emulate a keyboard and can send the generated password wirelessly to a paired device (such as a laptop or phone) as if typed manually.
5.  **Re-pairing on Disconnect**:
    -   The device is designed to force a new pairing request upon disconnection, ensuring that the user must manually reconnect to the device each time for added security.


## Risks and Security Considerations

This project is built using **open-source technologies** and is based on **Arduino code** running on the M5StickC Plus, a device that **lacks built-in memory encryption or advanced security protections**. As such, there are some important considerations to keep in mind regarding the security of the device and the code.

### Security Risks:

1.  **Open-source Nature**: Since the project is open-source, the code is available to anyone. This means that the **device ID** and **initial PIN** are potentially visible in the code. Although these can be changed by the user, it's important to recognize that anyone with access to the source code can see how the system works, which may present security risks in the wrong hands.
2.  **No Encrypted Memory on Device**: The M5StickC Plus does not have **hardware-encrypted storage**. This means that any sensitive information stored or processed by the device, such as the device ID or any runtime variables, is vulnerable if the device is physically compromised. There is no secure enclave or encrypted memory available to store information securely on the device itself.
3.  **Hardcoded Values**: While the **device ID** and **initial PIN** are hardcoded into the Arduino code, they can be modified by the user before flashing the device. However, it’s important to understand that these values are visible in the source code, which is not the most secure method if the code is publicly shared.

### 1. **SHA256 Hashing Risks**

-   **Deterministic Nature**: SHA256 is a cryptographic hash function that is deterministic. This means the same inputs will always generate the same output. While this is essential for your application (so that the same Bank + PIN always results in the same password), it introduces the following risks:
    
    -   **Guessable Inputs**: If an attacker knows or can guess your Bank and PIN combination, they can easily regenerate the same password.
    -   **No Salt**: Typically, when using hashing algorithms, a random value (salt) is added to the input to ensure different hashes for the same input. In your case, you're relying on the unique **DeviceID** and **PIN** as a sort of "salt," but if these values are known or compromised, passwords can become predictable.
    
### 2. **Character Substitution from Hash**

-   **Character Mapping and Distribution**: The method of converting the SHA256 hash bytes into characters using the modulus operator (`%`) is generally fine for ensuring characters fall within your charset (letters, numbers, symbols). However, since you’re selecting characters from a predefined charset, you  introduce some patterns or biases:
    
    -   **Uniformity**: Depending on how the hash bytes are distributed, some characters may appear more frequently than others in the final password, reducing randomness slightly.
    -   **Limited Character Set**: You are limiting the password to a set of **72 characters**. While this is good for compatibility, it reduces entropy compared to using a wider character set.

### 3. **Conversion and Modulus Operation**

-   **Modulo Operation**: You're using the modulus operator (`%`) to map hash bytes to indexes within the charset. This technique works fine, but it can introduce slight biases in cases where the hash bytes aren't evenly distributed across all potential values.
    
    -   For instance, since you're dividing by `72` (the length of the charset), some characters may appear more frequently due to how the modulo operation distributes values.
    
### 4. **Password Duplication**

-   **Risk of Duplicate Passwords**: Since your password generator is deterministic, using the same Bank + PIN combination on the same device will always generate the same password. This introduces the following risks:
    
    -   **Reuse Across Services**: If the same Bank + PIN combination is reused for multiple services, the password will be identical across those services, which is a known security risk.
    -   **Collision Between Devices**: If the same **PIN** and **Bank** combination is used on two different devices with the same **DeviceID** (e.g., through duplication or manufacturing error), it could result in the same password being generated.
    
### 6. **Non-Secure Storage (Preferences)**

-   **Storing Initial PIN and DeviceID**: You are using the **Preferences** library to store the **Initial PIN** and potentially other sensitive data. While Preferences provide non-volatile storage, they are not encrypted by default on the ESP32 platform.
    
    -   If an attacker gains access to the device's storage, they may be able to extract the stored PIN, making it easier to regenerate the passwords.
    
### 7. **BLE Pairing Risks**

-   **Bluetooth Security**: Since the device uses **BLE Keyboard** functionality, there are inherent security risks in BLE communications.
    
    -   **Re-pairing**: If BLE connections are not cleared correctly after each session, the device could automatically reconnect to a previously paired device, allowing an attacker to potentially intercept or misuse the password.

### 8. **Potential for Physical Attack**

-   **Device Access**: Since the security of this system relies on the uniqueness of the **DeviceID** and the secrecy of the **PIN**, an attacker who gains physical access to the device could potentially extract these values.
    
    -   **Device Duplication**: An attacker who duplicates the DeviceID or guesses the PIN could generate the same passwords.


### Security Advantages:

Despite the aforementioned risks, the project provides a **more ecure way to generate and manage passwords** with some notable security features:

1.  **Password Generation by Device ID and SHA256 Hash**: The passwords are not hardcoded. Instead, they are dynamically generated through a combination of the **device ID**, a **SHA256 hash**, and a **user-defined PIN and bank**. This ensures that:

    -   The same combination of inputs will always generate the same password, making it easy for users to remember input patterns (like `A1200/12`) rather than actual passwords.
    -   Passwords are consistently **unique** and can include **uppercase, lowercase, numbers, and special characters** for strong password creation.
    
2.  **No Account Data Stored on the Device**: One of the biggest advantages of this project is that **no account data** or sensitive information (such as service names or account usernames) is stored on the device.

> *For example, if a user knows that their **Google account** uses the pattern `A1200/12` (Bank A, PIN 1200, 12 characters), they only need to remember this pattern. The actual password is regenerated on-demand by the device using the known inputs.*

   
 -   Even if the device is physically compromised, the attacker would not know which accounts are associated with which PINs and banks unless they have access to additional external information (such as the user’s records).
 
4.  **Separation of Data and Accounts**: Because there is no clear connection between the generated passwords and specific accounts (i.e., which Bank/PIN combination belongs to which account), it provides an extra layer of security. If the device is compromised, **attackers cannot easily deduce which password belongs to which account** without additional information from the user.

While this project is **not suited for high-security use cases** that require encrypted hardware or secure storage, it offers a practical and secure method for generating **dynamic passwords**. Users who take care to manage the physical security of the device and customize the code with unique device IDs and PINs can benefit from the convenience of **consistent password generation** without relying on insecure storage solutions.

This approach provides a significant advantage over hardcoded password generators by using **cryptographic hashing** and dynamic inputs. **Users only need to remember input patterns (Bank + PIN + Length)**, and not the actual passwords, which can enhance security without relying on external password managers.

## Password Generation Logic and Substitution

The password generation logic in this project uses a **deterministic** method, ensuring that the same combination of inputs (Bank + PIN + Length) will always result in the same password. This is done using a combination of the **Device ID**, **Initial PIN**, and a user-provided **4-digit PIN** and **Bank** (A to Z). The core of this logic relies on **SHA256 hashing** to generate secure, repeatable results.

### Deterministic vs. Random Generation

1.  **Random Password Generators**:
    -   **Random password generators rely on entropy** (randomness) to create passwords that are unpredictable each time. While this provides strong security, it comes with the challenge of needing to store the passwords or use a password manager since there's no way to reproduce the same password without having it stored.
    
2.  **Deterministic Password Generators**:
    -   In contrast, a **deterministic password generator** produces the **same password** when given the same inputs. This means that users can rely on the **pattern** they choose (Bank + PIN) to always generate the same password for the same account.
 
    -   The advantage of this system is that passwords do not need to be stored anywhere. Instead, users simply remember the input pattern used to generate the password (e.g., Bank A + PIN 1200 + length of 12 characters).
  -----

### Substitution Mechanism

The password generation in this project leverages **substitution** to ensure the password contains a secure mix of characters:

-   The SHA256 hash is generated based on the combination of the device ID, the initial PIN, and the user-provided Bank and PIN.
-   This hash is then used to select characters from a **charset** that includes:
    -   **Uppercase letters** (`A-Z`)
    -   **Lowercase letters** (`a-z`)
    -   **Numbers** (`0-9`)
    -   **Special characters** (e.g., `!@#$%^&*`)

Each byte from the generated hash is used to map to an index in this charset, ensuring a diverse and secure selection of characters in the password.

### How many Passwords can it generate? 

-   **Banks**: The user can select from **26 banks** (`A-Z`), giving us 26 possible choices.
-   **4-digit PIN**: The user can choose a **4-digit PIN**, where each digit ranges from `0-9`. This gives us \(10^4 = 10,000\) possible combinations.
  
-   **Password Length**: The user can specify the length of the password, typically between **8 to 40 characters**. For simplicity, we can assume the length to be an additional factor in determining the diversity of passwords, but the core uniqueness comes from the combination of Bank and PIN. As a fact, the same BANK and PIN, will have the same root, for example - a password of bank A1001 with 8 char will be generated as 12345678.... with 12 char, it will contain the same 1-8 root + the new char. **So take this into account!**

This means that with the combination of **26 banks and 10,000 possible 4-digit PINs,** the device can generate **260,000 unique patterns**. Each of these patterns can be used to generate a password of any specified length (e.g., 8, 12, 16 characters, etc.), providing an incredibly large space of possible passwords.

### Why This System is (a little bit more) Secure

**The security of the system comes from the fact that:**

-   The password is generated **dynamically** based on user inputs and not hardcoded into the device.
-   Even if someone has access to the source code, they would still need the **specific combination of Bank, PIN, Device ID, and Initial PIN** to generate the correct password for the account.
-   The use of **SHA256 hashing** ensures that the output is **cryptographically secure** and not easily reversible.

## Libraries
| **Library**         | **License**     | **Author**              | **Version**   | **Link** |
|---------------------|-----------------|-------------------------|---------------|----------|
| `M5StickCPlus.h`     | MIT License     | M5Stack                  | 0.2.7         | [M5StickCPlus](https://github.com/m5stack/M5StickC) |
| `BleKeyboard.h`      | MIT License     | T-vK                    | 0.3.0         | [BleKeyboard](https://github.com/T-vK/ESP32-BLE-Keyboard) |
| `SHA256.h`           | Public Domain   | Mike B. (Cryptosuite)    | 1.0.0         | [SHA256](https://github.com/BlakeFoster/Arduino-SHA256) |
| `Preferences.h`      | MIT License     | Espressif Systems        | Part of ESP32 Core | [Preferences](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences) |


## Key Functionalities and Code Breakdown

### Required Libraries

To make the program functional, the following libraries are required:

1.  **M5StickCPlus.h**:
    -   This library allows control over the M5StickC Plus hardware components, including the LCD display and buttons. It is necessary for interacting with the device's buttons and for displaying information such as the generated password.
2.  **BleKeyboard.h**:
    -   The M5StickC Plus does not support USB HID, meaning that it cannot act as a USB keyboard directly. However, it does support **Bluetooth Low Energy (BLE)**, allowing it to emulate a keyboard via Bluetooth.
    -   The **BleKeyboard** library allows the M5StickC Plus to function as a wireless Bluetooth keyboard. This is essential for sending the generated password to connected devices (laptops, phones, tablets) wirelessly.
3.  **Preferences.h**:
    -   This library is used for storing persistent data in the ESP32's non-volatile storage. In this project, it stores user-configurable options such as the **initial PIN** and **default password length**.
4.  **SHA256.h**:
    -   The **SHA256** library is critical for generating secure, deterministic passwords. It hashes the combination of the **Device ID**, **user's input PIN**, and **Bank selection** to create the password. 

### Setup and User Input Before Compiling

Before compiling and uploading the code to the M5StickC Plus, the user needs to configure a few key details:

1.  **Device ID**:
    
    -   The **Device ID** is a critical piece of information that uniquely identifies your M5StickC Plus device. This ID is used as part of the password generation process and ensures that passwords generated on your device are unique to that device.
        
    -   You must input your own Device ID into the code at the following line:
        
        ```cpp
        const String deviceID = "YOUR_DEVICE_ID_SHA256_HASH_HERE";
        ```
        
    -   This could be the ESP32 chip's MAC address or another unique identifier, or you can generate a hash online. 
        
2.  **Initial PIN**:
    
    -   The **Initial PIN** serves as an extra layer of security for password generation. It combines with the user’s **4-digit PIN** and **Bank** selection to generate the password.
        
    -   You must set an initial PIN value in the code before compilation:
        
        ```cpp
        const String initialPin = "1234";  // Example PIN
        ```
        
3.  **Password Length**:
    
    -   The user can set a default password length, which will be used unless changed during runtime:
        
        ```cpp
        int passwordLength = 12;  // Default password length
        ```
        

## Code Walkthrough

1.  **Startup and PIN Entry**:

    -   When the M5StickC Plus is powered on, it prompts the user to enter a **4-digit PIN**. This PIN, combined with the **Initial PIN** and **Device ID**, forms the basis for generating a secure password.
    -   The program then waits for user input, with the **Bank** selection and PIN being used to customize the password generation.
3.  **Bluetooth Keyboard Functionality**:
    -   The device emulates a Bluetooth keyboard, allowing it to send the generated password to any device that supports Bluetooth input (e.g., computers, phones).
    -   This is particularly useful for users who need to enter passwords on multiple devices without typing them manually.
    -   **Since the M5StickC Plus does not support USB HID (human interface device) mode, BLE is the best option for sending passwords wirelessly.**
4.  **Password Generation with SHA256**:
    -   Once the user has selected a **Bank**, entered their **PIN**, and specified the password length, the device uses **SHA256 hashing** to generate a secure password.
    -   The combination of **Device ID**, **Initial PIN**, and the user’s **Bank and PIN** ensures that each password is unique and deterministic. The password generated will always be the same for the same set of inputs.
    -   The output is a secure password that contains a mixture of uppercase, lowercase letters, numbers, and special characters.
5.  **Re-pairing Requirement**:
    -   After each disconnection, the device forces a new pairing request to ensure that the BLE connection is reset. This provides added security by requiring the user to manually reconnect the device, ensuring no automatic reconnection happens without user consent.

----------

### Function Breakdown

### `setup()`

```cpp
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
```

-   **Key Steps**:
    1.  **Initialize M5StickC Plus components**: Sets up the screen and button interactions.
    2.  **Initialize Bluetooth Keyboard**: Prepares the device to act as a BLE keyboard.
    3.  **Clear previous bonding information**: Ensures that the Bluetooth device must pair again after each restart.
    4.  **Load preferences**: Checks if the **Initial PIN** is stored and loads it; otherwise, it sets a default.
    5.  **Display the startup screen**: Shows the initial welcome screen and waits for the user to proceed.

### `clearBondingInformation()`

```cpp
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
```

-   **Key Steps**:
    -   Retrieves the list of previously bonded Bluetooth devices and removes them, forcing the device to re-pair each time it connects to a host.

### `startupScreen()`

```cpp
void startupScreen() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.print("Password Manager");
    M5.Lcd.setCursor(60, 80);
    M5.Lcd.print("by Craftz");
    M5.Lcd.setCursor(30, 120);
    M5.Lcd.print("Press A to continue");

    while (true) {
        M5.update();
        if (M5.BtnA.wasPressed()) {
            break;
        }
    }
    unlockDevice();
}
```

-   **Key Steps**:
    -   Shows a simple welcome message on the screen and waits for the user to press Button A to proceed to the next step, which is unlocking the device with the PIN.

### `unlockDevice()`

```cpp
void unlockDevice() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(50, 50);
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
                M5.Lcd.setCursor(50, 50);
                M5.Lcd.print("Wrong PIN!");
                delay(2000);
                unlockDevice();
            }
        }
    }
}
```

Handles the unlocking process by prompting the user to enter their 4-digit PIN. Once the correct PIN is entered, it proceeds to wait for a BLE connection.
-   **Key Steps**:
    -   The user is prompted to enter their PIN by cycling through digits using Button B and confirming with Button A. After 3 incorrect attempts, the device locks for 1 minute.



### `sha256GeneratePassword`

This function takes three parameters:
- **`bank`**: A character representing the selected bank (from 'A' to 'Z').
- **`pin`**: A `String` containing the initial 4-digit PIN.
- **`passwordPin`**: Another `String` containing the user-input password PIN.

These three values, along with the **Device ID** of the M5StickC Plus, are used to generate a unique, consistent password based on an SHA256 hash.

### Step-by-Step Explanation:

1. **Initialize the SHA256**:
   ```cpp
   SHA256 sha256;
   sha256.reset();
   ```
   The function initializes the **SHA256** object, which will be used to generate the hash. The `reset()` method ensures there is no residual data in the buffer.

2. **Feed the Inputs into the Hash**:
   The function then feeds the values of **bank**, **pin**, **deviceID**, and **passwordPin** into the SHA256 object. Each value is passed as a `String` and converted to a C-string (`char*`) for the SHA256 function to process.
   
   ```cpp
   sha256.update(String(bank).c_str(), 1); // Adds the bank value
   sha256.update(pin.c_str(), pin.length()); // Adds the initial PIN
   sha256.update(deviceID.c_str(), deviceID.length()); // Adds the device ID
   sha256.update(passwordPin.c_str(), passwordPin.length()); // Adds the password PIN
   ```

3. **Generating the Hash**:
   Once all the inputs have been fed into the SHA256 object, the `finalize()` method is called, which generates a **256-bit hash (32 bytes)** based on the provided data.
   
   ```cpp
   byte hash[32];
   sha256.finalize(hash, sizeof(hash));
   ```

4. **Character Substitution Based on the Hash**:
   Now comes the important part: transforming the hash into a readable, secure password. To do this, we use the `hash[32]` array, where each byte has a value between 0 and 255.

   - The function uses a character set (charset) to map the hash values to readable characters:
     ```cpp
     String charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
     ```

   - The charset has 72 characters (26 uppercase letters, 26 lowercase letters, 10 numbers, and 10 special characters).

   - Each byte from the hash is used to select a character from this charset. The **modulus operation** (`%`) ensures that the byte value is mapped within the valid index range of the charset:
     ```cpp
     for (int i = 0; i < tempPasswordLength; i++) {
       int index = hash[i % 32] % charset.length(); // Maps the hash value to a charset index
       result += charset[index];
     }
     ```
     - The expression `hash[i % 32]` makes sure that the index cycles through the 32 bytes of the hash repeatedly (if the password length exceeds 32, it reuses the hash in a loop).
     - The `% charset.length()` ensures that the index is always between 0 and 71, which are the valid indexes of the charset.

5. **Return the Generated Password**:
   After the substitution, the function returns the generated password:
   ```cpp
   return result;
   ```

### Example:

- Suppose the generated SHA256 hash starts with the following bytes: `[43, 178, 67, 90, ...]`
- For each byte, the function maps the value to a character from the charset:
  - 43 % 72 → Maps to index 43 in the charset: 'r'
  - 178 % 72 → Maps to index 34 in the charset: 'i'
  - 67 % 72 → Maps to index 67 in the charset: '@'
  - 90 % 72 → Maps to index 18 in the charset: 'S'
  
  Continuing this process, the function constructs the password by substituting each hash value with the corresponding character from the charset.


### `checkPin()`

```cpp
bool checkPin(String pin) {
    String storedPin = preferences.getString(PREF_PIN, initialPin);
    return pin.equals(storedPin);
}
```

-   **Key Steps**:
    -   Retrieves the stored initial PIN from preferences and compares it to the user’s input.

### `waitForBLEConnection()`

```cpp
void waitForBLEConnection() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(30, 50);
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
```

-   **Key Steps**:
    -   Continuously checks whether a BLE connection has been established. If the connection is made, the program continues.

### `displayMainMenu()`

```cpp
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
```


-   **Key Steps**:
    -   Users can cycle through options with Button B and confirm their selection with Button A. Each option corresponds to a different function (password generation, settings, etc.).

### `generatePassword()`

```cpp
void generatePassword() {
    // Seleção do Bank
    char bank = 'A';
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(50, 50);
    M5.Lcd.print("Select Bank (A-Z):");
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
    M5.Lcd.setCursor(50, 50);
    M5.Lcd.print("Generated Password:");
    M5.Lcd.setCursor(50, 80);
    M5.Lcd.print(password);

    // Exibe a força da senha
    M5.Lcd.setCursor(50, 110);
    if (isStrongPassword(password)) {
        M5.Lcd.print("Strength: Strong");
    } else {
        M5.Lcd.print("Strength: Weak");
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

```
-   **Key Steps**:
    -   The user selects a **Bank** (A to Z), enters a **PIN**, and specifies a password length.
    -   The password is then generated using a **SHA256 hash** based on the combination of the Bank, PIN, and **Device ID**.
    -   The generated password is displayed on the screen, and the user can send it via Bluetooth using Button A.


## Navigating the Interface

1.  **Startup**: When the device is powered on, the user is greeted with a welcome screen. Press **Button A** to proceed.
2.  **Unlocking**: The user is prompted to enter a 4-digit **PIN** using **Button B** to increment digits and **Button A** to confirm each digit. After entering the correct PIN, the user proceeds to the main menu.
3.  **Main Menu**:
    -   The main menu contains options such as **Password Generation**, **Set Password Length**, **Set Brightness**, **Check Battery Status**, and **Shutdown**.
    -   Use **Button B** to cycle through the options and **Button A** to select.
4.  **Generating a Password**:
    -   Select **Password Generation** from the main menu. Choose a **Bank** (A-Z), enter a 4-digit PIN, and specify the password length. The device generates a secure password based on these inputs and allows you to send it via Bluetooth to any connected device.
5.  **BLE Keyboard**: The device emulates a Bluetooth keyboard and can send the generated password wirelessly to any Bluetooth-enabled host (phone, laptop, tablet), by pressing A as many times as necessary. 


## Known Issue: BLE Keyboard Bug on Windows


There is a known bug when using the **M5StickC Plus** as a BLE keyboard on **Windows**. After the device disconnects from the host (e.g., laptop or desktop), and then reconnects, the BLE keyboard does not function as expected. Specifically, it fails to send keystrokes or input data to the host, even though the device appears connected.



## Workaround


To resolve this issue, follow these steps:

1.  On your **Windows host** device, open **Settings** and navigate to the **Bluetooth & Devices** section.
2.  Find the **M5StickC Plus** BLE device in the list of paired devices.
3.  **Remove** the device from the list.
4.  Reconnect the M5StickC Plus by initiating a new pairing process.

Once the device is re-paired, the BLE keyboard functionality should work as expected, and the generated passwords will be sent correctly to the host.

------

### Additional Security Consideration

While this bug is an inconvenience, it can also be viewed as an additional layer of security. By requiring the user to remove and re-pair the device after each session, it ensures that only trusted devices can connect, and prevents automatic reconnection without user input. This forces an active pairing process each time the device is used, thereby preventing unintended or unauthorized use.


## Future Plan Releases

In future updates of the **M5StickC Plus Password Generator**, the following features and improvements are planned:

-   **Device Timeout and Auto Logout**: The device will automatically log out after **5 minutes** of inactivity. However, it will remain connected to the host via Bluetooth to ensure that it can be quickly accessed again without requiring a new pairing process. 
-   **Generate Device ID from Internal Device ID and Fuse**: The device ID used in password generation will be created using a **SHA256 hash** of the **internal device ID number**. This will provide a more unique and secure device identifier, ensuring that each M5StickC Plus generates different passwords even if the code is identical. **( esp_efuse_mac_get_default(), esp_chip_info()).**

-   **Fix the BLE Reconnection Bug**: An effort will be made to correct the current BLE reconnection issue on **Windows **, where the BLE keyboard fails to send input after disconnecting and reconnecting. The goal is to ensure seamless BLE keyboard functionality without requiring manual removal and re-pairing on the host.

-   Encrypting Values Stored in Preferences - Using mbedTLS (Built-in to ESP32),
-   Strengthen Initialization
-   Erase on Multiple Failed Attempts

## License

This project is licensed under the **Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)** license. Under this license:

-   **Attribution**: You must give appropriate credit to the original author, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
-   **NonCommercial**: You may not use this work for commercial purposes.
-   **ShareAlike**: If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

For more details, please review the full terms of the [CC BY-NC-SA 4.0 License](https://creativecommons.org/licenses/by-nc-sa/4.0/).
