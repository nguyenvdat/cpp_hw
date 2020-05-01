#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
using std::cout;    using std::endl;
using std::cerr;     using std::string;
using std::vector;
#include <unordered_set>
#include <unordered_map>
#include <queue>


const string WIKI_ROOT = "https://en.wikipedia.org/wiki/";

std::unordered_set<string> findWikiLinks(const string& page_html) {
  std::unordered_set<string> link_set;
  const string LINK_PATTERN = "<a href=\"/wiki/";
  string::const_iterator i1;
  string::const_iterator i2 = page_html.begin();
  while (true) {
    i1 = std::search(i2, page_html.end(), LINK_PATTERN.begin(), LINK_PATTERN.end());
    if (i1 == page_html.end()) {break;}
    i2 = std::find(i1 + 15, page_html.end(), ' ');
    if (std::all_of(i1 + 15, i2, [](char i) { return i != ':' && i != '#'; }))
    {
      link_set.insert(string(i1 + 15, i2 - 1));
    }
  }
  return link_set;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

string getPageSource(const std::string& page_name) {
  CURL *curl;
  CURLcode res;
  string read_buffer;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, (WIKI_ROOT + page_name).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return read_buffer;
}


vector<string> findWikiLadder(const string& start_page, const string& end_page) {
  string page_html_end = getPageSource(end_page);
  auto link_set_end = findWikiLinks(page_html_end);
  std::unordered_map<string, int> common_count_map;
  auto cmp_fn = [&link_set_end, &common_count_map] (vector<string> ladder1, vector<string> ladder2) {
    string page_name1 = ladder1.back();
    string page_name2 = ladder2.back();
    int common_with_end_count1 = 0;
    int common_with_end_count2 = 0;
    if (common_count_map.find(page_name1) == common_count_map.end()) {
      string page_html1 = getPageSource(page_name1);
      auto link_set1 = findWikiLinks(page_html1);
      for (auto elem : link_set1) {
        if (link_set_end.find(elem) != link_set_end.end()) {
          ++common_with_end_count1;
        }
      }
      common_count_map[page_name1] = common_with_end_count1;
    }
    else {
      common_with_end_count1 = common_count_map[page_name1];
    }
    if (common_count_map.find(page_name1) == common_count_map.end()) {
      string page_html2 = getPageSource(page_name2);
      auto link_set2 = findWikiLinks(page_html2);
      for (auto elem : link_set2) {
        if (link_set_end.find(elem) != link_set_end.end()) {
          ++common_with_end_count2;
        }
      }
      common_count_map[page_name2] = common_with_end_count2;
    }
    else {
      common_count_map[page_name2] = common_with_end_count2;
    }
    // cout << common_with_end_count1 << " " << common_with_end_count2 << endl;
    return common_with_end_count1 < common_with_end_count2;
  };
  std::priority_queue<vector<string>, vector<vector<string>>, decltype(cmp_fn)> ladder_queue(cmp_fn);
  std::unordered_set<string> visited_page_name;
  std::unordered_set<string> top_page_set;
  string top_html_page;
  string top_page_name;
  vector<string> start_page_ladder = {start_page};
  ladder_queue.push(start_page_ladder);
  while (!ladder_queue.empty()) {
    vector<string> top_ladder = ladder_queue.top();
    ladder_queue.pop();
    top_page_name = top_ladder.back();
    visited_page_name.insert(top_page_name);
    top_html_page = getPageSource(top_page_name);
    top_page_set = findWikiLinks(top_html_page);
    if (top_page_set.find(end_page) != top_page_set.end()) {
      top_ladder.push_back(end_page);
      return top_ladder;
    }
    for (auto page_name : top_page_set) {
      if (visited_page_name.find(page_name) == visited_page_name.end()) {
        vector<string> temp_ladder = top_ladder;
        temp_ladder.push_back(page_name);
        ladder_queue.push(temp_ladder);
      }
    }
  }
  return {};
}

int main(void)
{
  // return 0;
  // string page_name = "https://en.wikipedia.org/wiki/Mathematics";
  // string page_html = getPageSource(page_name);
  // cout << page_html << endl;
  // string a = "<p>In <a href=\"/wiki/Topology\">topology</a>, the <b>long line</b> (or<b>Alexandroff line</b>) is a <a href=\"/wiki/Topological_space\">topological space</a> somewhat similar to the <a href=\"/wiki/Real_line\">real line</a>, but in a certain way \"longer\". It behaves locally just like the real line, but has different large-scale properties (e.g., it is neither <a href=\"/wiki/Lindel%C3%B6f_space\">Lindelöf</a> nor <a href=\"/wiki/Separable_space\">separable</a>). Therefore, it serves as one of the basic counterexamples of topology <a href=\"http://www.ams.org/mathscinet-getitem?mr=507446\">[1]</a>. Intuitively, the usual real-number line consists of a countable number of line segments [0,1) laid end-to-end, whereas the long line is constructed from an uncountable number of such segments. You can consult <a href=\"/wiki/Special:BookSources/978-1-55608-010-4\">this</a> book for more information. </p>";
  // auto link_set = findWikiLinks(a);
  // for (auto itr = link_set.begin(); itr != link_set.end(); itr++)
  //   cout << (*itr) << endl;
  string start_page = "Fruit";
  string end_page = "Strawberry";
  vector<string> ladder = findWikiLadder(start_page, end_page);
  for (auto it = ladder.begin(); it != ladder.end(); ++it) {
    cout << (*it) << " ";
  }
  cout << endl;
}