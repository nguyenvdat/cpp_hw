#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
using std::cout;    using std::endl;
using std::cerr;     using std::string;
#include <unordered_set>

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
}