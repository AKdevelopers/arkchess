int GetLEIndex(int rank, int file) {
    // A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
    int squareIndex = (56 - (8 * rank)) + file;

    return squareIndex;
}