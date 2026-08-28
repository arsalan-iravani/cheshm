# ParaxFace2

## Project Overview

ParaxFace2 is an **ESP8266-based interactive digital face system** with realistic animated eyes running on a **GC9A01 240×240 round TFT display**.

### Current Status: Image Viewer (v1.0)

The current implementation displays JPEG images from an SD card and provides:
- WiFi web interface for image management
- Image upload with automatic conversion to 240×240 JPEG
- Image list, display, and delete functionality
- Mobile-responsive web UI
- Non-blocking JPEG rendering

### Future: Procedural Eye Animation System (v2.0)

**PLANNED FOR NEXT PHASE:**
- Fully procedurally-generated, code-rendered realistic eyes
- Dynamic iris with procedural texture
- Realistic pupil with dilation/contraction
- Natural eye movement (gaze, micro-saccades)
- Realistic blinking with variable timing
- Multiple facial expressions (happy, sad, angry, surprised, sleepy, confused, crying)
- Procedural tears animation
- WiFi control of eye behavior, color, expression, speed
- Live animation while serving web requests

---

## Hardware Requirements

### Microcontroller
- **ESP8266 NodeMCU / ESP-12F**
- Flash: 4 MB (1 MB available for user code + OTA)
- SRAM: ~45 KB available for sketch

### Display
- **GC9A01 240×240 Round TFT Display**

### Storage
- **Micro SD Card** (for image storage in current version)

### Connections

#### TFT Display (GC9A01)
```
TFT Pin     ESP8266 Pin    GPIO      Purpose
─────────────────────────────────────────────
VCC         3.3V           -         Power
GND         GND            -         Ground
SCLK        D5             GPIO14    SPI Clock
MOSI        D7             GPIO13    SPI Data In
MISO        D6             GPIO12    SPI Data Out
CS          D8             GPIO15    Chip Select
DC          D3             GPIO0     Data/Command
RST         D4             GPIO2     Reset
```

#### Micro SD Card
```
SD Pin      ESP8266 Pin    GPIO      Purpose
─────────────────────────────────────────────
VCC         3.3V           -         Power
GND         GND            -         Ground
CS          D2             GPIO16    Chip Select
CLK         D5             GPIO14    SPI Clock
MOSI        D7             GPIO13    SPI Data In
MISO        D6             GPIO12    SPI Data Out
```

**Note:** TFT and SD card share the SPI bus (SCLK, MOSI, MISO) with separate chip select lines.

### Wiring Diagram

```
ESP8266 NodeMCU
┌─────────────────────────────────────────────────┐
│                                                 │
│  D2(GPIO16) ──────────> SD CS                  │
│  D3(GPIO0)  ──────────> TFT DC                 │
│  D4(GPIO2)  ──────────> TFT RST                │
│  D5(GPIO14) ──┬──────> TFT SCLK                │
│               └──────> SD CLK                   │
│  D6(GPIO12) ──┬──────> TFT MISO                │
│               └──────> SD MISO                  │
│  D7(GPIO13) ──┬──────> TFT MOSI                │
│               └──────> SD MOSI                  │
│  D8(GPIO15) ──────────> TFT CS                 │
│  3.3V       ──┬──────> TFT VCC                 │
│               └──────> SD VCC                   │
│  GND        ──┬──────> TFT GND                 │
│               └──────> SD GND                   │
│                                                 │
└─────────────────────────────────────────────────┘
```

---

## Project Structure

```
ParaxFace2/
├── ParaxFace2.ino          # Main sketch file (entry point)
├── display.h               # TFT display initialization
├── display.cpp             # Display and image rendering
├── wifi.h                  # WiFi configuration
├── wifi.cpp                # WiFi AP mode setup
├── web.h                   # Web server interface
├── web.cpp                 # Web server and HTML UI
├── README.md               # This file
├── SETUP.md                # Detailed setup instructions
├── TECHNICAL_ANALYSIS.md   # Complete technical analysis
├── LIBRARIES.md            # Required libraries and versions
└── HARDWARE_PINS.md        # Detailed pin configuration
```

---

## Required Libraries

### Arduino IDE Libraries (Install from Library Manager)

1. **TFT_eSPI** (version 2.4.70 or later)
   - Display driver for TFT controllers
   - Required for GC9A01 support

