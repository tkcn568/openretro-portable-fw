# ADR Candidates — OpenRetro Portable Firmware

This document lists potential Architecture Decision Records for the firmware project. Each entry captures an open architectural question, with space for answers as you organize your notes.

**Status:** Work in progress. Use this to identify which decisions warrant formal ADRs.

---

## Known Constraints & Context

The following constraints should inform all firmware ADRs:

- **Battery life target:** 6–8h runtime on 2×AA NiMH/alkaline
- **Real-time constraints:** Display refresh (LTDC VSYNC), audio streaming (I²S DMA), cartridge communication (SPI)
- **Memory constraints:** H753 console has 1MB SRAM; G071 cartridge has 36KB SRAM
- **Form factor:** Handheld with cartridge slot (GBA-like insertion)
- **Dual-CPU system:** Console (H753) and cartridge (G071) communicate via SPI; each has its own firmware

---

## Architecture & Organization

### ADR: Separate Console vs Cartridge Firmware

**Question:** Should console and cartridge have entirely separate codebases, or should they share a common base? What are the implications for maintenance, build system, and team structure?

**Known context:**
- Console firmware: runs on H753, manages display, audio, SD, battery
- Cartridge firmware: runs on G071, manages game logic and cart↔console communication
- Both are embedded systems, but with very different responsibilities
- Shared protocols/interfaces between them

**Rationale:** (to be filled in)

**Code sharing strategy:** (to be filled in)

---

### ADR: Bootloader vs Direct Application Launch

**Question:** Does console need a bootloader? Do production cartridges? Should dev carts support DFU firmware updates?

**Known context:**
- Console: initial bring-up from cold start
- Dev cart: USB-FS peripheral, DFU support likely needed for iteration
- Production cart: programmed once at manufacture, no update capability?
- Bootloader adds complexity; direct launch simpler but no OTA

**Rationale:** (to be filled in)

**Update mechanism per device:** (to be filled in)

---

## Communication Protocol

### ADR: Cart-to-Console Message Protocol Design

**Question:** What messages pass between console and cartridge? Request/response or event-driven? How are errors and retries handled?

**Known context:**
- Console is SPI master, cartridge is SPI slave
- Console controls game clock and frame timing
- Cartridge needs to signal events (button press, SD data ready, etc.)
- SPI is full-duplex; both directions can carry data simultaneously

**Rationale:** (to be filled in)

**Message primitives:** (to be filled in)

**Error handling:** (to be filled in)

---

### ADR: Real-Time Packet Scheduling on Shared SPI

**Question:** How do game frames, SD reads, and audio samples coexist on the shared SPI bus? Are there message priorities or time-slicing?

**Known context:**
- Game frames: console-driven, tied to LTDC VSYNC
- SD reads: console-initiated on cartridge (likely deferred, not blocking)
- Audio samples: console-pushed via I²S (separate from SPI)
- SPI bandwidth: ~10+ Mbps; must not block audio DMA

**Rationale:** (to be filled in)

**Scheduling strategy:** (to be filled in)

**Latency budget for cartridge responses:** (to be filled in)

---

## Memory & Storage

### ADR: SD Card Abstraction Layer

**Question:** How does the SD abstraction differ between console (SDMMC 4-bit SDIO) and cartridge (SPI mode)? Single unified interface or hardware-specific drivers?

**Known context:**
- Console: SDMMC1 peripheral, 4-bit SDIO mode, potentially 50+ Mbps
- Cartridge: SPI-based SD access via cart↔console SPI (much slower)
- Two very different hardware stacks; code reuse minimal?
- Cartridge SD is likely only for cartridge-local storage (save files, etc.)

**Rationale:** (to be filled in)

**Driver architecture:** (to be filled in)

---

### ADR: H753 Memory Layout and Allocation

**Question:** How are ITCM (instruction), DTCM (data), AXI RAM, and external memory divided on H753? Where does the frame buffer live? Cache behavior?

