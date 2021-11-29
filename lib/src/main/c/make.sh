gcc -shared -o ../../../build/shared/jportaudio.dll \
  -I"C:\Program Files\Java\jdk-15.0.2\include" \
	-I"C:\Program Files\Java\jdk-15.0.2\include\win32" \
	-I"..\..\..\..\portaudio\include" \
	JniUtils.c JpaUtils.c PortAudio.c HostApi.c Stream.c PaWasapiStreamInfo.c Device.c \
	../../../build/shared/portaudio.dll
