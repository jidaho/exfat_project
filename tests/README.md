The system tests section is an automated set of scripts that test the funtional requirements of this project. 
To access it, type in make system_tests or make grading_tests.

Any individual test can be accessed by bash tests/x, so long as an image file has already been created.
bash tests/create_image.bash will make a test image and mount it.
bash tests/clean_up will get rid of image files and unmount them.

