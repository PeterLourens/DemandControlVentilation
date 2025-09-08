#include "task_web_processors.h"

char* webserial_processor(const String &var)
{
    char *webserial_url_tmp = create_webserial_url();

    if (var == "WEBSERIAL_URL")
        return (webserial_url_tmp);

    return nullptr;
}