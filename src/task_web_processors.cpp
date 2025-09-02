#include "task_web_processors.h"

String webserial_processor(const String &var)
{

    String webserial_url_tmp;

    webserial_url_tmp = create_webserial_url();

    if (var == "WEBSERIAL_URL")
        return (webserial_url_tmp);

    return String();
}