#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
using std::cout;    using std::endl;
using std::cerr;     using std::string;
#include <unordered_set>

std::unordered_set<string> findWikiLinks(const string& page_html) {
  std::unordered_set<string> link_set;
  const string LINK_PATTERN = "<a href=\"/wiki/";
  string::const_iterator i1;
  string::const_iterator i2 = page_html.begin();
  while (true) {
    i1 = std::search(i2, page_html.end(), LINK_PATTERN.begin(), LINK_PATTERN.end());
    if (i1 == page_html.end()) {break;}
    i2 = std::find(i1, page_html.end(), '>');
    if (std::all_of(i1 + 15, i2, [](char i) { return i != ':' && i != '#'; }))
    {
      // cout << std::string(i1 + 15, i2 - 1) << endl;
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
    curl_easy_setopt(curl, CURLOPT_URL, page_name.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    std::cout << read_buffer << std::endl;
  }
  return read_buffer;
}

int main(void)
{
  // return 0;
  string page_name = "https://www.wikipedia.org";
  string page_html = getPageSource(page_name);
  cout << page_html << endl;
  string a = "<p>In <a href=\"/wiki/Topology\">topology</a>, the <b>long line</b> (or<b>Alexandroff line</b>) is a <a href=\"/wiki/Topological_space\">topological space</a> somewhat similar to the <a href=\"/wiki/Real_line\">real line</a>, but in a certain way \"longer\". It behaves locally just like the real line, but has different large-scale properties (e.g., it is neither <a href=\"/wiki/Lindel%C3%B6f_space\">Lindelöf</a> nor <a href=\"/wiki/Separable_space\">separable</a>). Therefore, it serves as one of the basic counterexamples of topology <a href=\"http://www.ams.org/mathscinet-getitem?mr=507446\">[1]</a>. Intuitively, the usual real-number line consists of a countable number of line segments [0,1) laid end-to-end, whereas the long line is constructed from an uncountable number of such segments. You can consult <a href=\"/wiki/Special:BookSources/978-1-55608-010-4\">this</a> book for more information. </p>";
  auto link_set = findWikiLinks(a);
  for (auto itr = link_set.begin(); itr != link_set.end(); itr++)
    cout << (*itr) << endl;
}