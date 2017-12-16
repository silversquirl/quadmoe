/*
 * Upload files to a QuadFile instance
 * Requires libcurl
 */

// Change this to use a different server
#define URL "https://file.quad.moe/"

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#define JSON_IMPLEMENTATION
#include "json.h"

// 1kiB should be more than enough
#define INIT_BUFLEN 1024

#define ERR(msg) fprintf(stderr, "%s\n", msg); exit(1);
#define ERRF(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__); exit(1);

struct qm_data {
  char *buf;
  size_t bufpos;
  size_t buflen;
};

static size_t processCurlData(char *bytes, size_t size, size_t nmemb, void *userdata) {
  struct qm_data *d = userdata;
  size_t nbytes = size * nmemb;

  if (d->bufpos + nbytes > d->buflen) {
    d->buflen *= 2;
    d->buf = realloc(d->buf, d->buflen);
  }

  strncpy(d->buf + d->bufpos, bytes, nbytes);
  d->bufpos += size * nmemb;

  return size * nmemb;
}

int main(int argc, char *argv[]) {
  if (argc <= 1) { ERR("Usage: quadmoe <file>"); }
  char *filename = argv[1];

  // Initialise CURL
  CURL *curl = curl_easy_init();
  if (!curl) { ERR("Failed to initialise CURL") }

  // Initialise form
  struct curl_httppost *post = NULL;
  struct curl_httppost *last = NULL;
  curl_formadd(&post, &last,
      CURLFORM_COPYNAME, "file",
      CURLFORM_FILE, filename,
      CURLFORM_END);

  // Set CURL options
  curl_easy_setopt(curl, CURLOPT_URL, URL);
  curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

  // Initialise callback data
  struct qm_data d;
  d.buflen = INIT_BUFLEN;
  d.bufpos = 0;
  d.buf = malloc(d.buflen);

  // Setup callback
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, processCurlData);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &d);

  // Send the request
  CURLcode err = curl_easy_perform(curl);
  if (err != CURLE_OK) { ERR("The request failed"); }

  // Clean up CURL stuff
  curl_easy_cleanup(curl);
  curl_formfree(post);

  // JSON parsing
  char *json = d.buf;
  size_t len = d.bufpos;

  struct json_token *toks = NULL;
  int read = 0;
  int num = json_num(json, len);
  if (!num) goto badres;
  toks = calloc(num, sizeof (struct json_token));
  enum json_status status = json_load(toks, num, &read, json, len);
  if (status != JSON_OK) goto badres;

  // Basically implementing my own json_query_string here...
  struct json_token *tok = json_query(toks, num, "url");
  if (!tok) goto badres;
  if (tok->type != JSON_STRING) goto badres;
  char *url = malloc(tok->len + 1);
  json_cpy(url, tok->len + 1, tok);

  puts(url);

  // Free stuff
  free(url);
  free(d.buf);
  free(toks);

  return 0;

badres:
  free(toks);
  ERRF("Invalid response from QuadFile instance: %s", json);
}
