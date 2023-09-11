# bfgamp
Guitar amp based on BASS/C++

| :boom: DANGER                       |
|:------------------------------------|
| Just started, that's not even alpha |

## Build

### Windows
**Initial setup**

Install LLVM and add directury, containing `clang-cl.exe` to `Path` if it was not done during install *(C:\Program Files\LLVM\bin)*
Set `CXX` environment variable to `clang-cl`.

**Build**
```
meson build
ninja -C build
```

> [!NOTE]
> Executable directory should have `bass.dll` and `bassasio.dll` files, which can be obtained from [official webiste](https://www.un4seen.com/).

### Linux
```
meson build
ninja -C build
```
