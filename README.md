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
6) Battery health function
7) Handle "Toggle Touchpad" button
8) Handle "Sleep" button
9) Handle "Plane Mode" button
10) Do something with "Fan Mode" button (circular switch between modes?)
11) Handle "ROG" button
12) Make an interface for AniMe matrix

Cmon, ASUS, it's really not this hard to make small dedicated software.
