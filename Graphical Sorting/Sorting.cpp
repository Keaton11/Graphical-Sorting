/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Holds the implementations of the methods associated with the
 *  Sorting class
 *
 *  Note: Although this file is very long, all code is relevant to the Sorting
 *  class. The file contains the implementation of each sorting algorithm and
 *  is thoroughly commented so readers can understand the steps in each algorithm.
*/

// includes header file
#include "Sorting.h"

/// Constructor
// recieves no parameters
Sorting::Sorting() {
    // drawCorner and drawSize are set to default values
    drawCorner.x = 240;
    drawCorner.y = 595;
    drawSize.x = 800;
    drawSize.y = 485;
    // colouredBars vector is resized (to the number of different bar colours)
    colouredBars.resize(3);
    // colours are set to default colours
    resetColours();
    // random type is set to default random type (pseudo random)
    randomType = 0;
    // data array is set (100 is the default data size)
    callRandom(true, 100);
    // retrieves the frequency of the performance counter
    QueryPerformanceFrequency(&Frequency);
}

/// Method Implementations: Randomization
// recieves a boolean which corresponds to whether the data should be reset
// and an integer corresponding to the number of randomizations that must occur
// empties data vector if reset is true
// calls appropriate randomization method
void Sorting::callRandom(bool reset, int n) {
    if (reset) {
        // if reset is true, the rawData vector is cleared
        rawData.clear();
    }
    // based on the value of randomType, a randomixation method is called
    if (randomType == 0) {
        randPseudoRandom(n);
    } else if (randomType == 1) {
        randUniformDistribution(reset, n);
    } else if (randomType == 2) {
        randSequenceShuffle(reset, n);
    }
    // method is called to reset variables
    resetVariables();
    sortType = -1;
}
// recieves an integer which corresponds to the number of randomizations
// randomizes values using the rand function and adds them to the raw data array
void Sorting::randPseudoRandom(int n) {
    for (int i = 0; i < n; ++i) {
        rawData.push_back(rand() % 1000 + 1);
    }
}
// recieves a boolean for whether the data should be reset
// and an integer which corresponds to the number of randomizations
// randomizes values using uniform real distribution and adds them to the raw data array
void Sorting::randUniformDistribution(bool reset, int n) {
    if (!reset) {
        // if reset is false, the data size is added to the number for the raw data vector is cleared
        // the raw data must be reset before this randomization method can be used
        n += rawData.size();
    }
    rawData.clear();

    default_random_engine generator;
    uniform_real_distribution<double> distribution(1, 1000);

    // generator is seeded on current time
    generator.seed(time(0));

    for (int i = 0; i < n; ++i) {
        // randomized values are added to the rawData vector
        rawData.push_back(distribution(generator));
    }
}
// recieves a boolean for whether the data should be reset
// and an integer which corresponds to the number of randomizations
// randomizes values by shuffling a sequence of evenly spaced integers
void Sorting::randSequenceShuffle(bool reset, int n) {
    if (!reset) {
        // if reset is false, the data size is added to the number for the raw data vector is cleared
        // the raw data must be reset before this randomization method can be used
        n += rawData.size();
    }
    rawData.clear();

    // boolean vector is declared (used to keep track of which indexes have already been used)
    vector<bool> randomized(n + 1, false);
    float difference = 1000.0 / n;
    int randomIndex;

    // vector is initialized with evenly spaced numbers so the slope is smooth
    for (int i = 0; i < n; ++i) {
        do {
            // while loop ensures index has not already been used
            randomIndex = rand() % n + 1;
        } while (randomized[randomIndex]);

        // number based on difference and index is added to the rawData vector
        rawData.push_back(randomIndex * difference);
        randomized[randomIndex] = true;
    }
}

/// Method Implementations: Reset Values
// recieves no parameters
// resets variables (before running a new algorithm)
void Sorting::resetVariables() {
    sorted = false;     // sorted is set to false
    data = rawData;     // rawData is copied into data
    // efficieny metrics are reset (set to 0)
    comparisons = 0;
    moves = 0;
    systemTime = 0;
    drawDelay = 0;
    // target indexes (used in binary search) are reset
    targetIndex[0] = -1;
    targetIndex[1] = -1;
    resetBars();        // method is called to reset the coloured bars
}
// recieves no parameters
// resets colour gradient to default colours
void Sorting::resetColours() {
    // default colours are red, iris, and purple
    gradientColour[0] = RED;
    gradientColour[1] = IRIS;
    gradientColour[2] = PURPLE;
}
// recieves no parameters
// resets coloured bars
void Sorting::resetBars() {
    // all bars are set to "not-faded" by default
    faded.resize(rawData.size());
    fill(faded.begin(), faded.end(), false);
    for (int i = 0; i < colouredBars.size(); ++i) {
        // no indexes contained coloured bars
        colouredBars[i] = -1;
    }
}

/// Method Implementations: Add / Remove Bars
// recieves no parameters
// calls randomization method to add one bar
// resets data, and draws bars
void Sorting::addBar() {
    if (rawData.size() < 800) {
        // if rawData size is greater than 800, no new bars can be added
        callRandom(false, 1);
        data = rawData;
        drawBars();
    }
}
// recieves no parameters
// calls randomization method to remove one bar
// resets data, and draws bars
void Sorting::removeBar() {
    if (rawData.size() > 2) {
        // if rawData size is less than 801, no bars can be removed
        if (randomType == 2) {
            // if randomType == 2, method is called to reset data to ensure sequence remains correctly shuffled
            callRandom(true, rawData.size() - 1);
        } else {
            rawData.pop_back();
            data = rawData;
            drawBars();
        }
    }
}

