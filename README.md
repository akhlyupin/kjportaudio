# KJPortAudio

Java bindings for PortAudio I/O library.

More info about PortAudio:
https://github.com/PortAudio/portaudio

## Usage

I use Windows platform with MinGW-W64 8.1.0 compiler.

1. Need to get portaudio.dll from PortAudio repository.
2. Next, please go to c source folder and run make script.
```
cd lib/src/main/c
./make.sh
```
3. Go to folder and get jportaudio.dll.
```
lib/build/shared
```
4. Java headers lies there:
```
lib/src/main/java/com/jportaudio
```