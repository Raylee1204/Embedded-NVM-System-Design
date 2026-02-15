## Introduction
此內容為Linux_Kernel_Driver的測試內容回報且如何實作之完整內容
---
### Qustion 1
#### Setting up the Environment:
```
uname -r

sudo apt install build-essential linux-headers-$(uname -r)
```
<img width="663" height="205" alt="image" src="https://github.com/user-attachments/assets/1c9f50b1-2334-4698-9700-1c7c7f45373e" />


----
#### Prepare a simple “Hello World” kernel module. 

1. Create an archive called ```hello.c``` and put the following code in it
```
vim hello.c
```
```
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student Name");
MODULE_DESCRIPTION("A simple Hello World module");
static int __init hello_init(void) {
 printk(KERN_INFO "Hello, world!\n");
 return 0;
}
static void __exit hello_exit(void) {
 printk(KERN_INFO "Goodbye, world!\n");
}
module_init(hello_init);
module_exit(hello_exit);

```
2. Create a file called Makefile and put the following code into it
```
vim Makefile
```
```
obj-m += hello.o

all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```
3. Compile the module
```
make
```
----
#### Loading and unloading the module
1. Use the insmod command to load the module
```
sudo insmod hello.ko
```
2. Use the rmmod command to uninstall the module
```
sudo rmmod hello
```
3. Use dmesg to view the core log, confirm that the module has been successfully loaded, and then uninstall it successfully
```
dmesg | tail
```
<img width="1831" height="275" alt="image" src="https://github.com/user-attachments/assets/0c4a8cf0-e1e9-4db1-bd51-b96d33a283c8" />



----
#### Recompiling the kernel to include the module
1. Download the Linux Kernel Archives (https://www.kernel.org) or using
```
wget -P ~/ https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.15.137.tar.xz
```
2. Decompress files to /usr/src
```
tar -xvf linux-5.15.137.tar.xz -C /usr/src
```
3. Copy ```hello.c``` to ```/usr/src/linux-5.15.137/drivers/misc/```
```
cp /usr/src/linux-headers-6.8.0-48-generic/hello/hello.c /usr/src/linux-5.15.137/drivers/misc/

```
4. Change the ```Makefile``` of the ```/drivers/msic/ ```address
(Add at the bottom)
```
obj-y                           += hello.o
```
<img width="1498" height="953" alt="image" src="https://github.com/user-attachments/assets/90640443-bbf3-4196-8d7b-b9923918a702" />


```
make localmodconfig
make -j$(nproc)
make modules_install
make install

sudo reboot
```
5. Check kernel version
```
uname -r
```
<img width="789" height="150" alt="image" src="https://github.com/user-attachments/assets/24d9e48f-fd81-417b-887b-5cc85bc6297f" />


6. Use ```dmesg``` to view core logs
```
sudo su
dmesg | grep "Hello, world!"
```
<img width="713" height="124" alt="image" src="https://github.com/user-attachments/assets/b11ad9e9-161e-48b9-871d-f821f3733856" />


----
#### Demonstration
1. 使用```sudo modprobe hello```手動輸入模組
```
sudo modprobe hello
```
<img width="795" height="609" alt="image" src="https://github.com/user-attachments/assets/707610f0-480b-40c0-a2b3-5e89b1339082" />


輸入並沒有回報錯誤，是因為模組本身已存在於kernel中，而並非完全沒反應。
+ ```modprobe``` 適合處理有依賴的模組加載需求，並且可以用來自動加載或移除模組（只針對可加載模組）。
+ ```insmod``` 用於手動加載一個單獨模組，且不會處理依賴問題。
如果模組內建於 kernel，那麼它會隨 kernel 自動加載，無法通過 ```modprobe``` 或 ```insmod``` 控制加載或卸載。
<img width="703" height="119" alt="image" src="https://github.com/user-attachments/assets/a337181e-58ff-469f-ad21-6a1b80acbcfc" />

---
#### Issues
**If kernel version doesn't update**
```
vim /etc/default/grub

GRUB_TIMEOUT_STYLE: hidden -> menu
GRUB_TIMEOUT: 0 -> -1

update-grub
reboot
```
**Make Failed –> Modify .config**
```
can't read modules.order: No such file or directory
make:*** [Makefile:1544 __modinstpre] Error2 
```
```
sudo vim .config

CONFIG_SYSTEM_TRUSTED_KEYS = ""
CONFIG_SYSTEM_REVOCATION_KEYS = ""
```
**Make modules failed –> Install dwarves**
```
.tmp_vmlinux.btf: pahole(pahole) is not available
Failed to generate BTF for vmlinux
Try to disable CONFIG_DEBUG_INFO_BTF
make:***[Makefile:1227:vmlinux] Error1
```
```
sudo apt-get install dwarves
```
