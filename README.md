# bfgamp
Guitar amp based on BASS/C++

| :boom: DANGER                       |
|:------------------------------------|
| Just started, that's not even alpha |

## Build

### Windows

> [!NOTE]
> Executable directory should have `bass.dll` and `bassasio.dll` files, which can be obtained from [official webiste](https://www.un4seen.com/).

#### Clang
Setup:

- Install LLVM and add directory, containing `clang-cl.exe` to `Path` if it was not done during install *(C:\Program Files\LLVM\bin)*.
- Set `CXX` environment variable to `clang-cl`.

Build:
```
meson build
ninja -C build
```

#### MSVC
```
meson build
meson compile -C build
```

#### Visual Studio
To generate Visual Studio solution:
```
meson build -Dbackend=vs2019
```

### Linux
```
meson build
ninja -C build
```
