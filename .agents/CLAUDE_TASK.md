# Nhiệm vụ: viết firmware cho USB Touchboard (STM32F429I-DISC1)

Bạn là agent code trên một project STM32 đã được cấu hình sẵn bằng STM32CubeMX + TouchGFX.
Nhiệm vụ của bạn là **chỉ viết phần code ứng dụng còn thiếu**, KHÔNG cấu hình lại peripheral,
KHÔNG chỉnh file `.ioc`. Làm việc tăng dần, giải thích ngắn gọn mỗi thay đổi, và build được bằng STM32CubeIDE.

## 1. Bối cảnh dự án

Thiết bị biến board **STM32F429I-DISC1** (MCU **STM32F429ZIT6U**) thành một **USB Touchboard** cắm vào PC Windows:
- Màn hình cảm ứng 240×320 (ILI9341 + STMPE811) làm **touchpad**: kéo ngón để di chuyển con trỏ chuột.
- 2 nút vật lý: **PG2 = chuột trái**, **PG3 = chuột phải**.
- Màn hình có 2 chế độ, chuyển qua lại bằng nút trên màn hình (đã làm trong TouchGFX):
  - `ScreenMain`: vùng touchpad.
  - `ScreenFN`: bảng phím chức năng gồm 4 mũi tên, tăng/giảm âm lượng, Play/Pause.
- Thiết bị trình diện với host là **USB Custom HID composite** (chuột + bàn phím + phím media).

## 2. Trạng thái hiện tại (ĐÃ XONG — đừng làm lại)

- TouchGFX + LCD + cảm ứng + FreeRTOS (CMSIS_V2) đã chạy được.
- USB `USB_OTG_HS` = Device_Only, Internal FS PHY (PB14=DM, PB15=DP), Full-Speed. Clock USB = 48 MHz.
- USB class = **Custom HID** (file interface `usbd_custom_hid_if.c` đã được sinh).
- USB OTG HS global interrupt đã bật trong NVIC.
- GPIO: **PA0** (không dùng), **PG2/PG3** = GPIO_Input, pull-up (nút nối GND → bấm đọc mức 0).
- FreeRTOS đã có sẵn task `GUI_Task` (TouchGFX) và `defaultTask`.
- Đã thêm task `buttonTask` (entry `StartButtonTask`) và `usbHidTask` (entry `StartUsbHidTask`).
- Đã thêm queue **`hidQueue`** (handle CMSIS: `hidQueueHandle`), size 8, item size = `uint32_t` (4 byte).
- TouchGFX Designer: `ScreenMain` có `touchArea` (Box) + nút `btnGoFN` (Change Screen → ScreenFN).
  `ScreenFN` có 7 nút + `btnGoPad` (Change Screen → ScreenMain). 7 nút đã có Interaction gọi các
  virtual function: `arrowUpClicked`, `arrowDownClicked`, `arrowLeftClicked`, `arrowRightClicked`,
  `volUpClicked`, `volDownClicked`, `playPauseClicked`.

## 3. Kiến trúc & quy ước (BẮT BUỘC theo đúng)

Mọi nguồn sự kiện (touch drag, nút vật lý, nút trên màn hình) đẩy một struct 4 byte vào `hidQueue`.
Chỉ `usbHidTask` đọc queue và gọi USB. Không task nào khác được gọi `USBD_CUSTOM_HID_SendReport`.

Struct sự kiện (tạo file header dùng chung, ví dụ `Core/Inc/hid_events.h`), phải đúng 4 byte:

```c
typedef enum { EV_MOUSE = 0, EV_KEYBOARD = 1, EV_CONSUMER = 2 } HID_EvType_t;
typedef struct {
    uint8_t type;   // HID_EvType_t
    uint8_t a;      // MOUSE: buttons (bit0=trái, bit1=phải) | KEYBOARD/CONSUMER: usage code
    int8_t  b;      // MOUSE: deltaX | KEYBOARD/CONSUMER: pressed (1=nhấn, 0=nhả)
    int8_t  c;      // MOUSE: deltaY | (không dùng cho loại khác)
} HID_Event_t;
```
Thêm `_Static_assert(sizeof(HID_Event_t) == 4, "HID_Event_t phai 4 byte");`.

Bảng usage HID cần dùng:
- Chuột: Generic Desktop, Button1 (trái), Button2 (phải), X/Y **tương đối** (relative).
- Bàn phím (Keyboard page 0x07): Up=0x52, Down=0x51, Left=0x50, Right=0x4F.
- Consumer Control (page 0x0C): Volume Increment=0xE9, Volume Decrement=0xEA, Play/Pause=0xCD.

## 4. Việc cần code (làm theo thứ tự này)

**Task 1 — HID report descriptor (nền tảng).**
Viết mảng `CUSTOM_HID_ReportDesc_FS[]` trong `usbd_custom_hid_if.c` thành **composite 3 report có Report ID**:
- Report ID 1 = Mouse: 1 byte buttons (2 nút + padding) + 2 byte X,Y relative (int8).
- Report ID 2 = Keyboard: report chuẩn (1 byte modifier + 1 byte reserved + 6 byte keycode), hoặc rút gọn
  1 keycode — miễn nhất quán với hàm gửi.
- Report ID 3 = Consumer Control: 1 byte bitmap (bit0=Vol+, bit1=Vol−, bit2=Play/Pause) hoặc dùng usage array — tùy bạn, miễn nhất quán.
Cập nhật kích thước endpoint IN (`CUSTOM_HID_EPIN_SIZE` trong `usbd_conf.h`) cho ≥ report dài nhất
(gồm cả byte Report ID). Ghi rõ layout từng report bằng comment.