/// Method Implementations: Drawing
// recieves no parameters
// draws a graphical representation of the data
void Sorting::drawBars() {
    // background (of the graphical representation box) and border are drawn
    al_draw_rectangle(drawCorner.x - 5, drawCorner.y - drawSize.y - 5, drawCorner.x + drawSize.x + 5, drawCorner.y + 5, DARK_PURPLE, 10);
    al_draw_filled_rectangle(drawCorner.x, drawCorner.y - drawSize.y, drawCorner.x + drawSize.x, drawCorner.y, BLACK);

    // variables are declared
    float barWidth = float(drawSize.x) / data.size();   // bar width is based on the number of bars
    float percentage, barHeight;
    ALLEGRO_COLOR colour;

    for (int i = 0; i < data.size(); ++i) {
        // percentage (of full height) is calculated and used to draw bars
        percentage = (float(data[i]) / 1000);
        barHeight = percentage * drawSize.y;
        // if index is found in the colouredBars or faded vector, bar is drawn in the specified colour
        // this is used to make important bars (ex. pivot bars) stand out, or fade unimportant bars (in binary search)
        if (i == colouredBars[0]) {
            colour = WHITE;
        } else if (i == colouredBars[1] || i == colouredBars[2]) {
            colour = BLACK;
        } else if (faded[i]) {
            colour = LIGHT_IRIS;
        } else {
            // by default, colourGradient method is called to determine the bars colour based on its height
            colour = colourGradient(percentage);
        }
        // bar is drawn in its calculated height and colour
        al_draw_filled_rectangle(drawCorner.x + i * barWidth, drawCorner.y, drawCorner.x + (i + 1) * barWidth, drawCorner.y - barHeight, colour);
        if (data.size() <= 200) {
            // if there are less than 200 bars, the bars are outlined in white
            al_draw_rectangle(drawCorner.x + i * barWidth, drawCorner.y, drawCorner.x + (i + 1) * barWidth, drawCorner.y - barHeight, LAVENDER, 1);
        }
    }
    // border lines are added
    al_draw_rectangle(drawCorner.x - 10, drawCorner.y - drawSize.y - 10, drawCorner.x + drawSize.x + 10, drawCorner.y + 10, WHITE, 1);
    al_draw_rectangle(drawCorner.x, drawCorner.y - drawSize.y, drawCorner.x + drawSize.x, drawCorner.y, WHITE, 1);

    // method is called to draw efficiency metrics
    drawMetrics();
    al_flip_display();
}
// recieves a percentage as parameters
// returns a colour based on such percentage
ALLEGRO_COLOR Sorting::colourGradient(float percentage) {
    // three unsigned character arrays are declared
    unsigned char colour1RBG[3], colour2RBG[3], gradientColourRBG[3];

    if (percentage <= 0.50) {
        // if percentage is less than or eqaul to 0.5, the rbg values of the first two colours are taken
        al_unmap_rgb(gradientColour[0], &colour1RBG[0], &colour1RBG[1], &colour1RBG[2]);
        al_unmap_rgb(gradientColour[1], &colour2RBG[0], &colour2RBG[1], &colour2RBG[2]);
        percentage *= 2;
    } else {
        // if percentage is greater than 0.5, the rbg values of the second two colours are taken
        al_unmap_rgb(gradientColour[1], &colour1RBG[0], &colour1RBG[1], &colour1RBG[2]);
        al_unmap_rgb(gradientColour[2], &colour2RBG[0], &colour2RBG[1], &colour2RBG[2]);
        percentage = (percentage - 0.50) * 2;
    }
    // the new colour is made based on the rbg values taken of the two other colours
    // rbg values are calculated to form a smooth three colour gradient
    gradientColourRBG[0] = (colour2RBG[0] - colour1RBG[0]) * percentage + colour1RBG[0];
    gradientColourRBG[1] = (colour2RBG[1] - colour1RBG[1]) * percentage + colour1RBG[1];
    gradientColourRBG[2] = (colour2RBG[2] - colour1RBG[2]) * percentage + colour1RBG[2];
    // new colour is returned
    return al_map_rgb(gradientColourRBG[0], gradientColourRBG[1], gradientColourRBG[2]);
}
// recieves no parameters
// draws efficieny metrics
// note: decimals are used to draw rectangles in calculated positions; however, values are rounded before being drawn
void Sorting::drawMetrics() {
    // black rectangle is drawn to hide previous rectangles
    al_draw_filled_rectangle(220, 605, 1060, 860, BLACK);
    // if the sortType is 3 or 5, four metrics are measured, so the rectangles are drawn accordingly
    if ((sortType == 3 && colouredBars[0] != -1) || sortType == 5) {
        for (int i = 0; i < 4; ++i) {
            al_draw_filled_rectangle(230 + 211.25 * i, 630, 416.25 + 211.25 * i, 695, IRIS);
            al_draw_rectangle(230 + 211.25 * i, 630, 416.25 + 211.25 * i, 695, WHITE, 1);
        }
        // comparisons, moves, and system time are drawn to the allegro display based on their values
        al_draw_text(basic_font, WHITE, 323.125, 630, ALLEGRO_ALIGN_CENTER, "Comparisons:");
        al_draw_text(basic_font, WHITE, 323.125, 660, ALLEGRO_ALIGN_CENTER, to_string(comparisons).c_str());
        al_draw_text(basic_font, WHITE, 534.375, 630, ALLEGRO_ALIGN_CENTER, "Moves:");
        al_draw_text(basic_font, WHITE, 534.375, 660, ALLEGRO_ALIGN_CENTER, to_string(moves).c_str());
        al_draw_text(basic_font, WHITE, 745.625, 630, ALLEGRO_ALIGN_CENTER, "System Time:");
        if (systemTime == -1) {
            // if systemTime is -1, the loading animation is drawn
            updateLoading();
            al_draw_text(basic_font, WHITE, 729.625, 660, ALLEGRO_ALIGN_LEFT, loading.c_str());
        } else {
            // "us" is used to represent microseconds
            al_draw_text(basic_font, WHITE, 745.625, 660, ALLEGRO_ALIGN_CENTER, (to_string(systemTime) + " us").c_str());
        }
        if (sortType == 3) {
            // if the sortType is 3, the pivot value is added to the fourth rectangle
            al_draw_text(basic_font, WHITE, 956.875, 630, ALLEGRO_ALIGN_CENTER, "Pivot:");
            al_draw_text(basic_font, WHITE, 956.875, 660, ALLEGRO_ALIGN_CENTER, to_string(data[colouredBars[0]]).c_str());
        } else if (targetIndex[0] == -1) {
            // if the sortType is 5 and the target index has not yet been updated, the target is added to the fourth rectangle
            al_draw_text(basic_font, WHITE, 956.875, 630, ALLEGRO_ALIGN_CENTER, "Target:");
            al_draw_text(basic_font, WHITE, 956.875, 660, ALLEGRO_ALIGN_CENTER, to_string(target).c_str());
        } else if (targetIndex[1] == -1) {
            // if the sortType is 5 and the target index has been updated, the target index is added to the fourth rectangle
            al_draw_text(basic_font, WHITE, 956.875, 630, ALLEGRO_ALIGN_CENTER, "Target Index:");
            al_draw_text(basic_font, WHITE, 956.875, 660, ALLEGRO_ALIGN_CENTER, to_string(targetIndex[0]).c_str());
        } else {
            // if the sortType is 5 and the target index has two values, the closest range to the target is added to the fourth rectangle
            al_draw_text(basic_font, WHITE, 956.875, 630, ALLEGRO_ALIGN_CENTER, "Closest Range:");
            al_draw_text(basic_font, WHITE, 956.875, 660, ALLEGRO_ALIGN_CENTER, (to_string(targetIndex[0]) + " - " + to_string(targetIndex[1])).c_str());
        }
    } else {
        // in most sorts, three metrics are measured, so the rectangles are drawn accordingly
        for (int i = 0; i < 3; ++i) {
            al_draw_filled_rectangle(230 + 281.666 * i, 630, 486.666 + 281.666 * i, 695, IRIS);
            al_draw_rectangle(230 + 281.666 * i, 630, 486.666 + 281.666 * i, 695, WHITE, 1);
        }
        // comparisons, moves, and system time are drawn to the allegro display based on their values
        al_draw_text(basic_font, WHITE, 358.333, 630, ALLEGRO_ALIGN_CENTER, "Comparisons:");
        al_draw_text(basic_font, WHITE, 358.333, 660, ALLEGRO_ALIGN_CENTER, to_string(comparisons).c_str());
        al_draw_text(basic_font, WHITE, 639.999, 630, ALLEGRO_ALIGN_CENTER, "Moves:");
        al_draw_text(basic_font, WHITE, 639.999, 660, ALLEGRO_ALIGN_CENTER, to_string(moves).c_str());
        al_draw_text(basic_font, WHITE, 921.665, 630, ALLEGRO_ALIGN_CENTER, "System Time:");
        if (systemTime == -1) {
            // if systemTime is -1, the loading animation is drawn
            updateLoading();
            al_draw_text(basic_font, WHITE, 905.665, 660, ALLEGRO_ALIGN_LEFT, loading.c_str());
        } else {
            // "us" is used to represent microseconds
            al_draw_text(basic_font, WHITE, 921.665, 660, ALLEGRO_ALIGN_CENTER, (to_string(systemTime) + " us").c_str());
        }
    }
}
// recieves no parameters
// updates the string loading to give the effect of a loading animation
void Sorting::updateLoading() {
    // loading string is updated based on the timer (to ensure loading animation is steady)
    if ((al_get_timer_count(timer) / 10) % 4 == 0) {
        loading = "";
    } else if ((al_get_timer_count(timer) / 10) % 4 == 1) {
        loading = ".";
    } else if ((al_get_timer_count(timer) / 10) % 4 == 2) {
        loading = ". .";
    } else if ((al_get_timer_count(timer) / 10) % 4 == 3) {
        loading = ". . .";
    }
}

