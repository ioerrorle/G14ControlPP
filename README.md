# G14ControlPP
ASUS services replacement for Zephyrus G14 laptops

##Introduction
Initially, main goal was to bring back PgUp/PgDown/Home/End keys functionality.
Problem with it is that ASUS Optimization handles all the Fn+key presses and does what it wants, so, I realized that I need to reproduce at least minimum functionality of the service and disable it for good.
So, here we go.
##tl;dr of laptop firmware philosophy
There are 3 possible ***Power sources*** - **Battery**, **180W charger** and **USB-C 60W charger**. Laptop has 3 ***Armoury Crate*** plans: **"Windows/Performance",  "Turbo/Manual"** and **"Silent"**.
Each plan has it's own **CPU power settings** (how much power can a CPU draw at max and for how long) and **Fans curves** (what speed will CPU/GPU fan have at certain temperature of a corresponding chip) for each **Power source**. in addition, fan curves and CPU settings can be applied manually. This utility allows you to do it.

##Features

Works:
- shows current APU TDP, temperature and fans RPM
- Fn+C/V control kbd brightness.
- Fn+Arrow keys work as intended in normal laptops (PgUp/PgDown/etc.)
- Fn+F7/F8 control LCD brightness

Roadmap:
1) ~~!!!DISABLE KBD BACKLIGHT WHEN LID IS CLOSED!!!~~ (done)
2) ~~Settings storage for various things~~ (done)
3) ~~Fan curves editor~~ (done)
4) ~~Multiple fan curves~~ (done)
5) ~~Handle "Mute" button~~(done, mutes ALL mics, not only active)
6) ~~Battery health function~~ (done)
7) ~~Functional CPU settings tab~~ (done)
8) ~~Profiles tab (CPU, power plan, fan curve settings for each power supply)~~ (done)
9) ~~Handle "Toggle Touchpad" button~~ (done)
10) ~~Handle "Sleep" button~~ (done)
11) ~~Handle "Plane Mode" button~~ (done)
12) ~~Do something with "Fan Mode" button (circular switch between modes?)~~ (done)
13) Make an interface for customizing Fn buttons
14) Handle "ROG" button
15) Save battery max charge setting, and apply it on start
16) Refactor settings storage
17) Fix fan curves tab
18) Handle all the errors that are possible to catch
19) **BETA STARTS HERE**
20) Make an OSD and notifications on certain events
21) Make an interface for AniMe matrix