2. **TJpg_Decoder** (version 1.3.3 or later)
   - JPEG decoder for ESP8266
   - Reads JPEG files from SD card

3. **SD** (version 1.2.4 or later)
   - SD card file system support
   - Built-in with ESP8266 core

4. **ESP8266WiFi** (built-in with ESP8266 core)
   - WiFi networking

5. **ESP8266WebServer** (built-in with ESP8266 core)
   - Web server functionality

### Installation Instructions

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for each library above and click **Install**
4. Restart Arduino IDE

---

## Arduino IDE Configuration

### Board Selection

1. **Tools → Board → esp8266 → NodeMCU 1.0 (ESP-12E Module)**

### Board Settings

```
Board: NodeMCU 1.0 (ESP-12E Module)
Upload Speed: 115200 baud
CPU Frequency: 80 MHz
Flash Size: 4M (1M SPIFFS) or 4M (3M SPIFFS)
IwIP Variant: v2 Lower Memory
Debug port: Disabled
Debug level: None
VTables: Flash
Module: NodeMCU 1.0 (ESP-12E Module)
Reset Method: ck
Crystal Frequency: 26 MHz
```

### COM Port

- Windows: `COM3` or `COM4` (check Device Manager)
- macOS: `/dev/cu.SLAB_USBtoUART` or `/dev/cu.wch_ch34x`
- Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`

---

## TFT_eSPI Configuration

The TFT_eSPI library requires custom pin configuration.

### Setup TFT_eSPI

1. Open Arduino Libraries folder:
   - Windows: `Documents/Arduino/libraries/`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`

2. Navigate to: `TFT_eSPI/User_Setups/`

3. Open `User_Setup.h` (or create if missing)

4. Replace with this configuration:

```cpp
// User_Setup.h for ParaxFace2 - GC9A01 240x240

#define USER_SETUP_ID 203

#define ILI9341_DRIVER
#define GC9A01_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define TFT_CS   D8   // GPIO15
#define TFT_DC   D3   // GPIO0
#define TFT_RST  D4   // GPIO2

#define TFT_MOSI D7   // GPIO13
#define TFT_SCLK D5   // GPIO14
#define TFT_MISO D6   // GPIO12

#define TFT_BACKLIGHT -1

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8

#define SMOOTH_FONT

#define SPI_FREQUENCY  40000000

#define SPI_READ_FREQUENCY 6000000

#define SPI_TOUCH_FREQUENCY 2500000
```

5. Save the file

6. **Restart Arduino IDE**

---

## Setup Instructions

### Step 1: Hardware Assembly

1. Connect ESP8266 to TFT display according to wiring diagram above
2. Connect Micro SD card reader to SD pins
3. Insert Micro SD card into reader
4. Connect ESP8266 to computer via USB cable

### Step 2: Install Arduino IDE

1. Download from https://www.arduino.cc/en/software
2. Install and launch Arduino IDE

### Step 3: Install ESP8266 Board Support

1. **File → Preferences**
2. In "Additional Board Manager URLs", paste:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. **Tools → Board → Boards Manager**
4. Search "esp8266" and install "esp8266 by ESP8266 Community"
5. Restart Arduino IDE

### Step 4: Install Required Libraries

See "Required Libraries" section above.

### Step 5: Configure TFT_eSPI

See "TFT_eSPI Configuration" section above.

### Step 6: Upload Code

1. Open `ParaxFace2.ino` in Arduino IDE
2. **Tools → Board → esp8266 → NodeMCU 1.0 (ESP-12E Module)**
3. Select correct COM port
4. **Sketch → Upload**
5. Wait for upload to complete

### Step 7: Test

1. Open **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. Press ESP8266 reset button or wait for restart
4. You should see startup messages:
   ```
   ==============================
        ParaxFace2
   ==============================
   TFT OK
   SD CARD OK
   WIFI START
   ==============================
   SSID: ParaxFace2
   IP: 192.168.4.1
   ==============================
   Web server started
   Open: http://192.168.4.1
   ```

---

## Web Interface Usage

### Accessing the Web UI

1. On your phone or computer, open WiFi settings
2. Find and connect to network: **ParaxFace2**
3. Password: **12345678**
4. Open web browser and navigate to: **http://192.168.4.1**

