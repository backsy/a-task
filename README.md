# The task

Create a programme that uses any text file with random content as an input, and checks all the words contained in such text file for letter combinations consisting of more than 3 letters.
Programme output will consist in up to 10 most common letter combinations and their difference in percentage displayed as a table and a bar chart.

## Sample solution:

The content of the input file:
hall feels heels
Output:
Frequencies:
eels: 28.57%
feel: 14.29%
heel: 14.29%
hall: 14.29%
feels: 14.29%
heels: 14.29%

PS: I notice the output is sorted by frequency but not alphabetically.

## My solution:

1. Character stream -> 2. word detector and input buffer -> 3. combinations finder -> 4. storage. -> End of file -> 5. sorting -> 6. table/graph output.

### 1. Character stream.
Reads the file one character at a time.

### 2. Word detector and input buffer.
Has internal memory for several characters. When an invalid character is received the buffer is reset to empty. When there are 4 or more characters in the buffer, the contents of the buffer are emited to the next function in the pipeline. Even after emitting, the contents are kept. Only invalid characters reset the buffer.

### 3. Combinations finder.
The received word is the first combination. It gets emitted into the following function. Remove the first character in the word. If the length is atleast 4 emit that too. Keep removing the first letter and emitting until there are less than 4 characters.

### 4. Storage.
Stores the combination and the occurance count. If the combination exists add 1 to the counter. If it doesnt exist set it to 1. After adding each occurance add 1 to total counter. The data store is a dynamically allocated array of pairs - a vector.

### End of file reached.

### 5. Sorting.
Read through the stored data and keep track of the 10 most used combinations. For efficency the first N elements of the datastore are added to the output which is a self-sorting array - a set. Each leftover element of the datastore is checked against the smallest element in the set. Bigger elements are added to the set and for each addition a single element is removed. Since the set is always sorted, the smallest element gets removed. This way the whole datastore needs to be read through once.

### 6. Print table and diagram.
Table - each row is the combination + the occurance percentage. Percentage is the number of occurances/total combinations * 100%.
Diagram can be an ascii of ===. Percentage * “=” as the bar.
Both need some formating to look nice.

## Extra considerations

The code uses std::isalpha which checks for us-ascii letters. Everything else is considered not a valid letter which in turn means character sequence "akrsärsen" consists of two different valid words.

The code is compiled against c++98. That means none of the nice new features of c++.

The compile command:
> g++ test.cpp -o test -std=c++98 -pedantic

g++ version:
> g++ (GCC) 8.2.1 20181127
