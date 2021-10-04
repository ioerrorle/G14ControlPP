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
2) Settings storage for various things
3) Fan curves editor
4) Multiple fan curves
5) Handle "Mute" button
6) Handle "Toggle Touchpad" button
7) Handle "Sleep" button
8) Handle "Plane Mode" button
9) Do something with "Fan Mode" button (circular switch between modes?)
10) Handle "ROG" button
11) Make an interface for AniMe matrix

Cmon, ASUS, it's really not this hard to make small dedicated software.