### Features

#### Upload Image
- Click **"Upload Image"** section
- Select any image (PNG, JPG, BMP, GIF, WEBP)
- Image is automatically converted to 240×240 JPEG
- Click **"Upload Image"** button
- Progress bar shows upload status
- Image displays on TFT when upload completes

#### Image List
- Shows all images currently stored on SD card
- **Show** button: Display the image on TFT
- **Delete** button: Remove image from SD card
- **Refresh** button: Reload image list

### Web Interface Features

- ✅ Dark theme, modern design
- ✅ Mobile responsive (works on phone and computer)
- ✅ Automatic image scaling to 240×240
- ✅ Proper JPEG compression
- ✅ Upload progress indication
- ✅ Real-time image list updates
- ✅ Image preview before upload

---

## Troubleshooting

### Serial Monitor Shows "SD CARD ERROR!"

**Problem:** SD card not detected or initialization failed

**Solutions:**
1. Check wiring: D2(CS), D5(CLK), D7(MOSI), D6(MISO)
2. Verify SD card is properly inserted
3. Try formatting SD card to FAT32
4. Check for loose connections
5. Try different SD card (some cards are not compatible)

### Serial Monitor Shows "WIFI AP ERROR!"

**Problem:** WiFi AP mode failed to start

**Solutions:**
1. Check if WiFi is enabled on ESP8266
2. Restart the ESP8266
3. Try changing SSID/password in `wifi.cpp`
4. Check Serial Monitor baud rate is 115200

### Cannot Connect to WiFi Network

**Problem:** "ParaxFace2" network doesn't appear

**Solutions:**
1. Check Serial Monitor to verify WiFi started successfully
2. Wait 30 seconds after startup for AP to fully initialize
3. Restart ESP8266
4. Check if device WiFi is enabled

### Cannot Access Web Interface

**Problem:** Browser cannot reach http://192.168.4.1

**Solutions:**
1. Verify connected to "ParaxFace2" network (not just visible)
2. Check you entered correct IP address
3. Try http://192.168.4.1/ (with trailing slash)
4. Restart browser and clear cache
5. Try from different device
6. Check Serial Monitor to verify web server started

### JPEG Display Errors

**Problem:** Image shows as "JPEG ERROR" or "DRAW ERROR"

**Solutions:**
1. Check image file is valid JPEG
2. Try converting image on computer to JPEG first
3. Ensure image is not corrupted
4. Try different image
5. Check that TJpg_Decoder library is installed

### Upload Fails

**Problem:** Image upload fails or shows "Upload failed"

**Solutions:**
1. Check SD card has free space
2. Try smaller image file
3. Check internet connection is stable
4. Refresh page and try again
5. Restart ESP8266

---

## Current Features (Working)

### Display System
- ✅ GC9A01 240×240 TFT initialization
- ✅ JPEG image rendering from SD card
- ✅ Automatic image centering
- ✅ Error handling for corrupt images
- ✅ Black screen initialization

### WiFi System
- ✅ ESP8266 AP mode (Access Point)
- ✅ SSID: ParaxFace2
- ✅ Password: 12345678
- ✅ IP: 192.168.4.1
- ✅ Non-blocking after initialization

### Web Server
- ✅ HTTP server on port 80
- ✅ RESTful API endpoints
- ✅ Client-side JPEG conversion (240×240)
- ✅ Progress tracking during upload
- ✅ Image list with JSON response
- ✅ Image display on demand
- ✅ Image deletion from SD card

### Web Interface
- ✅ Mobile responsive design
- ✅ Dark theme with modern styling
- ✅ Image preview before upload
- ✅ Upload progress bar
- ✅ Image list with action buttons
- ✅ Real-time updates

### File Management
- ✅ SD card file listing
- ✅ Automatic image name sanitization
- ✅ File validation
- ✅ Safe file operations

---

## Known Limitations

### Current Version Limitations

