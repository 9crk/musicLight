gcc -c zfft.c
gcc -c libza.c
g++ main.cpp -o test ./zfft.o ./libza.o -lportaudio -lopencv_highgui -lopencv_imgproc -lopencv_core
g++ piano.cpp -o piano ./zfft.o ./libza.o -lportaudio -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs
g++ playPCM.cpp -o playPCM -lopencv_highgui -lopencv_imgproc -lopencv_core
g++ genPic.cpp -o genPic -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs
