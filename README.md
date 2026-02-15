# Embedded NVM System Design: Linux Kernel & MSP430 FRAM

![Platform](https://img.shields.io/badge/Platform-Linux%20Kernel%205.x-yellow) ![Device](https://img.shields.io/badge/Device-MSP430FR4133-red) ![Tech](https://img.shields.io/badge/Tech-FRAM%20Non--Volatile%20Memory-blue) ![Language](https://img.shields.io/badge/Language-Embedded%20C-green)

## 📖 Project Overview
本專案為 **嵌入式非揮發性記憶體 (NVM) 系統設計** 的實作紀錄。
專案旨在探討 **Linux 核心空間 (Kernel Space)** 的驅動程式開發流程，以及 **超低功耗微控制器 (MSP430)** 結合 **FRAM (鐵電隨機存取記憶體)** 的資料保存技術。透過本專案，驗證了從作業系統底層到硬體感測應用的完整嵌入式開發路徑。

This project demonstrates the implementation of Linux Kernel Modules and Ultra-Low Power Embedded Systems using FRAM technology. It bridges the gap between OS-level driver management and hardware-level non-volatile data logging.

## 🚀 System Architecture

專案分為兩大模組，分別對應 Host 端 (OS) 與 Device 端 (MCU) 的技術實作：

| Module | Technical Focus | Implementation Highlights |
| :--- | :--- | :--- |
| **01. Linux Kernel** | **Driver Lifecycle & Build System** | LKM (Loadable Kernel Module), Kernel Recompilation, dmesg debugging |
| **02. MSP430 FRAM** | **NVM Data Logging & Power Mgmt** | Power-loss Recovery, Memory Mapping, Interrupt Handling |

---

## 🛠️ Module 01: Linux Kernel Space Development

本模組展示如何在 Linux 核心空間運行程式碼，並探討驅動程式的生命週期管理。



### 1. Kernel Module Lifecycle (LKM)
實作標準的動態載入模組 (Loadable Kernel Module)：
* **Init & Exit**: 使用 `module_init` 與 `module_exit` 巨集管理資源的申請與釋放。
* **Logging**: 使用 `printk` 將訊息輸出至 Kernel Ring Buffer (可透過 `dmesg` 查看)。
* **Verification**:
    ```bash
    sudo insmod hello.ko  # 掛載模組
    dmesg | tail          # 查看 "Hello, world!"
    sudo rmmod hello      # 卸載模組
    ```

### 2. Kernel Integration (Recompilation)
模擬實際產品開發流程，將客製化驅動程式直接 **編譯進核心 (Built-in)**，而非作為外部模組掛載。
* **Workflow**: 修改 `/drivers/misc/Makefile` 與 `Kconfig`，重建 Linux 5.15.137 核心映像檔。
* **Result**: 系統啟動時自動載入驅動，無需手動掛載。

---

## 🌡️ Module 02: MSP430 FRAM Temperature Logger

本模組利用 **TI MSP430FR4133** 的 FRAM 特性，建構一個具備 **斷電恢復 (Power-loss Recovery)** 功能的溫度記錄器。



### 1. FRAM Technology Implementation
FRAM (Ferroelectric RAM) 結合了 RAM 的寫入速度與 Flash 的非揮發性。本系統利用此特性記錄即時溫度。
* **Memory Mapping**:
    * `0x1830`: **Snapshot Address** (存放最後一筆溫度)
    * `0x1868`: **Valid Flag** (資料完整性標記)
* **Critical Section**: 寫入 FRAM 前需解除 `DFWP` (Data FRAM Write Protection) 保護，寫入後立即上鎖以確保資料安全。

### 2. Feature Demonstration
* **Data Persistence (斷電不遺失)**:
    * 系統偵測到重啟後，自動檢查 FRAM 有效標記。
    * 若標記有效，則從 `0x1830` 讀回斷電前的溫度數值並顯示於 LCD。
    * [📺 **Demo Video: FRAM Data Persistence**](https://youtube.com/shorts/wov0Okp8v8g?feature=share)
* **Threshold Alert (高溫警示)**:
    * 設定溫度閾值 (Threshold)，當感測溫度超過設定值時，觸發 GPIO 控制 LED 警示。
    * [📺 **Demo Video: Threshold Alert System**](https://youtube.com/shorts/tRiRJBfxJiM?feature=share)

---

## 🔧 Technical Challenges & Troubleshooting

### Linux Kernel Build
* **Issue**: `can't read modules.order: No such file or directory`
    * **Solution**: 清除 `.config` 中的 `CONFIG_SYSTEM_TRUSTED_KEYS` 設定。
* **Issue**: `BTF: .tmp_vmlinux.btf: pahole is not available`
    * **Solution**: 安裝 `dwarves` 套件以支援 BTF 除錯資訊生成。

### MSP430 FRAM Access
* **Issue**: 無法寫入 FRAM 或寫入後數值未更新。
    * **Solution**: MSP430 預設開啟 MPU (Memory Protection Unit)。必須在寫入操作前後明確切換 `SYSCFG0` 暫存器的 `DFWP` 位元。

---
*Author: [Your Name]*
*Tools: Ubuntu Linux, TI Code Composer Studio (CCS)*