- ❌ **No eye animation** - Only displays static JPEG images
- ❌ **No eye control interface** - Cannot control eye appearance or behavior
- ❌ **Blocking web server** - Cannot animate while serving requests
- ❌ **No expressions** - No happy, sad, angry, etc. expressions
- ❌ **No blinking** - No eyelid animation
- ❌ **No gaze control** - Cannot move eye to look in different directions
- ❌ **No tears** - No tear animation
- ❌ **No eyebrows** - No eyebrow rendering
- ❌ **No iris texture** - Static image-based only
- ❌ **No pupil dilation** - Fixed pupil size
- ❌ **No color control** - Only displays JPEG images as-is

### Hardware Limitations

- **Memory:** ESP8266 has only ~45 KB usable SRAM
- **Performance:** Single-core CPU at 80-160 MHz
- **Display:** 240×240 pixels (small, but sufficient for eye animation)
- **Storage:** Limited flash for OTA updates due to SPIFFS

---

## Planned Features (Next Phase)

### Eye Rendering System
- 🔄 Procedurally generated realistic eye
- 🔄 Sclera (white of eye) with veins
- 🔄 Iris with procedural texture
- 🔄 Pupil with dynamic dilation
- 🔄 Highlights and reflections
- 🔄 Eyelids (upper and lower)
- 🔄 Eyelashes (if memory allows)

### Animation System
- 🔄 Non-blocking animation loop
- 🔄 Natural eye movement
- 🔄 Micro-saccades (small random eye movements)
- 🔄 Smooth gaze transitions
- 🔄 Realistic blinking with variable timing
- 🔄 Blink detection and response

### Facial Expressions
- 🔄 Normal (neutral/relaxed)
- 🔄 Happy (smiling eyes)
- 🔄 Sad (drooping eyelids)
- 🔄 Angry (narrowed eyes)
- 🔄 Surprised (wide eyes)
- 🔄 Sleepy (half-closed eyes)
- 🔄 Confused (asymmetric positioning)
- 🔄 Crying (with animated tears)

### Advanced Features
- 🔄 Procedural tears (sad/crying)
- 🔄 Eyebrows (if memory allows)
- 🔄 Color selection (brown, blue, green, gray, hazel, custom RGB)
- 🔄 Speed controls (animation speed adjustment)
- 🔄 Live WiFi control (changes appear immediately)

### Web Interface Enhancements
- 🔄 Eye control panel
- 🔄 Color picker
- 🔄 Expression selector
- 🔄 Gaze direction controls
- 🔄 Animation behavior controls
- 🔄 Real-time status display
- 🔄 Speed sliders

---

## File Descriptions

### ParaxFace2.ino (Main Sketch)

Entry point for the application. Handles:
- Serial initialization
- TFT display startup
- SD card initialization
- WiFi AP mode startup
- Web server initialization
- Initial image loading
- Main event loop (blocking on web requests in current version)

### display.h

Header file for display functions. Declares:
- `TFT_eSPI tft` - Global TFT object
- Screen dimensions (240×240)
- Display initialization function
- Image loading and rendering functions
- Upload message display

### display.cpp

Implementation of display functions:
- TFT initialization with GC9A01 driver
- JPEG decoding callback
- Image loading from SD card
- Image rendering with proper centering
- Error handling for corrupt images
- Upload prompt message rendering
- Image file validation

### wifi.h

Header file for WiFi functions:
- WiFi initialization
- IP address retrieval

### wifi.cpp

WiFi implementation:
- ESP8266 AP mode setup
- SSID configuration
- Password configuration
- IP address management
- Status reporting

### web.h

Header file for web server:
- Server initialization
- Request handling
- Image management interface

### web.cpp

Web server implementation:
- RESTful API endpoints
  - `GET /` - Serve HTML UI
  - `GET /images` - Get image list as JSON
  - `POST /upload` - Upload new image
  - `GET /show?file=` - Display image
  - `GET /delete?file=` - Delete image
- HTML/CSS/JavaScript web interface
- Image upload handling
- File name sanitization
- Image format validation
- Error handling

---

## API Endpoints

### GET /

Serves the main web interface (HTML/CSS/JavaScript).

**Response:** HTML page with image management UI

### GET /images

Returns list of all images on SD card as JSON.

**Response:**
```json
[
  "image1.jpg",
  "photo.jpg",
  "eye.jpg"
]
```

### POST /upload?filename=name.jpg

Upload new image file.

**Parameters:**
- `filename` - Desired filename (sanitized automatically)

**Response:** "UPLOAD OK" on success

