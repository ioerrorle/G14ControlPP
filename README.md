# G14ControlPP
ASUS services replacement for Zephyrus G14 laptops

Initially, main goal was to bring back PgUp/PgDown/Home/End keys functionality.
Problem with it is that ASUS Optimization handles all the Fn+key presses and does what it wants, so, I realized that I need to reproduce at least minimum functionality of the service and disable it for good.
So, here we go.
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
12) Do something with "Fan Mode" button (circular switch between modes?)
13) Handle "ROG" button
14) Make an interface for AniMe matrix

Cmon, ASUS, it's really not this hard to make small dedicated software.