**Task 2 — hàm gửi report.**
Trong `usbd_custom_hid_if.c` (hoặc nơi hợp lý), viết 3 hàm:
`SendMouse(uint8_t buttons, int8_t x, int8_t y)`, `SendKey(uint8_t keycode, uint8_t pressed)`,
`SendConsumer(uint8_t usage, uint8_t pressed)`. Mỗi hàm dựng buffer với **byte đầu = Report ID**
tương ứng, rồi gọi `USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, buf, len)`.

**Task 3 — `StartUsbHidTask`** (trong `freertos.c`, vùng USER CODE):
Vòng lặp `osMessageQueueGet(hidQueueHandle, &ev, NULL, osWaitForever)`, `switch(ev.type)`:
- EV_MOUSE → `SendMouse(ev.a, ev.b, ev.c)`.
- EV_KEYBOARD → gửi **press** rồi **release**: `SendKey(ev.a, 1)`; `osDelay(15)`; `SendKey(0, 0)`.
- EV_CONSUMER → tương tự: `SendConsumer(ev.a, 1)`; `osDelay(15)`; `SendConsumer(0, 0)`.
(Nút trên màn hình là chạm tức thời nên bàn phím/consumer cần tự nhả để host chỉ nhận 1 lần nhấn.)
Thêm `osDelay` nhỏ để không gửi nhanh hơn polling interval của endpoint.

**Task 4 — `StartButtonTask`** (trong `freertos.c`, vùng USER CODE):
Poll PG2/PG3 mỗi ~15 ms, có **debounce** (đọc ổn định 2–3 lần), phát hiện đổi trạng thái.
Ghép trạng thái 2 nút thành byte buttons (bit0=trái từ PG2, bit1=phải từ PG3, active-low),
và **chỉ khi có thay đổi** thì `osMessageQueuePut(hidQueueHandle, &(HID_Event_t){EV_MOUSE, buttons, 0, 0}, 0, 0)`.

**Task 5 — TouchGFX (dưới `TouchGFX/gui`).**
Sửa các class `Model`, `Presenter`, `View` **của tôi** (không sửa file `*ViewBase`/generated):
- `Model` (Model.cpp/.hpp): thêm `moveMouse(int16_t dx,int16_t dy)`, `sendKey(uint8_t code)`,
  `sendConsumer(uint8_t usage)`. Trong Model, khai báo queue với C linkage:
  `extern "C" { #include "cmsis_os2.h" extern osMessageQueueId_t hidQueueHandle; }` và include `hid_events.h`.
  - `moveMouse`: **kẹp** dx,dy về [-127,127] rồi đẩy `{EV_MOUSE, 0, (int8_t)dx, (int8_t)dy}`.
  - `sendKey`: đẩy `{EV_KEYBOARD, code, 1, 0}`.
  - `sendConsumer`: đẩy `{EV_CONSUMER, usage, 1, 0}`.
- `ScreenMainPresenter` / `ScreenFNPresenter`: thêm hàm chuyển tiếp gọi xuống `model->...`.
- `ScreenMainView`: override `handleDragEvent(const DragEvent&)` → gọi `presenter->moveMouse(evt.getDeltaX(), evt.getDeltaY())` (nhớ gọi bản base trước).
- `ScreenFNView`: override 7 virtual function đã tạo trong Designer, map:
  - `arrowUpClicked` → `presenter->sendKey(0x52)`; Down→0x51; Left→0x50; Right→0x4F.
  - `volUpClicked` → `presenter->sendConsumer(0xE9)`; volDown→0xEA; playPause→0xCD.

## 5. Ràng buộc quan trọng

- Chỉ chỉnh sửa trong các block `/* USER CODE BEGIN ... */ ... /* USER CODE END ... */` ở file do CubeMX sinh
  (main.c, freertos.c, usbd_custom_hid_if.c), để lần Generate sau không xóa mất.
- KHÔNG sửa file TouchGFX generated (`*Base`), chỉ sửa lớp dẫn xuất trong `TouchGFX/gui`.
- KHÔNG cấu hình lại peripheral trong code; coi như `.ioc` đã đúng.
- Report descriptor (Task 1) và các hàm gửi (Task 2) phải khớp layout byte tuyệt đối.
- Nếu tên file/screen/biến thực tế khác với giả định ở trên, hãy **tìm trong repo** và điều chỉnh cho khớp;
  nếu có mơ hồ thật sự thì hỏi lại trước khi sửa hàng loạt.

## 6. Tiêu chí hoàn thành

1. Build STM32CubeIDE không lỗi.
2. Cắm cổng USB user (CN6) vào Windows → hiện là thiết bị HID, không báo lỗi enumeration.
3. Kéo ngón trên `ScreenMain` → con trỏ chuột di chuyển; PG2/PG3 → click trái/phải.
4. Nút `btnGoFN`/`btnGoPad` chuyển màn mượt.
5. Trên `ScreenFN`: 4 mũi tên gõ đúng phím; Vol+/Vol−/Play-Pause điều khiển media Windows.

## 7. Cách làm việc

Làm từng Task theo thứ tự, sau mỗi Task tóm tắt file đã đổi và lý do. Ưu tiên đúng và tối giản
hơn là thêm tính năng. Bắt đầu bằng việc đọc cấu trúc repo (đặc biệt `Core/`, `USB_DEVICE/`, `TouchGFX/gui/`)
để xác nhận tên file và handle thực tế trước khi viết.
