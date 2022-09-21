The file `mkhogs.cc` contains the source for a small utility which converts images to HOG vectors for training SVM. The utility reads in all the images in a folder, converts them to vectors using the OpenCV HOG class, using the specified size. Then it outputs them as consecutive vectors, with the specified label, in libsvm format.

There are also two modes for converting the images: `r` for randomized sub images (useful for negative examples) and `s` for resizing the image. As an example:

    ./mkhogs 64x64 r -1 detect/neg > detect.hogs
    ./mkhogs 64x54 s +1 detect/pos >> detect.hogs

This would label randomized sub images from the folder `detect/neg` with the value -1, and label scaled images from the folder `detect/pos` with the value 1.

To train the model I used the Fish Recognition Ground-Truth database\sidenote{See \url{groups.inf.ed.ac.uk/f4k/GROUNDTRUTH/RECOG/}}, though in retrospect a database with clearer reference pictures and less total fish species would have probably been a better choice here.

See [*] for a demonstration of the tracker in action, using footage provided by the Fish for Knowledge database. The file `fishcls.cc` contains a program which uses two SVM model files provided to detect and then classify fish in a video source. The first model is converted from libsvm's format to OpenCV and then processed by OpenCV's HOG class. We pass the second model to a libsvm C++ wrapper `llsvm` which attempts to classify the regions of the frame which are detected to be fish by the first pass.

The results of the detection were surprisingly good considering only 50 positive and 50 negative samples were used, but the classification of species left something to be desired. I found that the training was very sensitive to the quality of the images, fewer high contrast and clear images was better than lots of lower quality ones. I hand picked the best 10 of each species of fish, for a total of 230 images, but even so it often gets the species wrong.

To improve the results further I think that in addition to better reference photos, retaining and using the results of previous frames would be necessary, since the fish can often become more difficult to determine at certain angles.