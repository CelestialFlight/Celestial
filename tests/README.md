These unit tests use cmocka-1.01 to run. 

### Install cmocka ###
In the cmocka-1.01 folder included in mflight (mflight/tests/cmocka_1.01), 
- mkdir build
- cd build
- cmake ..
- make
- sudo make install

You might need to run this:
- sudo mv /usr/local/lib/libcmocka* /usr/lib

### Run Unit Tests  ###
In mflight/tests:
 - mkdir build
 - cd build
 - cmake ..
 - make
 
TODO: Make some program that can run all the unit tests in one call.