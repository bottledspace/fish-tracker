https://user-images.githubusercontent.com/113856426/191630163-6e12614a-11bd-4e27-879c-804dfd9493b4.mov

# Overview

The following is a program for tracking species of marine fish from underwater video footage, for the [Fish for Knowledge open challenge](https://homepages.inf.ed.ac.uk/rbf/Fish4Knowledge/), and served as an opportunity to explore the use of support vector machines as a pattern recognition technique for computer vision. The results proved to be quite promising: given a very small amount of data, and even smaller images, a model could be constructed which detected fish from under water video footage with reasonable accuracy and excellent performance.

The feature set was created from the [Histogram of Oriented Gradients](https://en.wikipedia.org/wiki/Histogram_of_oriented_gradients) of the [ground truth images provided](https://homepages.inf.ed.ac.uk/rbf/Fish4Knowledge/GROUNDTRUTH/RECOG/). This involves partitioning each image into an array of cells. For each cell a corresponding histogram is computed, by counting how many pixels in the cell that the gradient falls into discrete angle intervals.

    ./mkhogs 64x64 s +1 detect/pos > detect.hogs

In the example above the features are labelled with a `+1` to indicate they belong to the positive class. In addition to the ground truth images, a second class of negative images was appended. The positive and negative classes each used 50 images. The positive images were all resized to 64x64 using the `s` option in the example above, and for the negative images random regions of size 64x64 were selected from each non-fish image using the `r` option.

    ./mkhogs 64x64 r -1 detect/neg >> detect.hogs

    
Once our training set has been created our model is then trained using libSVM. Two models were trained: one for detecting fish, and the other for classifying between species of fish. The classification model uses 23 classes, each with 10 examples of fish.

    svm-train detect.hogs detect.model

Finally, each frame of video is converted to a histogram of oriented gradients as with our featureset. Then a sliding window approach is used to detect fish using the detection model. If a fish is detected it can then be classified using the corresponding subregion of the image. The results are seen in the video above, using footage provided by the [Fish for Knowledge undersea data archive](https://homepages.inf.ed.ac.uk/rbf/Fish4Knowledge/resources.htm).

    ./fishcls 64x64 detect.model classify.model video.mp4

The detection model works quite well with a very limited dataset. The classification of species did not perform nearly as well, and is a source for future work. In particular, I believe results would be improved by reusing information from past frames when classifying, in order to avoid alternating classifications between frames. Another potential direction would be to use a different classification technique entirely for determining species, perhaps combining the fast detection using SVM with a slower but more accurate technique for species classification.

# Citations

The ground truth images which this fish tracker has been trained on are covered under the following license:


```
Permission is granted for anyone to copy, use, modify, or distribute this data and accompanying documents
for any purpose, provided this copyright notice is retained and prominently displayed, along with a note
saying that the original data are available from our web page and refering to [1]. The data and documents
are distributed without any warranty, express or implied. As the data were acquired for research purposes
only, they have not been tested to the degree that would be advisable in any important application. All
use of these data is entirely at the user's own risk.
```


[1] B. J. Boom, P. X. Huang, J. He, R. B. Fisher, "Supporting Ground-Truth annotation of image datasets using clustering", 21st Int. Conf. on Pattern Recognition (ICPR), 2012.
