gcc -shared -o ../../../build/shared/jportaudio.dll \
  -I"C:\Program Files\Java\jdk-15.0.2\include" \
	-I"C:\Program Files\Java\jdk-15.0.2\include\win32" \
	-I"..\..\..\..\portaudio\include" \
	JniUtils.c PortAudio.c \
	../../../build/shared/portaudio.dll