/// Method Implementations: Sorting
// recieves two indexes as parameters
// swaps the data values at given indexes
void Sorting::swapBars(int index1, int index2) {
    // temporary variable stores value at index1 before swapping
    int temp = data[index1];
    data[index1] = data[index2];
    data[index2] = temp;
}
// recieves sort type as an integer and speed as a float
// resets data, then calls appropriate sorting algorithm
// times the duration of the sort in microseconds (factoring in draw time)
void Sorting::callSort(int sT, float s) {
    // variables are updated based on parameters
    sortType = sT;
    speed = s;

    // method is called to reset variables
    resetVariables();

    // timer is started
    systemTime = -1;
    systemTimer tSort;
    QueryPerformanceCounter(&tSort.start);

    // based on the sort type called, the appropriate algorithm is called
    if (sortType == 0) {
        bubbleSort();
    } else if (sortType == 1) {
        selectionSort();
    } else if (sortType == 2) {
        insertionSort();
    } else if (sortType == 3) {
        quickSort(&data, 0);
    } else if (sortType == 4) {
        mergeSort(0, data.size() - 1);
    } else if (sortType == 5) {
        if (!sorted) {
            // if data is not sorted, std::sort is used to quickly sort the data
            // before beginning the binary search
            std::sort(data.begin(), data.end());
            QueryPerformanceCounter(&tSort.start);
        }
        binarySearch(0, data.size() - 1);
    } else if (sortType == 6) {
        cocktailSort();
    } else if (sortType == 7) {
        combSort();
    } else if (sortType == 8) {
        gnomeSort();
    } else if (sortType == 9) {
        oddEvenSort();
    } else if (sortType == 10) {
        heapSort();
    } else if (sortType == 11) {
        cycleSort();
    } else if (sortType == 12) {
        radixSort();
    } else if (sortType == 13) {
        shellSort();
    } else if (sortType == 14) {
        bitonicSort();
    } else if (sortType == 15) {
        stoogeSort(0, data.size() - 1);
    } else if (sortType == 16) {
        bogoSort();
    } else if (sortType == 17) {
        std::sort(data.begin(), data.end());
    } else if (sortType == 18) {
        kSort1();
    } else if (sortType == 19) {
        kSort2();
    } else if (sortType == 20) {
        kSort3();
    } else if (sortType == 21) {
        kSort4(0, data.size() - 1);
    } else if (sortType == 22) {
        kSort5();
    } else if (sortType == 23) {
        kSort6();
    }

    // timer is stopped
    QueryPerformanceCounter(&tSort.stop);
    // system time is calculated, factoring in the draw delay
    systemTime = (tSort.stop.QuadPart - tSort.start.QuadPart) * 1000000 / Frequency.QuadPart - drawDelay;

    if (sortType != 5)
        // bars are reset; except for binary search
        resetBars();
    // bars are drawn
    drawBars();
    sorted = true;  // sorted is set to true
}