### GET /show?file=name.jpg

Display the specified image on TFT.

**Parameters:**
- `file` - Image filename to display

**Response:** "OK" on success

### GET /delete?file=name.jpg

Delete image from SD card.

**Parameters:**
- `file` - Image filename to delete

**Response:** "DELETED" on success

---

## Testing Checklist

Before proceeding with the next development phase, please test:

### Hardware Tests
- [ ] ESP8266 compiles and uploads successfully
- [ ] Serial Monitor shows startup messages
- [ ] TFT displays output (no black screen)
- [ ] SD card initializes without error

### WiFi Tests
- [ ] "ParaxFace2" network appears in WiFi list
- [ ] Can connect to network with password "12345678"
- [ ] IP address 192.168.4.1 is accessible

### Web Interface Tests
- [ ] Can open http://192.168.4.1 in browser
- [ ] Web page loads with proper styling
- [ ] Can see "Upload Image" section
- [ ] Can see "Images" section

### Image Upload Tests
- [ ] Can select an image file
- [ ] Image preview shows in browser
- [ ] Can click "Upload Image" button
- [ ] Upload progress bar appears
- [ ] Upload completes successfully
- [ ] Image appears in "Images" list
- [ ] Image displays correctly on TFT

### Image Management Tests
- [ ] Can click "Show" to display image on TFT
- [ ] Can click "Refresh" to reload image list
- [ ] Can click "Delete" to remove image
- [ ] Deleted images are removed from list
- [ ] After deleting all images, "PARAX UPLOAD IMAGE" message appears

### Error Handling Tests
- [ ] Try uploading very large image (should handle gracefully)
- [ ] Try uploading non-image file (should handle gracefully)
- [ ] Try uploading with no SD card (should show error message)
- [ ] Try accessing invalid filename

---

## Performance Metrics

### Current Version

- **TFT Initialization Time:** ~500ms
- **SD Card Mount Time:** ~100ms
- **WiFi Startup Time:** ~500ms
- **JPEG Decode Time:** 1-3 seconds (varies by image complexity)
- **Image Display Time:** After decode completes
- **Web Server Response Time:** <500ms for file operations
- **Memory Usage:** ~25-30 KB out of 45 KB available

### Target Specifications (Next Phase)

- **Animation FPS:** 30-60 (16-33ms per frame)
- **Animation Loop Time:** <5ms per iteration
- **Web Server Latency:** <10ms for API calls
- **Total Memory Usage:** Stay within 40 KB limit

---

## Debugging Tips

### Enable Serial Debugging

Serial output is sent to Serial Monitor at 115200 baud. Messages include:
- Startup status
- TFT initialization
- SD card mount status
- WiFi AP creation
- Web server start
- Image loading progress
- Error messages

### Common Serial Messages

```
==============================
       ParaxFace2
==============================
TFT OK
SD CARD OK
WIFI START
==============================
SSID: ParaxFace2
IP: 192.168.4.1
==============================
Web server started
Open: http://192.168.4.1
```

### Interpreting Errors

- **"TFT INIT ERROR"** - Display not responding, check wiring
- **"SD CARD ERROR!"** - SD card not found, check connections
- **"WIFI AP ERROR!"** - WiFi startup failed, restart device
- **"JPEG ERROR"** - Image file is corrupt, try different image
- **"DRAW ERROR"** - Image rendering failed, check memory

---

## License

This project is open source. Feel free to modify and use for your purposes.

---

## Next Steps

After testing the current version successfully:

1. **Report test results** - Tell me which tests passed/failed
2. **Identify issues** - Let me know any problems encountered
3. **Proceed to Phase 2** - I'll begin building the eye animation system

Development will proceed step-by-step with:
- Complete documentation of every change
- Full code listings for every file
- Testing instructions for each phase
- Clear error reporting and fixes

---

## Contact & Support

For issues or questions:
1. Check **Troubleshooting** section above
2. Review **Serial Monitor** output
3. Verify hardware connections
4. Check that all libraries are installed correctly
5. Try restarting ESP8266 and Arduino IDE

---

**Last Updated:** 2026-08-28  
**Version:** 1.0 (Current - Image Viewer)  
**Next Phase:** 2.0 (Procedural Eye Animation)