**Known context:**
- H753 SRAM map: DTCM 128KB, ITCM 128KB, AXI 512KB, others
- Frame buffer: 320×240×2 bytes ≈ 150KB
- Tight budget: game code, audio buffers, display list, SD cache, stack all compete
- ITCM for instruction fetch hotspots; DTCM for data hotspots

**Rationale:** (to be filled in)

**Section placement strategy:** (to be filled in)

**Heap / stack allocation:** (to be filled in)

---

### ADR: Game/Cartridge Storage Conventions

**Question:** How is cartridge ROM organized? Where do save files live? Is there a reserved region for the bootloader?

**Known context:**
- Cartridge has SDIO or SPI-mode SD; exact capacity TBD
- Save files: on cartridge SD, console SD, or both?
- Bootloader: DFU vs fixed app only?

**Rationale:** (to be filled in)

**ROM layout:** (to be filled in)

---

## Real-Time Constraints

### ADR: Display Refresh and Frame Timing

**Question:** How are LTDC interrupts (VSYNC) synchronized with application code? Does the firmware interrupt on every line, every frame, or use a different strategy?

**Known context:**
- H753 LTDC: 320×240@60Hz (typical)
- VSYNC period: ~16.7ms
- Game logic, audio buffering, and SD must fit within or between frames
- Interrupt-driven refresh vs polled loop

**Rationale:** (to be filled in)

**Interrupt strategy:** (to be filled in)

**Frame budget:** (to be filled in)

---

### ADR: Audio I²S DMA Ring Buffer Strategy

**Question:** How is the audio stream kept fed? Ring buffer, ping-pong, or other DMA strategy? What happens on underrun?

**Known context:**
- I²S (or SAI) peripheral on H753
- PCM5102A DAC expects continuous I²S data
- DMA fed by audio decoder or sample generator
- Underrun = audio dropout / pop

**Rationale:** (to be filled in)

**Buffer strategy:** (to be filled in)

**Underrun detection and recovery:** (to be filled in)

---

### ADR: Cartridge SPI Slave Responsiveness

**Question:** What is the maximum acceptable latency for cartridge to respond to console SPI reads? Are interrupts or polling used?

**Known context:**
- Cart is SPI slave; must respond within a transaction window
- Console typically has spare compute cycles waiting for cart data
- Slow responses could cause display glitches or audio buffer misses

**Rationale:** (to be filled in)

**Latency requirements:** (to be filled in)

**Implementation strategy (interrupt vs polling):** (to be filled in)

---

## Initialization & Configuration

### ADR: H753 Clock Tree and PLL Configuration

**Question:** Why a specific SYSCLK speed, PLL multipliers, and AHB/APB prescalers? Is USB-FS crystal-less mode viable?

**Known context:**
- H753 can run up to 480 MHz
- LTDC bandwidth increases with clock speed (but so does power draw)
- USB-FS + CRS (clock recovery) may eliminate need for external crystal?
- Power budget: battery life constraint may limit clock speed

**Rationale:** (to be filled in)

**Chosen SYSCLK and PLL settings:** (to be filled in)

**USB-FS strategy:** (to be filled in)

---

### ADR: G071 Clock Configuration and USB Enumeration

**Question:** Does the cartridge need an external crystal for USB-FS reliable enumeration, or is HSI + CRS adequate for the dev cart?

**Known context:**
- Dev cart: USB-FS for DFU/debug
- Production cart: no USB, HSI sufficient
- HSI ±2% may or may not meet USB spec without CRS
- External crystal adds cost and board space to dev cart only

**Rationale:** (to be filled in)

**Dev vs production differentiation:** (to be filled in)

---

### ADR: Interrupt Priority Assignment

**Question:** How are interrupt priorities assigned to LTDC, SAI/I²S, SPI, USB, and systick? Document the rationale and any critical nesting.

**Known context:**
- Multiple real-time sources: display refresh, audio, SPI slave
- Priorities affect response latency and jitter
- Some interrupts should preempt others (audio > display > USB)