/// Method Implementations: Sorting Algoritms
// recieves no parameters
// sorts data using the Bubble sort algorithm
void Sorting::bubbleSort() {
    bool swapped = true;    // swapped is set to true;
    // loop ends once an iteration has passed where no swaps have bnen made
    while (swapped) {
        swapped = false;    // swapped is set to false
        for (int i = 0; i < data.size() - 1; ++i) {
            ++comparisons;
            // the data of all indexes (from 0 to data size - 1) is compared to the data of the index to its right
            // if the data from the first index is larger, the two bars are swapped
            // if no bars are swapped throughout the for loop, all the data is sorted
            if (data[i] > data[i + 1]) {
                swapBars(i, i + 1);
                ++moves;
                swapped = true;
                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = i + 1;
                drawBars();
                al_rest(speed / 100);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
    }
}
// recieves no parameters
// sorts data using the Selection sort algorithm
void Sorting::selectionSort() {
    for (int i = 0; i < data.size(); ++i) {
        // for loop iterates through every index of the data vector
        int smallestIndex = i;
        for (int j = i; j < data.size(); ++j) {
            ++comparisons;
            // the smallest value after the given index is located
            if (data[j] < data[smallestIndex]) {
                smallestIndex = j;
            }
        }
        // once the index containing the smallest value has been found, it is swapped with i
        swapBars(i, smallestIndex);
        ++moves;

        // after each swap, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        colouredBars[0] = i;
        colouredBars[1] = smallestIndex;
        drawBars();
        al_rest(speed / 10);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
    }
}
// recieves no parameters
// sorts data using the Insertion sort algorithm
void Sorting::insertionSort() {
    for (int i = 1; i < data.size(); ++i) {
        // for loop iterates through every index of the data vector starting at index 1
        ++comparisons;
        // the data at each index is compared to the data at the previous index
        while (data[i] < data[i - 1] && i > 0) {
            // if smaller, the data is swapped
            swapBars(i, i - 1);
            ++moves;

            // after each swap, the graphical display is updated
            // QPC is used to account for the time spent drawing
            QueryPerformanceCounter(&tDraw.start);

            colouredBars[0] = i - 1;
            drawBars();
            al_rest(speed / 50);

            QueryPerformanceCounter(&tDraw.stop);
            drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

            // if swapped, i is reduced by one, and is again compared with the index before it
            --i;
            ++comparisons;
        }
    }
}
// recieves a data vector, and an offset value as parameters
// sorts data using the Quick sort algorithm
void Sorting::quickSort(vector<int> *dataVector, int offset) {
    if (dataVector->size() < 2)
        // breakpoint in recursive function (if vector size is less than 2)
        return;

    // pivot is set to the last value in the vector
    int pivot = dataVector->at(dataVector->size() - 1);
    vector<int> dataSmaller, dataLarger;

    for (int i = 0; i < dataVector->size() - 1; ++i) {
        // for loop iterates through all values in the vector
        ++comparisons;
        ++moves;
        if (dataVector->at(i) > pivot) {
            // values larger than pivot are placed in one vector (dataLarger)
            dataLarger.push_back(dataVector->at(i));
        } else {
            // values smaller or equal to pivot are placed in another vector (dataSmaller)
            dataSmaller.push_back(dataVector->at(i));
        }
    }

    // after the vector is split, the graphical display is updated
    // QPC is used to account for the time spent drawing
    QueryPerformanceCounter(&tDraw.start);

    // in order to update the display, the original data vector must be updated
    // this is not necessary for the sorting algorithm, but is just performed to display the sort,
    // so these steps have been included in the draw delay
    for (int i = 0; i < dataSmaller.size(); ++i) {
        data[i + offset] = dataSmaller[i];
    }
    colouredBars[0] = offset + dataSmaller.size();
    data[offset + dataSmaller.size()] = pivot;
    for (int i = 0; i < dataLarger.size(); ++i) {
        data[i + offset + dataSmaller.size() + 1] = dataLarger[i];
    }
    drawBars();
    al_rest(speed / 3);

    QueryPerformanceCounter(&tDraw.stop);
    drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

    // the method is recursively called on both vectors
    quickSort(&dataSmaller, offset);
    quickSort(&dataLarger, offset + dataSmaller.size() + 1);

    // the smaller vector, the pivot, and the larger vector are appended
    *dataVector = dataSmaller;
    dataVector->push_back(pivot);
    dataVector->insert(dataVector->end(), dataLarger.begin(), dataLarger.end());
}
// recieves a left index and a right index as parameters
// sorts data using the Merge sort algorithm
void Sorting::mergeSort(int leftIndex, int rightIndex) {

    ++comparisons;
    if (leftIndex >= rightIndex) {
        // breakpoint in recursive function (if left index is greater or equal the the right index)
        return;
    }

    // mid index (midpoint between the two indexes) is calculated
    int midIndex = leftIndex + (rightIndex - leftIndex) / 2;
    // the method is recursively called on the both halves of the interval
    mergeSort(leftIndex, midIndex);
    mergeSort(midIndex + 1, rightIndex);

    // mid index serves as a splitting point into two vectors
    // (after merge sort has been recursively called on each half, they have been sorted)
    vector<int> leftVector, rightVector;
    for (int i = 0; i < midIndex - leftIndex + 1; ++i) {
        ++moves;
        leftVector.push_back(data[i + leftIndex]);
    }
    for (int i = 0; i < rightIndex - midIndex; ++i) {
        ++moves;
        rightVector.push_back(data[i + midIndex + 1]);
    }

    // variables are declared to merge the vectors (c stands for current)
    int cLeftIndex = 0;
    int cRightIndex = 0;
    int cMergedIndex = leftIndex;

    // until the end of either the left vector or the right vector, the smaller element is placed first
    while (cLeftIndex < leftVector.size() && cRightIndex < rightVector.size()) {
        ++comparisons;
        ++moves;
        if (leftVector[cLeftIndex] <= rightVector[cRightIndex]) {
            data[cMergedIndex] = leftVector[cLeftIndex];
            ++cLeftIndex;
        } else {
            data[cMergedIndex] = rightVector[cRightIndex];
            ++cRightIndex;
        }

        // after an element has been added to the data vector, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        colouredBars[1] = leftIndex;
        colouredBars[0] = cMergedIndex;
        colouredBars[2] = rightIndex;
        drawBars();
        al_rest(speed / 10);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
        ++cMergedIndex;
    }

    // the remaining elements from either the left or right vector are added to the sorted vector
    while (cLeftIndex < leftVector.size()) {
        ++moves;
        data[cMergedIndex] = leftVector[cLeftIndex];
        ++cLeftIndex;
        ++cMergedIndex;
    }
    while (cRightIndex < rightVector.size()) {
        ++moves;
        data[cMergedIndex] = rightVector[cRightIndex];
        ++cRightIndex;
        ++cMergedIndex;
    }
}
// recieves a left index and a right index as parameters
// searches for a target value via Binary search
void Sorting::binarySearch(int leftIndex, int rightIndex) {
    // each time this method is called, the graphical display is updated
    // QPC is used to account for the time spent drawing
    QueryPerformanceCounter(&tDraw.start);

    // the left and right indexes are drawn in another colour
    // the bars outside this region are "faded"
    // this is purely to graphically display the algorithm, rather than part of the search,
    // so these steps have been counted as part of the draw delay
    if (leftIndex != rightIndex) {
        colouredBars[1] = leftIndex;
        for (int i = 0; i < leftIndex; ++i) {
            faded[i] = true;
        }
        colouredBars[2] = rightIndex;
        for (int i = data.size(); i > rightIndex; --i) {
            faded[i] = true;
        }
        drawBars();
        al_rest(speed);
    }

    QueryPerformanceCounter(&tDraw.stop);
    drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

    ++comparisons;
    if (rightIndex >= leftIndex) {
        // if the right index is greater or equal to the left index, the mid index is calculated
        ++moves;
        int midIndex = leftIndex + (rightIndex - leftIndex) / 2;
        ++comparisons;
        if (data[midIndex] == target) {
            // if the mid index is the target value, the method is returned
            // the target's index is stored
            colouredBars[0] = midIndex;
            targetIndex[0] = midIndex;
            return;
        } else if (data[midIndex] > target) {
            // if the target is less than the mid index, the method is recursively called between the left and mid index
            binarySearch(leftIndex, midIndex - 1);
            return;
        } else {
            // if the target is greater than the mid index, the method is recursively called between the mid and right index
            binarySearch(midIndex + 1, rightIndex);
            return;
        }
    } else {
        // if the right index is less than the left index, the target value is not present
        // the closest range (between data values) is stored
        if (data[rightIndex] > 1000)
            data[rightIndex] = 0;
        if (data[leftIndex] > 1000)
            data[leftIndex] = 1000;
        targetIndex[0] = data[rightIndex];
        targetIndex[1] = data[leftIndex];
    }
}
// recieves no parameters
// sorts data using the Cocktail sort algorithm
// Cocktail sort is a variation of Bubble sort which traverses through an array while
// alternating between both directions (left to right and right to left)
void Sorting::cocktailSort() {
    bool swapped;
    do {
        swapped = false;    // swapped is set to false
        for (int i = 0; i < data.size() - 1; ++i) {
            // the data of all indexes (from 0 to data size - 1) is compared to the data of the index to its right
            // if the data from the first index is larger, the two bars are swapped
            // if no bars are swapped throughout the for loop, all the data is sorted
            +comparisons;
            if (data[i] > data[i + 1]) {
                swapBars(i, i + 1);
                swapped = true; // swapped is set to true if a swap occurs
                ++moves;

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = i + 1;
                drawBars();
                al_rest(speed / 50);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
        if (!swapped) {
            // if swapped is false, the data has been sorted, so the method is returned
            break;
        }
        swapped = false;    // swapped is set to false
        for (int i = data.size() - 1; i >= 0; --i) {
            // the data of all indexes (from data size - 1 to 0) is compared to the data of the index to its right
            // if the data from the first index is larger, the two bars are swapped
            // if no bars are swapped throughout the for loop, all the data is sorted
            ++comparisons;
            if (data[i] > data[i + 1]) {
                swapBars(i, i + 1);
                swapped = true; // swapped is set to true if a swap occurs
                ++moves;

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = i;
                drawBars();
                al_rest(speed / 50);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
    } while (swapped);  // if swapped is false, the data has been sorted, so the while loop ends
}
// recieves no parameters
// sorts data using the Comb sort algorithm
// Comb sort is similar to Bubble sort, but is more efficient as
// it uses a gap of size greater than 1
void Sorting::combSort() {

    int gap = data.size();  // gap size is initialized to data size
    float shrink = 1.3;     // shrink factor is set to 1.3 (generally considered the ideal shrink factor)
    bool swapped = true;    // swapped is set to true

    while (swapped) {
        gap /= shrink;      // gap size is updated using shrink factor
        if (gap < 1) {
            gap = 1;        // if the gap size is less than one, the gap size is set to one and sorted is set to true
            swapped = false;  // if no swaps occur in the following while loop, the data has been sorted
        }
        int position = 0;   // position begins at 0; while loop iterates through all positions in which the gap can be applied
        while (position + gap < data.size()) {
            ++comparisons;
            // if data at index position is greater than data at index position plus gap, the values are swapped
            if (data[position] > data[position + gap]) {
                swapBars(position, position + gap);
                swapped = true;     // swapped is set to true if a swap occurs
                ++moves;

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = position;
                drawBars();
                al_rest(speed / 50);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
            ++position;     // position is increased by 1
        }
    }
}
// recieves no parameters
// sorts data using the Gnome sort algorithm
// Gnome sort is similar to Insertion sort in that it works with one item at a time
// however, the item is placed through a series of swaps like Bubble sort
void Sorting::gnomeSort() {
    int position = 0;   // position is set to 0

    while (position < data.size()) {
        // while loop continues until position reaches the last index in the data vector
        ++comparisons;
        if (position == 0 || data[position] >= data[position - 1]) {
            // if the data at a position is greater than that at the index before, position increases by 1
            ++position;
        } else {
            // if the data at a position is smaller than that at the index before, the values are swapped and position decreases by 1
            swapBars(position, position - 1);
            --position;
            ++moves;

            // after each swap, the graphical display is updated
            // QPC is used to account for the time spent drawing
            QueryPerformanceCounter(&tDraw.start);

            colouredBars[0] = position;
            drawBars();
            al_rest(speed / 50);

            QueryPerformanceCounter(&tDraw.stop);
            drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
        }
    }
}
// recieves no parameters
// sorts data using the Odd-Even sort algorithm
// Odd-Even sort is similar to Bubble sort, but is divided into
// two different phases (odd and even)
void Sorting::oddEvenSort() {
    bool swapped = true;    // swapped is set to true

    while (swapped) {       // loop continues until an iteration has occured where no swaps have been made
        swapped = false;    // swapped is set to fale
        for (int i = 1; i >= 0; --i) {
        // for loop swiches between an odd and even phase
            for (int j = i; j < data.size() - 1; j += 2) {
                // the data of all odd/even indexes is compared to the data of the index to its right
                // if the data from the first index is larger, the two bars are swapped
                // if no bars are swapped throughout the for loop, all the data is sorted
                ++comparisons;
                if (data[j] > data[j + 1]) {
                    swapBars(j, j + 1);
                    swapped = true;
                    ++moves;

                    // after each swap, the graphical display is updated
                    // QPC is used to account for the time spent drawing
                    QueryPerformanceCounter(&tDraw.start);

                    colouredBars[0] = j;
                    drawBars();
                    al_rest(speed / 50);

                    QueryPerformanceCounter(&tDraw.stop);
                    drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
                }
            }
        }
    }
}
// recieves no parameters
// sorts data using the Heap sort algorithm
// Heap sort is an algorithm which makes use of a binary tree
// the vector is sorted by continuously removing the largest element of a heap
// and placing it in its correct position in the data vector
void Sorting::heapSort() {

    for (int i = data.size() / 2; i >= 0; --i) {
        // a max heap is formed by calling the heapify method for the first half of the vector
        heapSortHeapify(data.size(), i);
    }
    for (int i = data.size() - 1; i >= 0; --i) {
        // the largest element (at position 0 after the heapify method has been called), is placed in its correct position
        swapBars(i, 0);
        ++moves;

        // after each swap, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        colouredBars[0] = i;
        drawBars();
        al_rest(speed / 50);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

        // the heapify method is called
        heapSortHeapify(i, 0);
    }

}
// recieves a heap size and a node as parameters
// used to "heapify" a given subtree (place largest element in position 0)
void Sorting::heapSortHeapify(int heapSize, int node) {
    int largest = node;         // by default, the largest value is set to node
    // the index of the left and right child (in the heap) are determined
    int left = 2 * node + 1;
    int right = 2 * node + 2;

    ++comparisons;
    if (left < heapSize && data[left] > data[largest]) {
        // if the left child is larger than the current largest value, largest is set to left
        largest = left;
    }

    ++comparisons;
    if (right < heapSize && data[right] > data[largest]) {
        // if the right child is larger than the current largest value, largest is ser to right
        largest = right;
    }

    if (largest != node) {
        // if the largest value is not at the node, the values are swapped
        swapBars(node, largest);
        ++moves;

        // after each swap, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        colouredBars[0] = largest;
        drawBars();
        al_rest(speed / 50);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

        // heapify is called again on the new largest value
        heapSortHeapify(heapSize, largest);
    }
}
// recieves no parameters
// sorts data using the Cycle sort algorithm
// Cycle sort is an algorithm which minimizes the number of memory writes
// through a process called cycle decomposition
void Sorting::cycleSort() {
    for (int cycleStart = 0; cycleStart < data.size(); ++cycleStart) {
    // for loop iterates through every index in the data vector
        // item is initialized as the element at the starting position of a cycle
        int item = data[cycleStart];
        int position = cycleStart;
        for (int i = cycleStart + 1; i < data.size(); ++i) {
            ++comparisons;
            if (data[i] < item) {
                // the number of elements smaller than item are counted to determine item's position
                ++position;
            }
        }
        if (position == cycleStart) {
            // if item is already in the correct position, the loop continues from the next index
            continue;
        }
        while (item == data[position]) {
            // position is increased for every value equal to item to ignore duplicates
            ++position;
        }

        // item is placed in its correct position
        int temp = data[position];
        data[position] = item;
        item = temp;    // item now contains the value that was at the previous swapped index
        ++moves;

        // after each swap, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        colouredBars[0] = position;
        drawBars();
        al_rest(speed / 50);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

        while (position != cycleStart) {
            // while loops rotates through the rst of the cycle to find the correct position for the new value item
            position = cycleStart;

            for (int i = cycleStart + 1; i < data.size(); ++i) {
                ++comparisons;
                if (data[i] < item) {
                    // the number of elements smaller than item are counted to determine item's position
                    ++position;
                }
            }
            while (item == data[position]) {
                // position is increased for every value equal to item to ignore duplicates
                ++position;
            }
            if (item != data[position]) {
                // if item is not in the correct position, item is placed in its position
                int temp = data[position];
                data[position] = item;
                item = temp;
                ++moves;    // item now contains the value that was at the previous swapped index

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = position;
                drawBars();
                al_rest(speed / 50);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
    }
}
// recieves no parameters
// sorts data using the Radix sort algorithm
// Radix sort is an algortihm which sorts data digit by digit
// this particular version (LSD) begins with the least significant digit
void Sorting::radixSort() {
    // variables are declared
    int index, position;
    list<int> pocket[10]; // pocket array is used to hold lists (one for each digit)
    for (int i = 10; 1000 / i > 0; i *= 10) {
        // loop runs through each multiple of 10 from 10 to 1000 (since data has a max of four digits)
        for (int j = 0; j < data.size(); ++j) {
            ++comparisons;
            ++moves;
            // each index is placed in a pocket corresponding to the value of the current digit
            index = (data[j] % i) / (i / 10);
            pocket[index].push_back(data[j]);
        }
        position = 0;
        for (int j = 0; j < 10; j++) {
            while (!pocket[j].empty()) {
                // pockets are added to the array starting from pocket 0 to pocket 9
                // this places the data in order based on the current digit
                data[position] = *(pocket[j].begin());
                pocket[j].erase(pocket[j].begin());
                ++position;

                // after each pocket is added, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = position;
                drawBars();
                al_rest(speed / 50);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
    }
}
// recieves no parameters
// sorts data using the Shell sort algorithm
// Shell sort is similar to Insertion sort, but is more efficient as
// it uses a gap of size greater than 1
void Sorting::shellSort() {
    // variables are declared
    int temp, position;
    // begins with a gap half the size of the vector size
    // reduces the gap by half in each iteration
    for (int gap = data.size() / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < data.size(); i++) {
            // temp is set the value at index i
            temp = data[i];
            // a gapped insertion sort is applied for this gap size
            for (position = i; position >= gap && data[position - gap] > temp; position -= gap) {
                // earlier gap sorted elements are shifted until the correct position for the value of temp is found
                ++comparisons;
                data[position] = data[position - gap];
            }
            // temp is placed in its correct position
            data[position] = temp;
            ++moves;

            // after the value has been placed in the correct position, the graphical display is updated
            // QPC is used to account for the time spent drawing
            QueryPerformanceCounter(&tDraw.start);

            colouredBars[0] = i;
            drawBars();
            al_rest(speed / 50);

            QueryPerformanceCounter(&tDraw.stop);
            drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
        }
    }
}
// recieves no parameters
// sorts data using the Bitonic sort algorithm
// Bitonic sort is a sorting algorithm which can be used to sort
// vectors containing 2^n elements
void Sorting::bitonicSort() {
    for (int i = 2; i <= data.size(); i *= 2) {
        // i begins at 2, and is doubled at every iteration
        for (int j = i / 2; j > 0; j /= 2) {
            // j begins at i / 2, and is halved at every iteration
            for (int k = 0; k < data.size(); ++k) {
                // l is set to j XOR k
                // ^ is the bitwise XOR operator
                int l = j ^ k;
                if (l > k && l < data.size()) {
                    comparisons += 2;
                    if ((((k & i) == 0) && (data[k] > data[l])) || (((k & i) != 0) && (data[k] < data[l]))) {
                    // if k AND i equals 0, and the value at index k is greater than the value at index l
                    // or k AND i does not equal 0, and the value at index k is less than the value at index l
                    // k and l are swapped
                    // & is the bitwise AND operator
                        swapBars(k, l);
                        ++moves;

                        // after each swap, the graphical display is updated
                        // QPC is used to account for the time spent drawing
                        QueryPerformanceCounter(&tDraw.start);

                        colouredBars[0] = k;
                        drawBars();
                        al_rest(speed / 50);

                        QueryPerformanceCounter(&tDraw.stop);
                        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
                    }
                }
            }
        }
    }
}
// recieves a left index and a right index as parameters
// sorts data using the Stooge sort algorithm
// Stooge sort is a recursive sorting algorithm which is known
// for having a high system time
void Sorting::stoogeSort(int left, int right) {
    ++comparisons;
    if (data[left] > data[right]) {
        // if the data value at the left index index is greater than that at the right, the values are swapped
        swapBars(left, right);
        ++moves;

        // after each swap, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        colouredBars[1] = left;
        colouredBars[2] = right;
        drawBars();
        al_rest(speed / 100);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
    }
    if (right - left > 1) {
        // if there are at least three items between the left and right index,
        // the method recursively calls itself on the first 2/3 of the array, the second 2/3, and then the first 2/3 again
        int i = (right - left + 1) / 3;
        stoogeSort(left, right - i);
        stoogeSort(left + i, right);
        stoogeSort(left, right - i);
    }
}
// recieves no parameters
// sorts data using the Bogo sort algorithm
// Bogo sort is a highly ineffcient algorithm which continues
// to randomly rearrange the data until it has been sorted
void Sorting::bogoSort() {
    bool unsorted = true;    // swapped is set to true
    int iterations = 0;     // iterations is set to 0
    // Bogo sort has been adapted, so that it alternates between directions (left to right and right to left)
    while (unsorted) {
        if (iterations % 2 == 0) {
            for (int i = 0; i < data.size(); ++i) {
                // all indexes (from 0 to data.size() - 1) are swapped with another index at random
                swapBars(i, rand() % data.size());
                ++moves;

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = i;
                drawBars();
                al_rest(speed / 100);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        } else {
            for (int i = data.size() - 1; i >= 0; --i) {
                // all indexes (from data.size() - 1 to 0) are swapped with another index at random
                swapBars(i, rand() % data.size());
                ++moves;

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = i;
                drawBars();
                al_rest(speed / 100);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
        unsorted = false;
        for (int i = 0; i < data.size() - 1; ++i) {
            ++comparisons;
            // method checks if the data has been sorted by comparing each index to the index above
            if (data[i] > data[i + 1]) {
                // if any elements is out of place, unsorted is set to true
                unsorted = true;
            }
        }
        iterations++;   // iterations increases by 1 (to track the direction of Bogo sort)
    }
}
/// Method Implementations: K Sorting Algorithms
/*  Disclaimer: Although these sorting algorithms were written without referencing pseudo code or
 *  specific algorithm descriptions, some contain certain concepts used in previous algorithms.
 *  It is likely that similar variations of these algorithms can be found elsewhere.
*/
// recieves no parameters
// sorts data using the K Sort 1 algorithm
void Sorting::kSort1() {
    bool swapped = true;    // sets swapped to true
    int position = 0;       // sets position to 0
    // for loop locates the index of the largest element
    for (int i = 1; i < data.size(); ++i) {
        ++comparisons;
        if (data[i] > data[position]) {
            position = i;
        }
    }
    ++moves;
    // the largest element is placed in the last positon of the data vector
    swapBars(position, data.size() - 1);

    // while loop continues until iteration occurs in which no swaps take place
    while (swapped) {
        swapped = false;    // swapped is set to false
        for (int j = 0; j < data.size() - 1; ++j) {
            for (int k = 1; j + k < data.size() + 1; ++k) {
                ++comparisons;
                if (data[j] <= data[j + k]) {
                    if (k > 1) {
                        // j is swapped with the index the position before the first element with a greater value
                        swapBars(j, j + k - 1);
                        swapped = true; // swapped is set to true
                        ++moves;

                        // after each swap, the graphical display is updated
                        // QPC is used to account for the time spent drawing
                        QueryPerformanceCounter(&tDraw.start);

                        colouredBars[0] = j + k - 1;
                        drawBars();
                        al_rest(speed / 100);

                        QueryPerformanceCounter(&tDraw.stop);
                        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
                    }
                    break;
                }
            }
        }
    }
    // alternative implementation of K Sort 1
    // significantly reduces the number of comparisons, but increases the number of swaps and the system time
    /*bool swapped = true;

    while (swapped) {
        swapped = false;
        for (int i = 0; i < data.size() - 1; ++i) {
            for (int j = 1; i + j < data.size(); ++j) {
                ++comparisons;
                if (data[i] <= data[i + j]) {
                    colouredBars[0] = i + j;
                    break;
                } else {
                    ++moves;
                    swapBars(i, i + j);
                    swapped = true;

                    QueryPerformanceCounter(&tDraw.start);

                    drawBars();
                    al_rest(speed / 100);

                    QueryPerformanceCounter(&tDraw.stop);
                    drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
                }
            }
        }
    }*/
}
// recieves no parameters
// sorts data using the K Sort 2 algorithm
void Sorting::kSort2() {
    bool swapped;       // sets swapped to true
    int position = 0;   // sets position to 0
    // gap is set to half the size of the data vector, and is divided by two each iteration
    for (float gap = data.size() / 2; gap > 0; gap /= 2) {
        if (gap < 1) {
            gap = 1;    // if gap is less than 1, it is rounded up to 1
        }
        swapped = false;    // swapped is set to false
        // swaps are made outwards (in both directions) from position
        // seperation between comparisons is determined by gap
        for (int i = position; i - gap >= 0; i -= gap) {
            ++comparisons;
            if (data[i] < data[i - gap]) {
                swapBars(i, i - gap);
                swapped = true;
                ++moves;
            }
        }
        for (int i = position; i + gap < data.size(); i += gap) {
            ++comparisons;
            if (data[i] > data[i + gap]) {
                swapBars(i, i + gap);
                swapped = true;
                ++moves;
            }
        }
        // after swaps are made following each gap change, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        drawBars();
        al_rest(speed / 30);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;

        ++position;     // position is incremented by 1 for every iteration of the while loop
        if (!swapped && gap == 1) {
            break;  // if no swaps are made and the gap size is one, the data is sorted
        }
    }
}
// recieves no parameters
// sorts data using the K Sort 3 algorithm
void Sorting::kSort3() {
    bool swapped;
    do {
        swapped = false;    // sets swapped to false
        for (int i = data.size() - 1; i > 0; --i) {
            ++comparisons;
            // if the value at an index is less than the value of the index before, the values are swapped
            if (data[i] < data[i - 1]) {
                ++moves;
                swapBars(i, i - 1);
                swapped = true; // swapped is set to true if a swap occurs in the first if loop
            }
            for (int j = 1; i - j >= 0 && i + j < data.size() - 1; ++j) {
                // comparisons are made outward from the position i
                ++comparisons;
                if (data[i - j] > data[i + j]) {
                    ++moves;
                    swapBars(i - j, i + j);
                }
            }
            // after swaps are made, the graphical display is updated
            // QPC is used to account for the time spent drawing
            QueryPerformanceCounter(&tDraw.start);

            colouredBars[0] = i;
            drawBars();
            al_rest(speed / 30);

            QueryPerformanceCounter(&tDraw.stop);
            drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
        }
    } while (swapped);  // while continues until iteration occurs where no swaps have been made
}
// recieves no parameters
// sorts data using the K Sort 4 algorithm
void Sorting::kSort4(int leftIndex, int rightIndex) {
    // recursive break points
    if (rightIndex <= leftIndex) {
        return;     // if the right index is less than or equal to the left, the method returns
    } else if (rightIndex - leftIndex == 1) {
        ++comparisons;
        // if the right and left index are one value apart, they are compared and swapped if necessary
        // the method then returns
        if (data[rightIndex] < data[leftIndex]) {
            swapBars(rightIndex, leftIndex);
            ++moves;
        }
        return;
    }
    int midIndex = leftIndex + (rightIndex - leftIndex) / 2;    // the midpoint between the two indexes is calculated

    // higher elements are moved to the right side of the interval
    for (int i = leftIndex; i < midIndex; ++i) {
        for (int j = midIndex; j <= rightIndex; ++j) {
            ++comparisons;
            // if there exists a value on the right half of the interval that is smaller than one on the left, the values are swapped
            if (data[i] > data[j]) {
                swapBars(i, j);
                ++moves;

                // after each swap, the graphical display is updated
                // QPC is used to account for the time spent drawing
                QueryPerformanceCounter(&tDraw.start);

                colouredBars[0] = i;
                drawBars();
                al_rest(speed / 50);

                QueryPerformanceCounter(&tDraw.stop);
                drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
            }
        }
    }
    // method is recursively called for the left and right half of the current interval
    kSort4(leftIndex, midIndex - 1);
    kSort4(midIndex, rightIndex);
}
// recieves no parameters
// sorts data using the K Sort 5 algorithm
void Sorting::kSort5() {
    // gap starts off as a tenth of the data size, and is reduced by one each iteration
    for (float gap = data.size() / 10; gap > 0; --gap) {
        bool swapped = false;
        for (int i = 0; i < gap; ++i) {
            for (int j = i ; j < data.size() - gap; j += gap) {
                ++comparisons;
                // comparisons are made based on the gap size
                if (data[j] > data[j + gap]) {
                    swapBars(j, j + gap);
                    swapped = true;
                    ++moves;

                    // after each swap, the graphical display is updated
                    // QPC is used to account for the time spent drawing
                    QueryPerformanceCounter(&tDraw.start);

                    colouredBars[0] = j;
                    drawBars();
                    al_rest(speed / 50);

                    QueryPerformanceCounter(&tDraw.stop);
                    drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
                }
            }
        }
        if (gap == 1) {
            // if an iteration (of gap size 1) occurs where no swaps are made, the for loop ends
            if (!swapped) {
                break;
            }
            gap = 2;    // gap is increased to 2 so the for loop can continue until all the data has been sorted
        }
    }

}
// recieves no parameters
// sorts data using the K Sort 6 algorithm
void Sorting::kSort6() {
    bool swapped = true;    // swapped is set to true
    while (swapped) {       // while loop continues until iteration occurs with no swaps
        swapped = false;
        for (int i = 1; i < data.size() - 1; i += 2) {
            // for loop starts from the second index and increases by two each time
            // compares both the index to the left and the index to the right with the current index
            ++comparisons;
            if (data[i] < data[i - 1]) {
                swapBars(i, i - 1);
                swapped = true;
                ++moves;
            }
            ++comparisons;
            if (data[i] > data[i + 1]) {
                swapBars(i, i + 1);
                swapped = true;
                ++moves;
            }
        }
        for (int i = data.size() - 2; i > 0; i -= 2) {
            // for loop starts from the second last index and decreases by two each time
            // compares both the index to the left and the index to the right with the current index
            ++comparisons;
            if (data[i] > data[i + 1]) {
                swapBars(i, i + 1);
                swapped = true;
                ++moves;
            }
            ++comparisons;
            if (data[i] < data[i - 1]) {
                swapBars(i, i - 1);
                swapped = true;
                ++moves;
            }
        }
        // after each set of swaps, the graphical display is updated
        // QPC is used to account for the time spent drawing
        QueryPerformanceCounter(&tDraw.start);

        drawBars();
        al_rest(speed / 20);

        QueryPerformanceCounter(&tDraw.stop);
        drawDelay += (tDraw.stop.QuadPart - tDraw.start.QuadPart) * 1000000 / Frequency.QuadPart;
    }
}
