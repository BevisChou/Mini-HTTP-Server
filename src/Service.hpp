#include "Util.hpp"
#include "Mapping.hpp"

void deploy(Mapping& mapping);

// Below are custom functions.
Response welcome(const Request& request);
Response html_dopost(const Request& request);
Response html_test(const Request& request);
Response img_logo(const Request& request);
Response txt_test(const Request& request);