**Rationale:** (to be filled in)

**Priority table:** (to be filled in)

**Critical nesting scenarios:** (to be filled in)

---

## Development & Debug

### ADR: Debug Interface (SWD vs JTAG)

**Question:** Why SWD on development carts? Why use 1.27mm pitch (compact) vs 2.54mm (breadboard-friendly)?

**Known context:**
- SWD: serial wire debug, 2-pin (SWDIO, SWCLK) vs JTAG's 4+
- 1.27mm pitch: small, compact, requires special header
- 2.54mm pitch: standard breadboard-friendly connectors
- Cart form factor may constrain space

**Rationale:** (to be filled in)

**Pitch selection:** (to be filled in)

---

### ADR: Console vs Dev-Cart Firmware Divergence

**Question:** What features are dev-only (USB gadget, logging over SPI)? Should this be compile-time #ifdef, runtime detection, or separate binaries?

**Known context:**
- Dev cart: USB peripheral, DFU, debug logging
- Production cart: no USB, minimal flash/RAM
- Code size constraints on G071

**Rationale:** (to be filled in)

**Configuration mechanism:** (to be filled in)

---

### ADR: Logging and Telemetry Strategy

**Question:** How is firmware logging done during development and debug? Over USB, SPI, UART, or SD card?

**Known context:**
- No external UART pins on console (likely)
- Cart has USB on dev version
- Performance impact on battery life and real-time constraints

**Rationale:** (to be filled in)

**Debug output channels:** (to be filled in)

---

## Abstraction & Driver Design

### ADR: Unified vs Hardware-Specific SPI Drivers

**Question:** For SPI (console-facing slave vs cart SD master), should there be one shared SPI driver or separate implementations?

**Known context:**
- Console: SPI1 or similar, slave mode, cart-facing, high priority
- Cart: SPI1 or similar, master mode, SD-facing
- Different configuration, different interrupt handling
- Code reuse minimal

**Rationale:** (to be filled in)

**Driver architecture:** (to be filled in)

---

### ADR: DMA Configuration and Ring Buffer Patterns

**Question:** Is there a standard pattern for DMA-backed peripherals, or are ring buffers custom per subsystem?

**Known context:**
- LTDC: frame buffer DMA (may be automatic once configured)
- I²S: audio sample DMA (ring buffer needed)
- SDMMC: block DMA (transfer completion interrupt)
- SD SPI on cart: no DMA (G071 may lack DMA for SPI)

**Rationale:** (to be filled in)

**Standard patterns:** (to be filled in)

---

## Cross-Cutting Decisions

### ADR: Game Cartridge Format and Compatibility

**Question:** Is the firmware targeting binary compatibility with GBA cartridges, or is a custom format acceptable? What implications does this have for ROM layout and boot sequence?

**Known context:**
- Hardware is GBA-form-factor compatible
- But firmware doesn't need to emulate GBA — it's a standalone system
- Format choice affects game porting effort

**Rationale:** (to be filled in)

**ROM format:** (to be filled in)

---

### ADR: Firmware Update Mechanism for Console and Cartridge

**Question:** Post-manufacture, how are console and cartridge firmware updated? OTA, JTAG, bootloader, or one-time programming?

**Known context:**
- Console: SD card possibly? Bootloader? DFU?
- Dev cart: USB-FS + DFU (likely)
- Production cart: one-time program at factory? No field updates?

**Rationale:** (to be filled in)

**Update paths:** (to be filled in)

---

### ADR: Error Handling and Recovery Strategy

**Question:** How should the firmware handle runtime errors (SD corruption, SPI timeout, audio underrun)? Graceful degradation, reset, or panic?

**Known context:**
- Battery-powered system; hard power-off is an option
- User expectations: no hang, ideally recoverable
- Game cart failure shouldn't crash console

**Rationale:** (to be filled in)

**Recovery strategies per subsystem:** (to be filled in)

---
