# D3D9HookExample
Plug and play example of a D3D9 Hook using [ImGui](https://github.com/ocornut/imgui) for the menu

## Use

Change "Call of Duty 4" to the name of the game window you're trying to hook
```
HWND hWindow = FindWindowA(0, "Call of Duty 4");
```
Compile and inject into the game of your choice with a DLL injector.
You can use the INSERT key to show and hide the menu, and END to eject DLL.

## Example
```
HWND hWindow = FindWindowA(0, "Call of Duty®: BlackOps");
```
![Example](./screenshots/blackops-example.PNG)

### Contributors
Declan Crane
