These unit tests use cmocka-1.01 to run. 

### Install cmocka ###
In the cmocka-1.01 folder included in celestial (celestial/tests/cmocka_1.01), 
- mkdir build
- cd build
- cmake ..
- make
- sudo make install

You might need to run this if you get an error about shared libaries not being foundS:
- sudo mv /usr/local/lib/libcmocka* /usr/lib

### Run Unit Tests  ###
In celestial/tests:
 - mkdir build
 - cd build
 - cmake ..
 - make
 
TODO: Make some program that can run all the unit tests in one call.
