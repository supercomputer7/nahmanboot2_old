# nahmanboot2 - NahmanOS Bootloader Revision 2
NahmanBoot is a bootloader intended for x86 PCs (only).
Written in x86 assembly & C++.

Check SUPPORT.md for support list (what is planned, what will not be implemented).

## Some thoughts in the right way
* Assume bare minimum working hardware - During development, any assumptions that can be made in High level programming are not valid here. As this code is interfacing hardware directly, sanity checks are a must. Relying on firmware should be a last resort in general, and code should check for bugs if any piece of firmware code is being used.

* Code is not perfect - If you read SUPPORT.md file, you might think to yourself that the code will be organized & coded perfectly. The truth is that it is not. I (the first "contributor") am coding and building this project for many purposes, mainly for fun and for learning about computers. No one is perfect, and because of that, one should not assume that his code (or of someone else) is perfect. Project's code will not use all the advanced methods when interfacing hardware, like super fast DMA transfers, fastest shiny graphics drawing etc. However, one shouldn't mistake to think that the produced code will use ugly hacks and lazy methods - the produced code will use general & reliable methods as much as possible, corresponding to "Assume bare minimum working hardware" principle.

* Bootmanager and that's it - This project, however fancy and advanced it might be in some point, will remain a Bootmanager, like GRUB. This project is not intended for being an Operating System and if you want to explore OSDev, I highly recommend to go https://wiki.osdev.org (I learned a lot from this website) and check the SerenityOS project at https://github.com/SerenityOS/serenity (which is a very nice project).

* Advanced Flexibility in Mind - The project will strive to produce a flexible code so changes can be made with ease.

* Minimum interference in boot sequence - Before transfering the execution to the kernel with appropriate parameters delievered, the bootloader will try to change back every crirical setting in hardware that has been changed while the bootloader loaded itself and other things.

## Installation

For now there is no userspace utility. Once it will be written, you'll be able to install with that utility.

A video uploaded by me to YouTube, showing NahmanOS bootloader (old revision) booting SerenityOS:  
https://www.youtube.com/watch?v=MtyUKLpjnS8
