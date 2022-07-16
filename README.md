# Coccinellidae Web Browser
The Coccinellidae web browser. The goal is to create a traditional browser UI (how things was before Chrome).

This is a fork of [Ladybird](https://github.com/awesomekling/ladybird).

This is a personal fork for my own enjoyment and learning :^)

Hardcoded for qt5 until I get an environment that can run qt6.

### How does Coccinellidae differ from Ladybird?
- [x] Search field (Default search engine Brave)
- [x] Statusbar
- [ ] Large fancy buttons
- [ ] Tabs above content (not above toolbar)
- [x] Starting maximized
- [x] Platform independent shortcuts (QKeySequence::StandardKey)

Menu:
- [x] Edit
- [ ] Edit: Cut
- [ ] Edit: Copy
- [ ] Edit: Paste
- [ ] Edit: Select All
- [x] Inspect -> View
- [x] Go
- [ ] Go: Back
- [ ] Go: Forward
- [ ] Go: Home
- [x] About
- [ ] About: Help
- [ ] About: About Coccinellidae

### Build instructions
Same as [Ladybird](https://github.com/awesomekling/ladybird) but using Qt5 & Qt5 Wayland (Ubuntu 20.04: `qt5-default` & `qtwayland5`)

### This is what I'm going for:

![Icons](meta/icons.png)

Commits are a bit of a mess, contributions are accepted.

## Screenshot Coccinellidae 2022-07-16
_A lot of work remains..._
![Coccinellidae](meta/Screenshot_coccinellidae_2022-07-16.png)

## Screenshot Ladybird 2022-07-16
![Ladybird](meta/Screenshot_ladybird_2022-07-16.png)