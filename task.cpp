#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <errno.h>

using namespace std;

const unsigned int DISPLAY_N_RESULTS = 10;
const char* FILENAME = "test_text.txt";

struct comp
{
  template<typename T>
  bool operator()(const T& l, const T& r) const
  {
      if (l.second == r.second)
        return l.first < r.first;

      return l.second < r.second;
    }
};

void check_file_opened(ifstream &stream);
void increment_counter(vector<pair<string, int> > &vector, string &word);
set<pair<string, int>, comp> most_common(
  vector<pair<string, int> > &vector,
  unsigned int n);

int main () {
  ifstream filestream(FILENAME);
  check_file_opened(filestream);

  char ch;
  int total_combinations = 0;
  string word_buffer;
  string temporary_subword;
  vector<pair<string, int> > counter;
  // loop getting single characters
  while (filestream.get(ch))
  {
    if (isalpha(ch)){
      if (isupper(ch)) ch = tolower(ch);
      // add valid chars to word_buffer
      word_buffer.push_back(ch);
      // if more than 3 chars in word_buffer add the combinations to counter
      if (word_buffer.length() > 3) {
        temporary_subword = word_buffer;
        while (temporary_subword.length() > 3)
        {
          increment_counter(counter, temporary_subword);
          total_combinations += 1;
          // remove first character from word and try adding that combination too
          temporary_subword.erase(temporary_subword.begin());
        }
        // empty the string for next combination
        temporary_subword.clear();
      }
    // only invalid characters reset the word_buffer
    } else {
      word_buffer.clear();
    }
  }
  if (filestream.bad())
  {
    perror("error while reading file");
  }
  filestream.close();

  // print the results
  set<pair<string, int>, comp> top = most_common(counter, DISPLAY_N_RESULTS);
  set<pair<string, int> >::reverse_iterator it2;
  double percent = 0;
  cout << "Frequency:" << endl;
  for(it2 = top.rbegin(); it2 != top.rend(); it2++)
  {
    percent = (it2->second/(double) total_combinations) * 100;
    cout
      << left
      << setw(10)
      << it2->first
      << ": "
      << right
      << fixed
      << setprecision(2)
      << setw(6)
      << percent << "% "
      << left
      << setw(50)
      << string(percent, '=')
      << endl;
  }
  return 0;
}

void check_file_opened(ifstream &stream)
{
  if (stream.fail())
  {
    switch (errno)
    {
      case EACCES:
          // this is set if the drive is not ready in DOS
          cout << "Drive not ready or permission denied" << endl;
          break;
      case ENOENT:
          cout << "Could not find this file" << endl;
          break;
      default:
          perror("opening data file");
    }
  }
}

void increment_counter(
  vector<pair<string, int> > &counter,
  string &word)
{
  // if the word is in the vector add 1 to its counter
  vector<pair<string, int> >::iterator it;
  for(it = counter.begin(); it != counter.end(); it++)
  {
    if(it->first == word)
    {
      it->second += 1;
      return;
    }
  }
  // if the word is not in the vector add it to it
  counter.push_back(pair<string, int>(word,1));
  return;
}

set<pair<string, int>, comp> most_common(
  vector<pair<string, int> > &counter,
  unsigned int n)
{
  set<pair<string, int>, comp> top_n;
  vector<pair<string,int> >::iterator it;
  // make sure you dont try to access values outside the vector
  if(n > counter.size())
  {
    n = counter.size();
  }
  // limiting the scope of i
  {
    // add first n items to set
    unsigned int i = 0;
    it = counter.begin();
    while(i < n)
    {
      top_n.insert(pair<string, int>(it->first, it->second));
      i++;
      it++;
    }
  }
  for(it = counter.begin() + n; it != counter.end(); it++)
  {
    // compare with top most value, if greater then add it to the set
    // remove the top most value which is the smallest
    // this uses reverse order than desired because
    // erasing and comparing at the bottom runs into random segfaults
    if(it->second > top_n.begin()->second)
    {
      top_n.insert(pair<string, int>(it->first, it->second));
      top_n.erase(top_n.begin());
    }
  }
  return top_n;
}

